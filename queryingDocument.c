//
// Created by HP LAPTOP on 03/02/2025.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


/*
Sample Input 0

2
Learning C is fun.
Learning pointers is more fun.It is good to have pointers.
3
1 2
2
5
6
2 1 1
4
3 1 1 1
Sample Output 0

Learning pointers is more fun.It is good to have pointers.
Learning C is fun
Learning
*/
// trên Hackerrank có bài thêm struct với cái đếm, nhưng không có max;




char* get_input(void) {
    char temp[10];
    fgets(temp, 9, stdin);
    int para_count = strtol(temp, NULL, 10);

    int text_length = 0;
    int text_max = 1024;
    char* text = (char*)malloc(text_max);
    text[0] = '\0';
    // TRƯỚC KHI DÙNG STRCAT BẮT BUỘC PHẢI KHỞI TẠO ĐỂ BIẾT ĐƯỜNG MÀ GÁN, TRÁNH GIÁ TRỊ RÁC

    for (int i = 0; i < para_count; i++) {
        int para_length = 0;
        int para_max = 1024;
        char* para = (char*)malloc(para_max);

        while (true) {
            char* cursor = para + para_length;
            char* line = fgets(cursor, para_max - para_length, stdin);
            if (!line) { break; }

            para_length += strlen(line);
            if (para_length < para_max || para[para_length - 1] == '\n') { break; }

            para_max <<= 1;
            char* tam = (char*)realloc(para, para_max);
            if (!tam) { break; }
            para = tam;
            tam = NULL;
        }

        if (para[para_length - 1] == '\n') {
            para_length--;
        }

        para[para_length] = '\0';
        text_length += para_length;

        bool re = false;
        while (text_length + 1 >= text_max) {
            text_max <<= 1;
            re = true;
        }
        if (re) {
            text = (char*)realloc(text, text_max);
        }
        if (!text) {
            fprintf(stderr, "Reallocated unsuccessfully");
            exit(EXIT_FAILURE);
        }

        strcat(text, para);
        if (i != para_count - 1) {
            strcat(text, "\n");
        }

        free(para);
        para = NULL;
    }

    return text;
}


char**** get_document(char* text) {
    int doc_size = 256;
    int para_size = 256;
    int sen_size = 512;
    // coi như là mảng các con trỏ trỏ thẳng vào text
    char**** doc = (char****)malloc(doc_size*sizeof(char***));
    for (int i = 0; i < doc_size; i++) {
        doc[i] = (char***)malloc(para_size*sizeof(char**));
        for (int j = 0; j < para_size; j++) {
            doc[i][j] = (char**)malloc(sen_size*sizeof(char*));
        }
    }
    int cs_para = 0;
    int cs_sen = 0;
    int cs_word = 0;
    char* n_ptr = text;
    char* end_ptr = text;
    while (*n_ptr != '\0') {
        // chạy hết text thì thôi
        bool kt_tu = false;
        bool kt_cau = false;
        bool kt_doan = false;
        if (!(*n_ptr ^ ' ') || !(*n_ptr ^ '.')) {
            // đi thêm được 1 từ
            kt_tu = true;
            if (*n_ptr == '.') {
                kt_cau = true;
            }

            *n_ptr = '\0';
            doc[cs_para][cs_sen][cs_word] = end_ptr;    // gán con trỏ vào đầu từ
            cs_word++;
            if (cs_word >= sen_size) {
                doc[cs_para][cs_sen] = (char**)realloc(doc[cs_para][cs_sen], (cs_word + 1)*sizeof(char*));
            }
            // vượt quá sen_size thì realloc thủ công


            do {
                n_ptr++;
            } while (*n_ptr == ' ');
            end_ptr = n_ptr;
            // bỏ qua các dấu ' ' thừa và gán end_ptr vào đầu từ tiếp theo
        } else if (!(*n_ptr ^ '\n')) {  // chơi XOR cho máu
            // kết thúc đoạn thì không thêm từ nào và cho end_ptr trỏ sang từ tiếp
            kt_doan = true;
            end_ptr = n_ptr + 1;
        }

        if (kt_cau) {
            doc[cs_para][cs_sen] = (char**)realloc(doc[cs_para][cs_sen], cs_word*sizeof(char*));
            cs_word = 0;
            cs_sen++;
            if (cs_sen >= para_size) {
                doc[cs_para] = (char***)realloc(doc[cs_para], (cs_sen + 1)*sizeof(char**));
                doc[cs_para][cs_sen + 1] = (char**)malloc(sen_size*sizeof(char*));
                // nhớ phải cấp phát cho thằng mới
            }
        } else if (kt_doan) {
            doc[cs_para] = (char***)realloc(doc[cs_para], cs_sen*sizeof(char**));
            cs_word = 0;
            cs_sen = 0;
            cs_para++;
            if (cs_para >= doc_size) {
                doc = (char****)realloc(doc, (cs_para + 1)*sizeof(char***));
                doc[cs_para + 1] = (char***)malloc(para_size*sizeof(char**));
                for (int i = 0; i < para_size; i++) {
                    doc[cs_para + 1][i] = (char**)malloc(sen_size*sizeof(char**));
                }
                // nhớ phải cấp phát như mới đầu
            }
        }
        // gặp 1 kí tự thường hoặc không kết thúc từ thì tăng n_ptr
        // kết thúc từ thì tăng trước rồi
        if (!kt_tu) {
            n_ptr++;
        }
    }
    // còn đoạn cuối chưa được thêm vì không có \n
    doc[cs_para] = (char***)realloc(doc[cs_para], cs_sen*sizeof(char**));
    cs_para++;
    doc = (char****)realloc(doc, cs_para*sizeof(char***));
    text = NULL;
    return doc;
}


char* kth_word_in_mth_sentence_of_nth_paragraph(char**** document, int k, int m, int n) {
    return document[n - 1][m - 1][k - 1];
}

char** kth_sentence_in_mth_paragraph(char**** document, int k, int m) {
    return document[m - 1][k - 1];
}

char*** kth_paragraph(char**** document, int k) {
    return document[k - 1];
}

void print_word(char* word) {
    printf("%s", word);
}

void print_sentence(char** sentence) {
    int word_count;
    scanf("%d", &word_count);
    for (register int i = 0; i < word_count; i++) {
        printf("%s", *(sentence + i));
        if (i != word_count - 1) {
            printf(" ");
        }
    }
}

void print_paragraph(char*** paragraph) {
    int sentence_count;
    scanf("%d", &sentence_count);
    for (int i = 0; i < sentence_count; i++) {
        print_sentence(*(paragraph + i));
        printf(".");
    }
}

int queryingDocument() {
    char* text = get_input();
    char**** document = get_document(text);
    int query;
    scanf("%d", &query);

    while (query--) {
        int type;
        scanf("%d", &type);

        if (type == 3) {
            int k, m, n;
            scanf("%d%d%d", &k, &m, &n);
            char* word = kth_word_in_mth_sentence_of_nth_paragraph(document, k, m, n);
            print_word(word);
            word = NULL;
        } else if (type == 2) {
            int k, m;
            scanf("%d%d", &k, &m);
            char** sentence = kth_sentence_in_mth_paragraph(document, k, m);
            print_sentence(sentence);
            sentence = NULL;
        } else if (type == 1) {
            int k;
            scanf("%d", &k);
            char*** paragraph = kth_paragraph(document, k);
            print_paragraph(paragraph);
            paragraph = NULL;
        }
        puts("");
    }


    free(document);
    document = NULL;
    return 0;
}