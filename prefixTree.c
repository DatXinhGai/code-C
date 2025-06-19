#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdint.h>
#include <stddef.h>
#include <assert.h>

#define DFS 1

// CÓ THỂ VIẾT CÂY PREFIX THEO KIỂU CÂY TỔNG QUÁT VỚI FIRST CHILD VÀ NEXT SIBLING
// dễ tùy biến, áp dụng được cho các kí tự trong Unicode, có con thì mới thêm vào
// áp dụng tốt cho trie thưa, các từ có độ dài ngắn không đồng đều
// tiết kiệm bộ nhớ


// các kí tự được chấp nhận từ 33 đến 126

// lưu số kí tự tối đa gợi ý lấy từ dfs
const int MAX_SUGGEST = 64;

// pading cho index của cây
const int LOW_PAD = 26;
const int ADD_PAD = 52;

// khoảng cách hoa thường
const int LOW_CAP_DIF = abs('A' - 'a');

// khoảng chấp nhận kí tự trong bảng mã ASCII
const int LOW_RES = 33;
const int HIGH_RES = 126;

// children_size
int ALPHABET_SIZE;

// lưu các kí tự không phải chữ cái
char* add_digit;    // lưu kí tự thêm
int* add_index;     // vị trí nút con trong cây
int add_size = 0;

FILE* out_ptr;

// flexible array member(FAM) phải ở cuối của struct!!!
// cấp phát khi chạy, linh hoạt, từ C99
struct TrieNode {
    bool end_of_word;
    int frequency;
    struct TrieNode* children[];
};

typedef struct TrieNode TrieNode;



// CÁC CẤU TRÚC DỮ LIỆU VÀ THUẬT TOÁN HỖ TRỢ

//

// QUICK SELECT chọn top_k với thời gian trung bình O(n)
typedef struct {
    char word[100];
    int frequency;
} Element;

void MedianOfThree(Element** ele_arr, int left, int right) {
    int mid = (left + right) / 2;
    Element* temp;
    if (ele_arr[mid]->frequency < ele_arr[left]->frequency) {
        temp = ele_arr[mid];
        ele_arr[mid] = ele_arr[left];
        ele_arr[left] = temp;
    }

    if (ele_arr[mid]->frequency < ele_arr[right]->frequency) {
        temp = ele_arr[mid];
        ele_arr[mid] = ele_arr[right];
        ele_arr[right] = temp;
    }
    if (ele_arr[left]->frequency < ele_arr[right]->frequency) {
        temp = ele_arr[left];
        ele_arr[left] = ele_arr[right];
        ele_arr[right] = temp;
    }
}

int HoarePartition(Element** ele_arr, int left, int right) {
    if (left >= right) {
        return right;
    }

    int pivotValue = ele_arr[left]->frequency;
    int cs_left = left + 1;
    int cs_right = right;

    while (true) {
        while (ele_arr[cs_left]->frequency > pivotValue) {
            cs_left++;
        }

        while (ele_arr[cs_right]->frequency < pivotValue) {
            cs_right--;
        }

        if (cs_left >= cs_right) {
            int pivotIndex = cs_right;
            Element* temp = ele_arr[left];
            ele_arr[left] = ele_arr[pivotIndex];
            ele_arr[pivotIndex] = temp;
            return pivotIndex;
        }

        Element* temp = ele_arr[cs_left];
        ele_arr[cs_left++] = ele_arr[cs_right];
        ele_arr[cs_right--] = temp;
    }
}

void quickSelect(Element** ele_arr, int left, int right, int count) {
    MedianOfThree(ele_arr, left, right);

    int pivotIndex = HoarePartition(ele_arr, left, right);

    int done = pivotIndex - left + 1;

    if (done == count) { return; }

    if (done < count) {
        quickSelect(ele_arr, pivotIndex + 1, right, count - done);
    } else {
        quickSelect(ele_arr, left, pivotIndex - 1, count);
    }
}
//

// lấy đầu vào số nguyên
int getInteger() {
    char temp[1024];

    fgets(temp, 1023, stdin);

    temp[strcspn(temp, "\r\n")] = '\0';

    char* n_ptr = temp;

    while (*n_ptr == ' ') {
        n_ptr++;
    }

    int integer = strtol(n_ptr, NULL, 10);
    return integer;
}

// cách khác là sprintf xong ghi là "%X"
char* convertToHex(int n) {
    // sprintf(temp, "%X", n);
    if (n == 0) {
        return strdup("0");
    }
    char* temp = (char*)malloc(16 * sizeof(char));
    int temp_index = 0;


    while (n) {
        int val = n % 16;
        if (val < 10) {
            temp[temp_index++] = val + '0';
        } else {
            temp[temp_index++] = val - 10 + 'A';
        }
        n /= 16;
    }

    temp[temp_index] = '\0';
    // hàm đảo chuỗi
    strrev(temp);
    return temp;
}

