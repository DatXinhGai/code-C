//
// Created by HP LAPTOP on 09/06/2025.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_LENGTH 1024

enum STATUS {
    HAS_OUTER_TEXT = 0,
    HAS_CHILD = 1,
    INVALID = -1,
};

typedef enum STATUS STATUS;

const char DOUBLE_QUOTES = 34;

int getInt() {
    char buf[MAX_LENGTH];

    fgets(buf, MAX_LENGTH, stdin);

    int num = strtol(buf, NULL, 10);

    return num;
}

char* readBuf() {
    int data_len = 0;
    int alloc_len = MAX_LENGTH;
    char* data = (char*)malloc(alloc_len);

    while (true) {
        char* cur_sor = data + data_len;

        char* line = fgets(cur_sor, alloc_len - data_len, stdin);
        if (!line) { break; }

        data_len += strlen(line);

        if (data_len < alloc_len || data[data_len - 1] == '\0') { break; }

        alloc_len <<= 1;
        char* temp = (char*)realloc(data, alloc_len);

        if (!temp) { break; }

        data = temp;
    }

    if (data[data_len - 1] == '\n' || data[data_len - 1] == '\r') {
        data[--data_len] = '\0';
    } else {
        data[data_len] = '\0';
    }

    data = (char*)realloc(data, data_len + 1);

    return data;
}


typedef struct Attributes {
    char* name;
    char* value;
    struct Attributes* next;
} Attributes;

typedef struct treeNode {
    char* tag_name;
    Attributes* attributes;
    char* text;
    struct treeNode* first_child;
    struct treeNode* next_sibling;
    struct treeNode* parent;
} treeNode;




Attributes* createAttributes(char key[], char value[]) {
    Attributes* new_att = (Attributes*)malloc(sizeof(Attributes));
    new_att->name = (char*)malloc(strlen(key) + 1);
    new_att->value = (char*)malloc(strlen(value) + 1);

    strcpy(new_att->name, key);
    strcpy(new_att->value, value);

    new_att->next = NULL;

    return new_att;
}

void addAttributes(treeNode* cur_node, char key[], char value[]) {
    Attributes* new_att = createAttributes(key, value);

    Attributes* cur_att = cur_node->attributes;

    if (!cur_att) {
        cur_node->attributes = new_att;
    } else {
        while (cur_att->next != NULL) {
            cur_att = cur_att->next;
        }

        cur_att->next = new_att;
    }
}

void freeAttributes(Attributes* attributes) {
    if (attributes == NULL) {
        return ;
    }

    freeAttributes(attributes->next);

    free(attributes->name);
    free(attributes->value);
    free(attributes);
}

void printAttributes(Attributes* attributes) {
    if (attributes == NULL) { return; }

    printf("\t%s : %s\n", attributes->name, attributes->value);

    printAttributes(attributes->next);
}

void printTagInfo(treeNode* root) {
    printf("Tag name: %s\n", root->tag_name);

    printAttributes(root->attributes);

    if (root->text) { printf("\tText: %s", root->text); }

    puts("");
}

void printTag(treeNode* root) {
    if (root == NULL) { return; }

    treeNode* child = root->first_child;
    while (child) {
        printTag(child);
        child = child->next_sibling;
    }

    printTagInfo(root);
}

treeNode* createNode() {
    treeNode* new_node = (treeNode*)malloc(sizeof(treeNode));

    new_node->attributes = NULL;
    new_node->first_child = new_node->next_sibling = new_node->parent = NULL;
    new_node->tag_name = new_node->text = NULL;

    return new_node;
}

void addChild(treeNode* parent, treeNode* child) {
    child->parent = parent;

    if (!parent->first_child) {
        parent->first_child = child;
    } else {
        treeNode* cur_child = parent->first_child;
        while (cur_child->next_sibling != NULL) {
            cur_child = cur_child->next_sibling;
        }

        cur_child->next_sibling = child;
    }
}

void deleteNode(treeNode* del) {
    treeNode* parent = del->parent;

    if (parent->first_child == del) {
        parent->first_child = del->next_sibling;
    } else {
        treeNode* cur_child = parent->first_child;
        while (cur_child->next_sibling != del) {
            cur_child = cur_child->next_sibling;
        }

        cur_child->next_sibling = del->next_sibling;
    }
}


void preOrderTraversal(treeNode* root) {
    if (root == NULL) { return; }
    puts(root->tag_name);

    treeNode* child = root->first_child;
    while (child) {
        preOrderTraversal(child);
        child = child->next_sibling;
    }
}