// lấy đầu vào là 1 dòng
char* readBuffer() {
    int alloc_len = 1024;
    int data_len = 0;

    char* data = (char*)malloc(alloc_len);

    while (true) {
        char* cur_sor = data + data_len;
        char* line = fgets(cur_sor, alloc_len - data_len, stdin);

        if (!line) {
            break;
        }

        data_len += strlen(line);

        if (data_len < alloc_len || data[data_len - 1] == '\n') {
            break;
        }

        alloc_len <<= 1;
        data = (char*)realloc(data, alloc_len);

        if (!data) {
            break;
        }
    }

    if (data[data_len - 1] == '\n') {
        data[--data_len] = '\0';
    } else {
        data[data_len] = '\0';
    }

    data = (char*)realloc(data, data_len + 1);

    return data;
}


// tìm vị trí đầu của từ cuối trong buffer
char* findLastPos(char* buffer) {
    char* ptr = buffer + strlen(buffer);
    // chạy đến khi gặp cách trống đầu tiên hoặc không tìm được(chỉ có 1 từ)
    while (ptr > buffer && *(ptr - 1) != ' ') {
        ptr--;
    }

    return ptr;

}

int returnFileSize(FILE* f_ptr) {
    // đưa con trỏ file về cuối
    fseek(f_ptr, 0, SEEK_END);
    // lấy size, ftell trả về độ dịch so với ban đầu
    int size = ftell(f_ptr);
    // đưa lại về đầu để đọc
    rewind(f_ptr);

    return size;
}

int chooseStream() {
    puts("Choose input stream:\n"
                 "1.stdin\n"
                 "2.file\n");
    int num = getInteger();

    return num;
}

// nhớ phải truyền vào địa chỉ của buffer, tránh gán bản sao local
void getInputFromStream(FILE* p_ptr, char** buffer, int* paraSize) {

    if (p_ptr == stdin) {
        *buffer = readBuffer();
        *paraSize = strlen(*buffer);
    } else {
        *paraSize = returnFileSize(p_ptr);
        *buffer = (char*)malloc(*paraSize + 1);
        size_t byteSizes = fread(*buffer, sizeof(char), *paraSize, p_ptr);
        if (byteSizes != *paraSize) {
            fprintf(stderr, "Lỗi khi đọc file\n");
            exit(EXIT_FAILURE);
        }
    }

    (*buffer)[*paraSize] = '\0';
}

bool compareWithoutCaseSensitive(const char* sample, const char* origin) {
    int sample_len = strlen(sample);
    int origin_len = strlen(origin);
    if (sample_len < origin_len) {
        return false;
    }

    for (int i = 0; i < origin_len; i++) {
        int dif = abs(sample[i] - origin[i]);
        if (dif != 0 && dif != LOW_CAP_DIF) {
            return false;
        }
    }

    return true;
}

bool compareWithCaseSensitive(const char* sample, const char* origin) {
    int sample_len = strlen(sample);
    int origin_len = strlen(origin);
    if (sample_len < origin_len) {
        return false;
    }

    for (int i = 0; i < origin_len; i++) {
        int dif = abs(sample[i] - origin[i]);
        if (dif != 0) {
            return false;
        }
    }

    return true;
}


// cấu trúc hàng đợi cho bfs
struct QNode {
    TrieNode* trie_node;
    char cur_word[100];
    struct QNode* next;
};

typedef struct QNode QNode;

typedef struct Queue {
    QNode* front;
    QNode* rear;
} Queue;
// khởi tạo queue
Queue* queue;

QNode* createQNode(TrieNode* trie_node) {
    QNode* new_node = (QNode*)malloc(sizeof(QNode));

    new_node->trie_node = trie_node;
    new_node->next = NULL;
    return new_node;
}

Queue* createQueue() {
    Queue* new_queue = (Queue*)malloc(sizeof(Queue));
    new_queue->front = new_queue->rear = NULL;

    return new_queue;
}

void queuePush(TrieNode* trie_node) {
    QNode* new_node = createQNode(trie_node);

    if (!queue->front) {
        queue->front = queue->rear = new_node;
    } else {
        queue->rear->next = new_node;
        queue->rear = new_node;
    }
}

QNode* queuePop() {
    if (!queue->front) {
        exit(EXIT_FAILURE);
    }

    QNode* return_node = queue->front;
    queue->front = queue->front->next;
    if (queue->front == NULL) {
        queue->front = queue->rear = NULL;
    }

    return return_node;
}

bool isEmptyQueue() {
    return (queue->front == NULL);
}

void freeQueueNode() {
    QNode* cur = queue->front;

    while (cur) {
        QNode* temp = cur;
        cur = cur->next;
        free(temp);
    }

    queue->front = queue->rear = NULL;
}
//

// trả về index ứng với kí tự
int returnIndex(const char* ptr) {
    int index = 0;

     if (*ptr >= 'A' && *ptr <= 'Z') {
        index = *ptr - 'A';
    } else if (*ptr >= 'a' && *ptr <= 'z') {
        index = *ptr - 'a' + LOW_PAD;
    } else {
        for (int i = 0; i < add_size; i++) {
            if (*ptr == add_digit[i]) {
                index = add_index[i];
                break;
            }
        }
    }

    return index;
}

// trả về kí tự ứng với index
char returnDigit(int index) {
    char digit;
    if (index < LOW_PAD) {
        digit = 'A' + index;
    } else if (index < ADD_PAD) {
        digit = 'a' + (index - LOW_PAD);
    } else {
        digit = add_digit[index - ADD_PAD];
    }
    return digit;
}



// các hàm cơ bản của TRIE
////
///
TrieNode* createTrieNode() {
    // cấp phát cha và con ở ngay bên cạnh luôn
    // đặc biệt, dùng cho FAM với size chưa biết trước
    // FAM không tính vào sizeof(struct), mà nối ngay đằng sau khi được cấp phát cùng struct
    // nhưng có thể cấp phát thừa 1 ít byte do lấy luôn phần padding của struct để bắt đầu

    //TrieNode* new_node = (TrieNode*)malloc(sizeof(TrieNode) + ALPHABET_SIZE * sizeof(TrieNode*));

    // nên là offsetof, tính chính xác số byte từ đầu struct đến thành phần FAM
    // hoặc cách khác đơn giản hơn là chơi mảng động các con trỏ, khi đó cần cấp phát 2 lần, dễ leak hơn
    TrieNode* new_node = (TrieNode*)malloc(offsetof(TrieNode, children) + ALPHABET_SIZE * sizeof(TrieNode*));

    for (int i = 0; i < ALPHABET_SIZE; i++) {
        new_node->children[i] = NULL;
    }

    new_node->end_of_word = false;
    new_node->frequency = 0;

    return new_node;
}

// tham số pos là hình thức để truyền vào hàm lấy đối số là địa chỉ hàm thôi
bool addWord(TrieNode* root, const char* word) {
    TrieNode* cur = root;
    const char* ptr = word;

    while (*ptr) {
        int index = returnIndex(ptr);

        if (!cur->children[index]) {
            cur->children[index] = createTrieNode();
        }

        cur = cur->children[index];
        ptr++;
    }

    cur->frequency++;
    cur->end_of_word = true;

    return true;
}



void traverseTrie(const TrieNode* root, char* word, int level) {
    if (root == NULL) {
        return ;
    }

    if (root->end_of_word) {
        word[level] = '\0';
        puts(word);
    }

    for (int i = 0; i < ALPHABET_SIZE; i++) {

        if (root->children[i]) {
            char digit = returnDigit(i);
            word[level] = digit;
            traverseTrie(root->children[i], word, level + 1);
        }
    }
}

TrieNode* searchNode(TrieNode* root, const char* word, int pos) {
    if (root == NULL || word[pos] == '\0') {
        return root;
    }

    int index = returnIndex(word + pos);
    return searchNode(root->children[index], word, pos + 1);
}

TrieNode* searchWord(TrieNode* root, const char* word) {
    TrieNode* ret_node = searchNode(root, word, 0);

    if (!ret_node->end_of_word) {
        fputs(word, out_ptr);
        fputc('\n', out_ptr);

        puts(word);
        return NULL;
    } else {
        //puts("Tìm thấy!");
        return ret_node;
    }
}

void freeTrieNode(TrieNode** freed_trie_node) {
    free(*freed_trie_node);
    *freed_trie_node = NULL;    // bắt buộc phải gán NULL;
}

int countChildren(TrieNode* root) {
    int count = 0;
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if (root->children[i]) {
            count++;
        }
    }

    return count;
}


int deleteWord(TrieNode** root, const char* word, int pos) {
    // nhánh cụt hoặc không chứa từ
    if (*root == NULL || (word[pos] == '\0' && (*root)->end_of_word == false)) {
        return 0;
    }

    if (word[pos] == '\0' && (*root)->end_of_word == true) {
        int childCount = countChildren(*root);
        if (childCount == 0) {
            // nếu là nút lá thì trả về 1, backtrack lên để xóa
            freeTrieNode(root);
            return 1;
        } else {
            // nếu phía dưới còn từ thì trả về 0, tức không xóa lên,
            // và chỉ bỏ kết thúc từ
            (*root)->end_of_word = false;
            return 0;
        }
    }

    int index = returnIndex(word + pos);
    int childRes = deleteWord(&((*root)->children[index]), word, pos + 1);

    int rootChildCount = countChildren(*root);
    // chỉ xóa khi có tín hiệu xóa từ con,
    // nó không còn con và không phải kết thúc từ!
    if (childRes && rootChildCount == 0 && (*root)->end_of_word == false) {
        freeTrieNode(root);
        return 1;   // backtrack xóa tiếp
    }

    return 0;
}