// dùng để xóa tag luôn cũng tiện
void freeTree(treeNode** root) {
    if (!(*root)) { return; }




    treeNode** child = &((*root)->first_child);
    while (*child) {
        treeNode** prev_child = child;
        child = &((*child)->next_sibling);
        freeTree(prev_child);
    }

    /*
    treeNode* child = (*root)->first_child;
    while (child) {
        treeNode* next = child->next_sibling;
        freeTree(&child);
        child = next;
    }
    */

    free((*root)->tag_name);
    free((*root)->text);

    freeAttributes((*root)->attributes);

    free(*root);
    *root = NULL;
}



void printInvalid(const char component[]) {
    fprintf(stderr, "Invalid %s!", component);
    exit(EXIT_FAILURE);
}

void checkAttributeValue(char* value) {
    // value phải ở trong dấu double quotes
    if (value[0] != DOUBLE_QUOTES || value[strlen(value) - 1] != DOUBLE_QUOTES) {
        printInvalid("Value of Attributes");
    }
}

void removeCommentsAndVersions(FILE* xml_ptr) {
    int ch;

    while (true) {
        ch = fgetc(xml_ptr);

        if (ch == '<') {
            ch = fgetc(xml_ptr);
            if (ch != '?') {    // không phải version thì quay trở về vị trí ban đầu
                fseek(xml_ptr, -2, SEEK_CUR);
            } else {
                while (ch != EOF) {
                    ch = fgetc(xml_ptr);
                    if (ch == '?' && (ch = fgetc(xml_ptr)) == '>') {
                        break;
                    }
                    if (ch == EOF) { printInvalid("Comments and Versions"); }
                }
            }

            break;

        } else {
            char comment[MAX_LENGTH];
            fgets(comment, MAX_LENGTH, xml_ptr);
        }
    }
}

void getSpace(FILE* xml_ptr) {
    int ch;
    while (true) {
        ch = fgetc(xml_ptr);
        if (ch == ' ' || ch == '\t'|| ch == '\n' || ch == '\r') {
            continue;
        } else {
            ungetc(ch, xml_ptr);
            return ;
        }
    }
}

char* getTagName(FILE* xml_ptr) {
    char* tag_name = (char*)malloc(MAX_LENGTH);
    int index = 0;

    int ch;

    // không có dấu cách ở đầu
    ch = fgetc(xml_ptr);

    if (ch == ' ' || ch == EOF || ch == '\t' || ch == '\n') {
        printInvalid("Tag Name");
    }
    ungetc(ch, xml_ptr);

    while (true) {
        ch = fgetc(xml_ptr);
        if (ch == '<' || ch == EOF || ch == '\t' || ch == '\n') {
            // 2 dấu mở liên tiếp
            printInvalid("Tag Name");
        } else if (ch == '>') {     // không có attributes
            ungetc(ch, xml_ptr);
            tag_name[index] = '\0';
            break;
        } else if (ch == ' ') {     // có attributes
            // check thử đằng sau

            // lấy hết dấu cách trống
            while (ch == ' ') {
                ch = fgetc(xml_ptr);
                if (ch == '\n' || ch == EOF || ch == '\t') {
                    printInvalid("Tag name");
                }
            }

            if (ch == '>') {    // không cách ở cuối
                printInvalid("Tag Name");
            }
            ungetc(ch, xml_ptr);

            tag_name[index] = '\0';
            break;
        } else {    // kí tự chấp nhận trong tên tag
            tag_name[index++] = ch;
        }
    }

    if (tag_name[0] == '\0') { printInvalid("Tag Name"); }

    tag_name = (char*)realloc(tag_name, strlen(tag_name) + 1);


    return tag_name;
}