void chooseWordsWithBFS(TrieNode* root, char* cur_word, int suggest_count, Element*** ele_arr, int* ele_size) {

    int max_size = 1;
    *ele_size = 0;

    freeQueueNode();


    queuePush(root);
    strcpy(queue->front->cur_word, cur_word);

    while (!isEmptyQueue() && *ele_size < MAX_SUGGEST) {
        QNode* top = queuePop();
        TrieNode* top_trie = top->trie_node;
        char temp_word[100];
        strcpy(temp_word, top->cur_word);
        int temp_len = strlen(temp_word);


        if (top_trie->end_of_word) {
            (*ele_arr)[*ele_size] = (Element*)malloc(sizeof(Element));
            strcpy((*ele_arr)[*ele_size]->word, temp_word);
            (*ele_arr)[(*ele_size)++]->frequency = top_trie->frequency;

            if (*ele_size >= max_size) {
                max_size <<= 1;
                *ele_arr = (Element**)realloc(*ele_arr, max_size * sizeof(char*));
            }
        }

        for (int i = 0; i < ALPHABET_SIZE; i++) {
            if (top_trie->children[i]) {
                queuePush(top_trie->children[i]);

                char child_word[100];
                strcpy(child_word, temp_word);
                child_word[temp_len] = returnDigit(i);
                child_word[temp_len + 1] = '\0';

                strcpy(queue->rear->cur_word, child_word);
            }
        }

        free(top);
    }

    freeQueueNode();

    *ele_arr = (Element**)realloc(*ele_arr, *ele_size * sizeof(char*));
}

void chooseWordsWithDFS(TrieNode* root, char* cur_word, char* temp_word, int temp_pos,
    Element*** ele_arr, int* ele_size, int* max_size, bool (*cmp_func) (const char* sample, const char* origin)) {

    if (root == NULL || *ele_size >= MAX_SUGGEST) {
        return ;
    }

    temp_word[temp_pos] = '\0';

    if (root->end_of_word == true && cmp_func(temp_word, cur_word)) {
        (*ele_arr)[*ele_size] = (Element*)malloc(sizeof(Element));
        strcpy((*ele_arr)[*ele_size]->word, temp_word);
        (*ele_arr)[(*ele_size)++]->frequency = root->frequency;

        if (*ele_size >= *max_size) {
            *max_size <<= 1;
            *ele_arr = (Element**)realloc(*ele_arr, *max_size * sizeof(Element*));
        }
    }

    for (int i = 0; i < ALPHABET_SIZE; i++) {
        TrieNode* child = root->children[i];
        temp_word[temp_pos] = returnDigit(i);

        chooseWordsWithDFS(child, cur_word, temp_word, temp_pos + 1, ele_arr, ele_size, max_size, cmp_func);
    }
}

void chooseMostFrequentWords(TrieNode* root, char* cur_word, int suggest_count, bool BFS,
    bool case_sensitive, char*** res, int* res_count) {
    Element** ele_arr = (Element**)malloc(sizeof(Element*));
    int ele_size = 0;
    int max_size = 1;

    char buf[1024];
    strcpy(buf, cur_word);
    int buf_pos = strlen(buf);
    TrieNode* cur_node = searchNode(root, cur_word, 0);

    if (cur_node == NULL) {
        *res_count = 0;
        return;
    }

    if (BFS) {
        chooseWordsWithBFS(cur_node, cur_word, buf, &ele_arr, &ele_size);
    } else if (case_sensitive) {
        chooseWordsWithDFS(cur_node, cur_word, buf, buf_pos, &ele_arr, &ele_size, &max_size,
            compareWithCaseSensitive);
    } else {
        buf[0] = '\0';
        chooseWordsWithDFS(root, cur_word, buf, 0, &ele_arr, &ele_size, &max_size,
            compareWithoutCaseSensitive);
    }


    if (ele_size > suggest_count) {
        quickSelect(ele_arr, 0, ele_size - 1, suggest_count);
    }

    *res_count = (ele_size < suggest_count)? ele_size : suggest_count;

    *res = (char**)malloc(*res_count * sizeof(char*));

    for (int i = 0; i < *res_count; i++) {
        (*res)[i] = strdup(ele_arr[i]->word);
    }

    free(ele_arr);
}

// DFS thì dễ stackoverflow

void freeTrie(TrieNode* root) {
    if (root == NULL) {
        return ;
    }

    for (int i = 0 ; i < ALPHABET_SIZE; i++) {
        freeTrie(root->children[i]);
    }

    free(root);
}

void freeTrieWithBFS(TrieNode* root) {

    if (root == NULL) { return; }

    queuePush(root);

    while (!isEmptyQueue()) {
        QNode* last_qnode = queuePop();
        TrieNode* last_trie_node = last_qnode->trie_node;

        for (int i = 0; i < ALPHABET_SIZE; i++) {
            if (last_trie_node->children[i] != NULL) {
                queuePush(last_trie_node->children[i]);
            }
        }

        free(last_trie_node);
        free(last_qnode);
    }
}
/////




void printSelection() {
    puts("1. Add without tokenize");
    puts("2. Add with tokenize");
    puts("3. Add from previous data");
    puts("4. Check word from paragraph");
    puts("5. Autocomplete");
    puts("6. Search word");
    puts("7. Delete word");
    puts("8. Save Trie");
}


// hàm để nhận thêm những digit không phải là kí tự chữ
char* getAdditionalDigit(FILE* f_ptr, int fileSize, int* res_size) {
    int fr_map[256] = {0};

    char* buffer = (char*)malloc(fileSize + 1);

    // đọc 1 lần tất cả các kí tự vào buffer để khỏi truy cập ra bộ nhớ ngoài
    int byteSizes = fread(buffer, sizeof(char), fileSize, f_ptr);
    if (byteSizes < fileSize) {
        fprintf(stderr, "Error when read file\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < fileSize; i++) {
        fr_map[buffer[i]]++;
    }

    char* res = (char*)malloc(256);
    *res_size = 0;

    for (int i = LOW_RES; i < HIGH_RES; i++) {
        if (fr_map[i] && !(i >= 'a' && i <= 'z') && !(i >= 'A' && i <= 'Z')) {
            res[*res_size] = i;
            (*res_size)++;
        }
    }

    res = (char*)realloc(res, *res_size);

    free(buffer);

    return res;
}


void addAllWords(TrieNode* root, FILE* f_ptr, int fileSize) {

    char* buffer = (char*)malloc(fileSize + 1);
    // lấy về size và gán cho buffer để tránh không có '\0'
    size_t byteSizes = fread(buffer, sizeof(char), fileSize, f_ptr);

    buffer[byteSizes] = '\0';

    char* token = strtok(buffer, "\n");

    while (true) {
        if (!token) {
            break ;
        }

        token[strcspn(token, "\r\n")] = '\0';

        addWord(root, token);

        // tiếp tục từ phần trước đó
        token = strtok(NULL, "\n");
    }

    puts("Cập nhật thành công!");

    free(buffer);
}

void processParagraph(TrieNode* root, FILE* p_ptr,
    bool (*processFunc)(TrieNode* root, const char* word)) {
    // frequency_map để truy cập nhanh xem kí tự hiện tại có trong từ điển hay không
    int fr_map[256] = {0};
    for (int i = 'a'; i <= 'z'; i++) {
        fr_map[i] = 1;
    }
    for (int i = 'A'; i <= 'Z'; i++) {
        fr_map[i] = 1;
    }
    for (int i = 0; i < add_size; i++) {
        fr_map[add_digit[i]] = 1;
    }

    // bắt buộc phải bỏ dấu
    fr_map[','] = fr_map['.'] = fr_map['!'] = fr_map['?'] = 0;
    fr_map['-'] = fr_map[39] = 1;

    char* buffer = NULL;
    int paraSize = 0;
    // lấy đầu vào tùy biến
    getInputFromStream(p_ptr, &buffer, &paraSize);

    // chạy con trỏ ptr trên buffer, kết thúc từ thì gọi searchWord và in ra luôn

    char* n_ptr = buffer;
    char temp_word[1024];
    char temp_index = 0;
    bool word_start = false;

    while (*n_ptr) {
        if (!word_start) {  // nếu chưa bắt đầu
            if (fr_map[*n_ptr]) {   // chứa kí tự của từ
                word_start = true;
                temp_word[temp_index++] = *n_ptr;
            }
        } else {
            if (fr_map[*n_ptr]) {
                temp_word[temp_index++] = *n_ptr;
            } else {    // gặp kí tự kết thúc
                temp_word[temp_index] = '\0';

                processFunc(root, temp_word);

                // đặt lại cái giá trị
                temp_index = 0;
                word_start = false;
            }
        }

        n_ptr++;
    }

    free(buffer);
}

void saveInfo(FILE* save_ptr) {
    fprintf(save_ptr, "%d\n", ALPHABET_SIZE);

    fprintf(save_ptr, "%d\n", add_size);

    for (int i = 0; i < add_size; i++) {
        fprintf(save_ptr, "%c", add_digit[i]);
    }


    fputc('\n', save_ptr);

    for (int i = 0; i < add_size; i++) {
        fprintf(save_ptr, "%d ", add_index[i]);
    }

    fputc('\n', save_ptr);
}

// khôi phục thông tin về cây
void recoverInfo(FILE* save_ptr) {
    char buf[1024] = "\0";

    fgets(buf, 1023, save_ptr);
    ALPHABET_SIZE = strtol(buf, NULL, 10);

    fgets(buf, 1023, save_ptr);
    add_size = strtol(buf, NULL, 10);

    fgets(buf, 1023, save_ptr);
    add_digit = (int*)malloc(add_size * sizeof(int));
    for (int i = 0; i < add_size; i++) {
        add_digit[i] = buf[i];
    }

    fgets(buf, 1023, save_ptr);
    add_index = (int*)malloc(add_size * sizeof(int));

    int index_pos = 0;

    char* n_ptr = buf;
    char* end_ptr = buf;

    while (*n_ptr != '\n') {
        if (*n_ptr == ' ') {
            *n_ptr = '\0';

            add_index[index_pos++] = strtol(end_ptr, NULL, 10);

            end_ptr = n_ptr + 1;
        }
        n_ptr++;
    }

    *n_ptr = '\0';

    add_index[index_pos] = strtol(end_ptr, NULL, 10);
}

// VIỆC ĐỌC VÀ GHI FILE Ở DƯỚI ĐÂY CÓ THỂ DÙNG 1 MẢNG CHAR BUFFER
// GHI HẾT RA BUFFER THEO BLOCK RỒI SAU ĐÓ MỚI XỬ LÍ
// SẼ HẠN CHẾ VIỆC TRUY CẬP BỘ NHỚ NGOÀI THƯỜNG XUYÊN
// TUY NHIÊN PHỨC TẠP HƠN

// lưu cây với cặp ngoặc lồng
// cha chứa cặp ngoặc trong đó là thông tin về con của nó
// cấu trúc là [digit][(*)(nếu kết thúc từ)][frequency(hệ 16)]


void saveTrieWithDFS(TrieNode* root, FILE* save_ptr) {


    if (root == NULL) {
        return;
    }

    fputc('(', save_ptr);

    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if (root->children[i]) {

            fputc(returnDigit(i), save_ptr);

            if (root->children[i]->end_of_word) {
                fputc('*', save_ptr);
            }


            char* hex_str = convertToHex(root->children[i]->frequency);
            fputs(hex_str, save_ptr);

            free(hex_str);


            saveTrieWithDFS(root->children[i], save_ptr);
        }
    }

    fputc(')', save_ptr);
}