void getAttributes(treeNode* cur_node, FILE* xml_ptr) {

    int ch;

    char key[MAX_LENGTH] = "\0";
    char value[MAX_LENGTH] = "\0";
    bool is_key = true;
    int key_index = 0;
    int value_index = 0;

    while (true) {
        ch = fgetc(xml_ptr);

        if (ch == EOF || ch == '<' || ch == '\t' || ch == '\n' || ch == '\r') {
            // không được kết thúc file và không chứa mở lần 2
            printInvalid("Attributes");
        } else if (ch == '>') {     // kết thúc
            if (value[0] == '\0' && key[0] != '\0') {     // nếu chưa có giá trị thì fail
                printInvalid("Attributes");
            } else {    // hợp lệ
                value[value_index] = '\0';

                // nếu tồn tại key và value
                if (*key && *value) {
                    checkAttributeValue(value);
                    addAttributes(cur_node, key, value);
                }

            }

            break;
        } else if (ch == ' ') { // hết 1 attributes
            // kiểm tra xem có kết thúc bởi dấu cách

            while (ch == ' ') {
                ch = fgetc(xml_ptr);
                if (ch == '\t' || ch == '\n' || ch == EOF) { printInvalid("Attributes"); }
            }


            if (ch == '>') {
                printInvalid("Attributes");
            }
            ungetc(ch, xml_ptr);

            if (value[0] == '\0') { // hết attributes mà không có giá trị
                printInvalid("Attributes");
            } else {    // hợp lệ, thêm vào danh sách attributes hiện có
                value[value_index] = '\0';
                // check xem value có ở trong nháy kép
                checkAttributeValue(value);

                addAttributes(cur_node, key, value);
            }

            // set lại giá trị để lấy tiếp attributes
            key[0] = '\0';
            value[0] = '\0';
            key_index = value_index = 0;
        } else if (ch == '=') {     // kết thúc key
            if (key[0] == '\0') {   // chưa có key
                printInvalid("Attributes");
            } else {
                is_key = false;
            }
        } else {    // kí tự thông thường
            if (is_key == true) {
                key[key_index++] = ch;
                key[key_index] = '\0';
            } else {
                value[value_index++] = ch;
                value[value_index] = '\0';
            }
        }
    }
}

STATUS getOuter(treeNode* cur_node, FILE* xml_ptr) {

    int ch;
    bool has_first_space = false;   // đánh dấu nếu outer bắt đầu bằng cách trống
    bool text_start = false;
    char text[1024] = "\0";
    int index = 0;

    while (true) {
        ch = fgetc(xml_ptr);
        if (ch == EOF || ch == '>') {
            printInvalid("Outer Text");
        } // không được đóng tag
        else if ((ch == ' ' || ch == '\t' || ch == '\n')) {
            if (text_start == false) {
                has_first_space = true;
            } else {    // nếu bắt đầu rồi thì check xem ở cuối có toàn trống không
                // lấy vị trí hiện tại để backtrack sau khi kiểm tra
                int prev_pos = ftell(xml_ptr);

                while (true) {
                    ch = fgetc(xml_ptr);
                    if (ch == ' ' || ch == '\t' || ch == '\n' || ch == '\r') { continue; }

                    if (ch == '<') { printInvalid("Outer"); }
                    else {
                        int cur_pos = ftell(xml_ptr);
                        // đưa con trỏ file về lại vị trí ban đầu
                        fseek(xml_ptr, prev_pos - cur_pos, SEEK_CUR);
                        break;
                    }
                }
            }
        } else if (ch == '<') {
            ch = fgetc(xml_ptr);
            if (ch == '/') {    // gặp đóng tag tức thằng này không có con, chứa outer text
                // trả lại các kí tự của close tag
                ungetc(ch, xml_ptr);
                ungetc('<', xml_ptr);

                text[index] = '\0';

                cur_node->text = (char*)malloc(strlen(text) + 1);
                strcpy(cur_node->text, text);

                return HAS_OUTER_TEXT;
            } else {    // gặp mở tag, vậy là có thẻ con
                ungetc(ch, xml_ptr);
                ungetc('<', xml_ptr);

                if (text_start == true) { printInvalid("Outer Text"); } // thẻ cha không có text

                return HAS_CHILD;
            }
        } else {    // gặp text

            if (has_first_space == true && text_start == false) {
                printInvalid("Outer Text");
            } // từ không bắt đầu
            else {
                text_start = true;
                text[index++] = ch;
            }
        }
    }
}

void checkEndTag(char* tag_name, FILE* xml_ptr) {
    char end_tag[MAX_LENGTH];
    end_tag[0] = '<';
    end_tag[1] = '/';
    end_tag[2] = '\0';
    strcat(end_tag, tag_name);
    strcat(end_tag, ">");

    char buf[MAX_LENGTH];
    int index = 0;

    int ch;

    while (true) {
        ch = getc(xml_ptr);

        if (ch == EOF) { printInvalid("End Tag"); }

        if (ch == '>') {
            buf[index++] = ch;
            buf[index] = '\0';
            break;
        }
        buf[index++] = ch;

    }

    if (strcmp(end_tag, buf) != 0) { printInvalid("End Tag"); }
}

void checkEnd(FILE* xml_ptr) {
    int ch;
    while ((ch = fgetc(xml_ptr)) != EOF) {
        if (ch != ' ' && ch != '\t' && ch != '\n') {
            printInvalid("End, Redundant Digit");
        }
    }
}

// ý tưởng đệ quy với thứ tự là lấy mở tag, lấy hết con rồi mới đóng tag
// vậy tức con phải hoàn thành trước cha => đệ quy vào lấy con trước
treeNode* getXMLtoTree(FILE* xml_ptr) {

    getSpace(xml_ptr);

    int ch = fgetc(xml_ptr);
    if (ch != '<') {
        printInvalid("First Open Tag");
    } else if ((ch = fgetc(xml_ptr)) == '/') {  // dấu này là dấu kết thúc của cha, tức cha hết con
        // đẩy lại luồng trả cho cha để kiểm tra end_tag của cha
        ungetc('/', xml_ptr);
        ungetc('<', xml_ptr);
        return NULL;    // đánh dấu việc hết con
    }

    ungetc(ch, xml_ptr);

    treeNode* cur_node = createNode();

    cur_node->tag_name = getTagName(xml_ptr);

    getAttributes(cur_node, xml_ptr);
    // đến đây đã xong mở thẻ

    STATUS status = getOuter(cur_node, xml_ptr);

    if (status == HAS_OUTER_TEXT) {
           // đây là base case
    } else {    // có con
        treeNode* child;
        while ((child = getXMLtoTree(xml_ptr)) != NULL) {
            addChild(cur_node, child);
        }
    }

    checkEndTag(cur_node->tag_name, xml_ptr);
    return cur_node;
}

bool findAttribute(Attributes* cur_att, char* att_key, char* att_value) {
    if (cur_att == NULL) { return false; }
    if ((!cur_att->name || strcmp(cur_att->name, att_key) == 0) &&
        (!cur_att->value || strcmp(cur_att->value, att_value) == 0)) {
        return true;
    }

    return findAttribute(cur_att->next, att_key, att_value);
}

void changeAttribute(Attributes* cur_att, char* key, char* new_value) {
    if (cur_att == NULL) {
        puts("Không tìm được attribute");
        return;
    }

    if (strcmp(cur_att->name, key) == 0) {
        free(cur_att->value);
        cur_att->value = strdup(new_value);
    }
}

treeNode* findTag(treeNode* root, char* tag_name, char* att_key, char* att_value) {
    if (root == NULL) { return NULL; }

    if (strcmp(root->tag_name, tag_name) == 0) {
        if (findAttribute(root->attributes, att_key, att_value)) {
            return root;
        }
    }

    treeNode* child = root->first_child;

    while (child) {
        treeNode* child_res = findTag(child, tag_name, att_key, att_value);
        if (child_res != NULL) {
            return child_res;
        }

        child = child->next_sibling;
    }

    return NULL;
}

treeNode* chooseNode(treeNode* root) {
    puts("0.");
    printTagInfo(root);

    treeNode* child = root->first_child;
    int index = 1;

    while (child) {
        printf("%d.\n", index++);
        printTagInfo(child);
        child = child->next_sibling;
    }

    int res = getInt();
    if (res == 0) {
        return root;
    } else {
        treeNode* cur_child = root->first_child;
        while (--res) {
            cur_child = cur_child->next_sibling;
        }

        chooseNode(cur_child);
    }
}

void addTag(treeNode* root) {
    treeNode* new_tag = createNode();

    puts("Nhập tên thẻ:");
    char* tag_name = readBuf();
    new_tag->tag_name = tag_name;

    puts("Nhập thuộc tính(key, value)(# để kết thúc)");
    while (true) {
        char* key = readBuf();
        if (key[0] == '#' || key[0] == '\n') { break; }

        char* value = readBuf();
        checkAttributeValue(value);

        addAttributes(new_tag, key, value);

        free(key);
        free(value);
    }

    puts("Nhập outer text: ('#' tức là không có)");
    char* text = readBuf();
    if (text[0] != '#' && text[0] != '\n') {
        new_tag->text = text;
    } else {
        free(text);
    }

    treeNode* add_pos_node = chooseNode(root);
    addChild(add_pos_node, new_tag);
}

void deleteTag(treeNode** root) {

    treeNode* del_tag = chooseNode(*root);

    if (del_tag == *root) {
        freeTree(root);
    } else {
        deleteNode(del_tag);

        freeTree(&del_tag);
    }
}

void modifyAttribute(treeNode* root) {
    treeNode* modified = chooseNode(root);
    puts("Nhập tên thuộc tính: ");
    char* att_name = readBuf();

    puts("Nhập giá trị mới:");
    char* new_value = readBuf();

    changeAttribute(modified->attributes, att_name, new_value);

    free(att_name);
    free(new_value);
}