// lưu cây theo lớp, các con cách nhau bởi dấu '|', kết thúc bằng dấu '\n'
// cấu trúc là [digit][*][frequency]['|']
void saveTrieWithBFS(TrieNode* root, FILE* save_ptr) {

    saveInfo(save_ptr);

    freeQueueNode();

    queuePush(root);

    while (!isEmptyQueue()) {
        QNode* last = queuePop();
        TrieNode* last_trie_node = last->trie_node;

        for (int i = 0; i < ALPHABET_SIZE; i++) {
            if (last_trie_node->children[i] != NULL) {

                queuePush(last_trie_node->children[i]);

                char temp[32] = "\0";
                int temp_index = 0;

                temp[temp_index++] = returnDigit(i);
                if (last_trie_node->children[i]->end_of_word) {
                    temp[temp_index++] = '*';
                }

                char* hex_nums = convertToHex(last_trie_node->children[i]->frequency);
                strcat(temp, hex_nums);
                free(hex_nums);

                temp_index = strlen(temp);
                temp[temp_index++] = '|';
                temp[temp_index++] = '\0';

                fputs(temp, save_ptr);
            }
        }

        fputc('\n', save_ptr);
    }

    freeQueueNode();
}

// lưu ý feof không dùng để điều khiển luồng, chỉ check khi đã đọc quá EOF của file
// bản chất là dùng stack ẩn của hệ thống
// ý tưởng là xây lại cây từ ngọn, khác với BFS xây cây từ gốc theo mức


void recoverTrieWithDFS(TrieNode* root, FILE* save_ptr) {


    int parenthesis = fgetc(save_ptr);

    // bắt buộc là ngoặc bắt đầu
    if (parenthesis != '(') {
        fprintf(stderr, "Lỗi");
        exit(EXIT_FAILURE);
    }

    while (true) {

        char digit = fgetc(save_ptr);
        if (digit == ')') {
            return; // kết thúc của nút con
        }

        // tìm index và tạo cây
        int index = returnIndex(&digit);
        root->children[index] = createTrieNode();

        char ch = fgetc(save_ptr);
        char num[16];
        int num_index = 0;

        if (ch == '*') {
            root->children[index]->end_of_word = 1;
            ch = fgetc(save_ptr); // lấy kí tự sau '*'
        }

        // ch lúc này chắc chắn là 1 phần của num
        num[num_index++] = ch;

        while (true) {

            char next = fgetc(save_ptr);

            if (next != EOF && isxdigit(next)) {    // kiểm tra xem có là kí tự hệ 16
                num[num_index++] = next;
            } else {
                if (next != EOF ) {
                    ungetc(next, save_ptr); // không phải thì đẩy ngược kí tự vào luồng để đọc lại lần kế
                }                           // chỉ đẩy lại luồng chứ không ghi vào file
                break;
            }
        }

        num[num_index] = '\0';

        int dec_val = strtol(num, NULL, 16);
        root->children[index]->frequency = dec_val;
        // đệ quy cho nút con
        recoverTrieWithDFS(root->children[index], save_ptr);
    }
}