void printAllWithAttribute(treeNode* root, char* att_key) {
    if (root == NULL) { return; }
    Attributes* att = root->attributes;
    while (att) {
        if (strcmp(att->name, att_key) == 0) {
            puts(att->value);
        }
        att = att->next;
    }

    treeNode* child = root->first_child;
    while (child) {
        printAllWithAttribute(child, att_key);
        child = child->next_sibling;
    }
}

void saveToFile(treeNode* root, FILE* save_ptr, char tab_buf[], int tab_pos) {
    if (!root) { return; }

    char buffer[MAX_LENGTH];
    char end_buffer[MAX_LENGTH] = "</";

    // thêm cách vào trước buffer
    strcpy(buffer, tab_buf);
    strcat(buffer, "<");

    // thêm tag name
    strcat(buffer, root->tag_name);
    strcat(end_buffer, root->tag_name);

    // thêm thuộc tính nếu có
    Attributes* att = root->attributes;
    while (att) {
        strcat(buffer, " ");
        strcat(buffer, att->name);
        strcat(buffer, "=");
        strcat(buffer, att->value);

        att = att->next;
    }

    strcat(buffer, ">");
    strcat(end_buffer, ">\n");


    if (!root->first_child) {
        if (root->text) {
            strcat(buffer, root->text);
        }
        strcat(buffer, end_buffer);
        fputs(buffer, save_ptr);

    } else {
        strcat(buffer, "\n");
        fputs(buffer, save_ptr);

        // phải chèn tab_buf vào trước end_buffer
        char new_end[MAX_LENGTH];
        strcpy(new_end, tab_buf);
        strcat(new_end, end_buffer);

        treeNode* child = root->first_child;

        // tăng thêm 1 lần tab nếu có con
        tab_buf[tab_pos++] = '\t';
        tab_buf[tab_pos] = '\0';

        while (child) {
            saveToFile(child, save_ptr, tab_buf, tab_pos);
            child = child->next_sibling;
        }

        // trả lại 1 lần tag của con
        tab_buf[tab_pos - 1] = '\0';

        fputs(new_end, save_ptr);
    }




}


void printXMLSelection() {
    puts("1. Add XML from File");
    puts("2. Add tag");
    puts("3. Delete tag");
    puts("4. Change tag attribute");
    puts("5. Save to File");
    puts("6. Print All with Attribute");
    puts("7. Print Tag");
}





int XML() {

    treeNode* root = NULL;

    bool con = true;
    while (con) {
        printXMLSelection();
        int num = getInt();

        switch (num) {
            case 1: {
                //char xml_file[MAX_LENGTH] = "saveXML.txt";
                char xml_file[MAX_LENGTH] = "xml_sample.txt";
                //fgets(xml_file, 1024, stdin);
                //xml_file[strcspn(xml_file, "\r\n")] = '\0';

                FILE* xml_ptr = fopen(xml_file, "r");
                if (!xml_ptr) {
                    perror("Lỗi khi mở file!");
                    exit(EXIT_FAILURE);
                }
                rewind(xml_ptr);

                // xóa phần đầu
                removeCommentsAndVersions(xml_ptr);

                // add vào tree
                root = getXMLtoTree(xml_ptr);

                // xử lí nếu phần cuối có kí tự thừa
                checkEnd(xml_ptr);

                preOrderTraversal(root);

                fclose(xml_ptr);
                break;
            }
            case 2: {
                addTag(root);
                preOrderTraversal(root);
                break;
            }
            case 3: {
                deleteTag(&root);
                preOrderTraversal(root);
                break;
            }
            case 4: {
                modifyAttribute(root);
                break;
            }
            case 5: {
                char* save_name = "saveXML.txt";
                FILE* save_ptr = fopen(save_name, "w");
                if (!save_ptr) {
                    fprintf(stderr, "Cannot open save file!\n");
                    exit(EXIT_FAILURE);
                }

                char tab_buf[MAX_LENGTH] = "\0";
                saveToFile(root, save_ptr, tab_buf, 0);

                fclose(save_ptr);
                freeTree(&root);

                break;
            }
            case 6: {
                char* att_key = readBuf();

                printAllWithAttribute(root, att_key);
                free(att_key);

                break;
            }
            case 7: {
                treeNode* tag = chooseNode(root);
                printTag(tag);
                break;
            }
            default: {
                con = false;
                break;
            }
        }
    }

    if (root) {
       freeTree(&root);
    }
    return 0;
}