void recoverTrieWithBFS(TrieNode* root, FILE* save_ptr) {

    freeQueueNode();

    queuePush(root);

    while (!isEmptyQueue()) {   // vòng lặp khi còn node
        QNode* last = queuePop();
        TrieNode* last_trie_node = last->trie_node;

        while (true) {  // vòng lặp khi còn con
            char digit = fgetc(save_ptr);

            if (digit == EOF) { // nếu cuối file
                freeQueueNode();

                return;
            }

            if (digit == '\n') { break; }   // hết con

            // tạo nút mới và gắn vào con của cha
            TrieNode* child = createTrieNode();
            int index = returnIndex(&digit);
            last_trie_node->children[index] = child;

            int ch = getc(save_ptr);
            if (ch == '*') {
                last_trie_node->children[index]->end_of_word = true;
                ch = getc(save_ptr);
            }

            // lúc này ch chắc chắn thuộc frequency
            char num[16];
            int num_index = 0;
            num[num_index++] = ch;

            while (true) {
                ch = getc(save_ptr);

                if (!isalnum(ch)) { // kết thúc fre, tức gặp '|'
                    num[num_index] = '\0';

                    int dec_val = strtol(num, NULL, 16);

                    last_trie_node->children[index]->frequency = dec_val;

                    break;
                }

                num[num_index++] = ch;
            }

            // sau khi xong thì tống con mới vào hàng đợi
            queuePush(last_trie_node->children[index]);
        }
    }

}

int prefixTree() {

    queue = createQueue();
    TrieNode* root = NULL;

    // file để ghi những từ không có trong từ điển
    out_ptr = fopen("output.txt", "w");
    if (!out_ptr) {
        exit(EXIT_FAILURE);
    }

    // file để ghi dữ liệu cây ra ngoài, lưu trữ
    char save_name[] = "save1.txt";

    bool more = true;

    while (more) {
        printSelection();
        int choice = getInteger();

        if (choice < 0) {
            break;
        }

        switch (choice) {
            case 1: {   // đầu vào là từ điển
                // xử lí file
                char dict_name[] = "dictionary.txt";
                FILE* f_ptr = fopen(dict_name, "rb");
                if (!f_ptr) {
                    fprintf(stderr, "Lỗi mở file\n");
                    exit(EXIT_FAILURE);
                }
                int fileSize = returnFileSize(f_ptr);
                rewind(f_ptr);

                if (!root) {
                    // tìm những kí tự đặc biệt được thêm vào
                    add_size = 0;
                    add_digit = getAdditionalDigit(f_ptr, fileSize, &add_size);
                    add_index = (int*)malloc(add_size * sizeof(int));
                    for (int i = 0; i < add_size; i++) {
                        add_index[i] = ADD_PAD + i;
                    }

                    ALPHABET_SIZE = ADD_PAD + add_size;

                    //khởi tạo
                    root = createTrieNode();
                }

                // thêm từ
                rewind(f_ptr);
                addAllWords(root, f_ptr, fileSize);

                //char buf[1024];
                //traverseTrie(root, buf, 0);

                fclose(f_ptr);
                break;
            }   // thêm từ từ điển
            case 2: {
                FILE* p_ptr;

                int stream = chooseStream();

                if (stream == 1) {
                    p_ptr = stdin;
                } else {
                    char para_name[] = "paragraph.txt";
                    p_ptr = fopen(para_name, "w+");
                    if (!p_ptr) {
                        fprintf(stderr, "Lỗi mở file\n");
                        exit(EXIT_FAILURE);
                    }
                }

                if (!root) {
                    // chấp nhận 2 dấu là ' và -
                    ALPHABET_SIZE = 54;
                    add_size = 2;
                    add_digit = (char*)malloc(2);
                    add_index = (int*)malloc(2*sizeof(int));
                    add_digit[0] = 39;
                    add_digit[1] = '-';
                    add_index[0] = 52;
                    add_index[1] = 53;


                    root = createTrieNode();
                }

                processParagraph(root, p_ptr, addWord);

                //char buffer[1024];
                //traverseTrie(root, buffer, 0);

                fclose(p_ptr);
                break;
            }   // thêm từ đoạn văn bản
            case 3: {
                if (root) {
                    freeTrieWithBFS(root);
                }

                FILE* save_ptr = fopen(save_name, "r");
                assert(save_ptr);

                // phải khôi phục dữ liệu gốc rồi mới được createNode
                recoverInfo(save_ptr);

                root = createTrieNode();

                if (DFS) {
                    recoverTrieWithDFS(root, save_ptr);
                } else {
                    recoverTrieWithBFS(root, save_ptr);
                }


                fclose(save_ptr);

                //char buffer1[1024];
                //traverseTrie(root, buffer1, 0);
                break;
            }   // đọc từ tệp đã lưu cây trước đó
            case 4: {
                if (!root) {
                    puts("Chưa có dữ liệu để tìm kiếm!");
                    break;
                }
                // lấy đoạn văn bản đầu vào và xử lí
                FILE* p_ptr;

                int stream = chooseStream();

                if (stream == 1) {
                    p_ptr = stdin;
                } else {
                    char para_name[] = "paragraph.txt";
                    p_ptr = fopen(para_name, "r");
                    if (!p_ptr) {
                        fprintf(stderr, "Lỗi mở file\n");
                        exit(EXIT_FAILURE);
                    }
                }


                rewind(p_ptr);
                // hàm lấy đối số là hàm
                processParagraph(root, p_ptr, searchWord);

                fclose(p_ptr);
                break;
            }   // tìm và in ra những từ không có trong từ điển
            case 5: {
                puts("Chọn số lượng từ suggest");
                int suggest_count = getInteger();
                char buffer[100000];
                // quên dòng này rồi strcat thì thôi
                buffer[0] = '\0';

                while (true) {
                    char* buf = readBuffer();

                    if (strchr(buf, '#')) {
                        break;
                    }

                    if (buf[0] == '1') {    // xóa từ hiện tại
                        char* last = findLastPos(buffer);
                        if (last > buffer) {
                            *(last - 1) = '\0';
                        } else {    // nếu chỉ có một từ
                            *last = '\0';
                        }
                    } else {
                        strcat(buffer, buf);

                        char* last_pos = findLastPos(buffer);

                        char last_word[100];
                        strcpy(last_word, last_pos);

                        char** res = NULL;
                        int res_count = 0;

                        chooseMostFrequentWords(root, last_word, suggest_count,true, true, &res, &res_count);

                        printf("0. %s\n", last_word);
                        for (int i = 0; i < res_count; i++) {
                            printf("%d. %s\n", i + 1, res[i]);
                        }

                        printf("Choose: ");
                        int nums = getInteger();


                        // thêm tính năng tự học như AI
                        // từ nào phổ biến thì gợi ý nhiều hơn
                        // từ nào không có thì tự thêm vào
                        if (res_count) {

                            if (nums > 0) { // nếu chọn thằng đã có
                                // copy thẳng vào buffer
                                strcpy(last_pos, res[nums - 1]);


                                TrieNode* chosen_node = searchWord(root, res[nums - 1]);
                                // update
                                (chosen_node->frequency)++;

                            } else {    // chọn 0 tức không chọn các gợi ý, từ đó có thể có hoặc chưa

                                TrieNode* typed_node = searchWord(root, last_word);

                                if (typed_node == NULL) {
                                    addWord(root, last_word);

                                } else {
                                    typed_node->frequency++;
                                }
                            }
                        } else {    // chắc chắn phải thêm
                            addWord(root, last_word);
                            TrieNode* cur = searchNode(root, last_word, 0);
                        }

                    }


                    printf("%s", buffer);

                    free(buf);
                }

                break;
            }   // ứng dụng autocomplete
            case 6: {
                while (true) {
                    char buf[1024];
                    fgets(buf, 1024, stdin);
                    buf[strcspn(buf, "\r\n")] = '\0';
                    if (buf[0] == '#') {
                        break;
                    }

                    if (searchWord(root, buf) != NULL) {
                        puts("Find");
                    }
                }
                break;
            }   // tìm từ
            case 7: {
                while (true) {
                    puts("Nhập từ muốn xóa:");
                    char del_word[1024];
                    fgets(del_word, 1024, stdin);
                    del_word[strcspn(del_word, "\r\n")] = '\0';

                    if (del_word[0] == '#') {
                        break;
                    }

                    bool success = deleteWord(&root, del_word, 0);
                    if (success) {
                        puts("Xóa từ thành công!");
                    } else {
                        puts("Từ này hiện không có trong từ điển");
                    }
                }
                break;
            }   // xóa từ
            case 8: {
                // MÃ HÓA DÙNG STACK VỚI CẶP NGOẶC LỒNG (STACK + ())
                // trong đó phần tử x nằm trong ngoặc của y tức là con, cháu của y
                // VD: (A0(A*1(A*1(A*1(A0(A*1()))...

                // CÁCH KHÁC LÀ DÙNG QUEUE LƯU THEO MỨC (\n)
                // lưu các con vào hàng đợi
                // VD: A0\nA*1\nB*1C0\n tức ông là A, cha là A, con là B, C
                // => cần thêm kí tự tách frequency với digit của thằng sau

                FILE* save_ptr = fopen(save_name, "w");
                assert(save_ptr);

                saveInfo(save_ptr);

                if (DFS) {
                    saveTrieWithDFS(root, save_ptr);
                } else {
                    saveTrieWithBFS(root, save_ptr);
                }

                fclose(save_ptr);

                freeTrieWithBFS(root);

                break;
            }   // mã hóa cây để tái sử dụng

            default:
                more = false;
                break;
        }
    }

    fclose(out_ptr);

    free(add_index);
    free(add_digit);

    freeQueueNode();

    freeTrieWithBFS(root);
    root = NULL;

    free(queue);

    return 0;
}