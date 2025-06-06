//
// Created by HP LAPTOP on 04/02/2025.
//
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
/*
2
Learning C is fun.
Learning pointers is more fun.It is good to have pointers.
3
1 2
2 1 1
3 1 1 1
*/

// giữ text lại cuối cùng để free

struct word {
char* data;
};

struct sentence {
struct word* data;
int word_count;//denotes number of words in a sentence
};

struct paragraph {
struct sentence* data  ;
int sentence_count;//denotes number of sentences in a paragraph
};

struct document {
struct paragraph* data;
int paragraph_count;//denotes number of paragraphs in a document
};

char* get_input() {
    char tam[10];
    fgets(tam, 9, stdin);
    int para_count = strtol(tam, NULL, 10);
    int input_len = 0;
    int input_max = 1024;
    char* text = (char*)malloc(input_max);
    text[0] = '\0';

    for (int i = 0; i < para_count; i++) {
        int data_len = 0;
        int alloc_len = 1024;
        char* data = (char*)malloc(alloc_len);

        while (true) {
            char* cursor = data + data_len;
            char* line = fgets(cursor, alloc_len - data_len, stdin);
            if (!line) {
                break;
            }

            data_len += strlen(line);
            if (data_len < alloc_len || data[data_len - 1] == '\n') {
                break;
            }

            alloc_len <<= 1;
            data = (char*)realloc(data, alloc_len);

        }
        if (data[data_len - 1] == '\n') {
            data_len--;
        }

        data[data_len] = '\0';

        input_len += data_len;
        while (input_len + 1 >= input_max) {
            input_max <<= 1;
            text = (char*)realloc(text, input_max);
        }

        strcat(text, data);
        free(data);
        data = NULL;
        if (i != para_count - 1) {
            strcat(text, "\n");
            input_len++;
        }
    }

    text = (char*)realloc(text, input_len + 1);
    return text;
}

struct document get_document(char* text) {
    int doc_size = 32;
    int para_size = 128;
    int sen_size = 256;

    // khoi tao doc
    struct document doc;
    doc.data = ( struct paragraph*)malloc(doc_size*sizeof(struct paragraph));
    doc.paragraph_count = 0;
    for (int i = 0; i < doc_size; i++) {
        doc.data[i].sentence_count = 0;
        doc.data[i].data = (struct sentence*)malloc(para_size*sizeof(struct sentence));
        for (int j = 0; j < para_size; j++) {
            doc.data[i].data[j].word_count = 0;
            doc.data[i].data[j].data = (struct word*)malloc(sen_size*sizeof(struct word));
        }
    }


    char* n_ptr = text;
    char* end_ptr = text;

    while (*n_ptr != '\0') {
        int* cs_para = &doc.paragraph_count;
        int* cs_sen = &doc.data[*cs_para].sentence_count;
        int* cs_word = &doc.data[*cs_para].data[*cs_sen].word_count;
        bool word_end = false;
        // ket thuc tu hay cau thi deu them 1 tu
        if (*n_ptr == ' ' || *n_ptr == '.') {
            bool sen_end = false;
            word_end = true;
            if (*n_ptr == '.') {
                sen_end = true;
            }
            // ket thuc tu
            *n_ptr = '\0';
            // tro vao dau tu
            doc.data[*cs_para].data[*cs_sen].data[*cs_word].data = end_ptr;
            (*cs_word)++;
            // qua co thi realloc
            if (*cs_word >= sen_size) {
                doc.data[*cs_para].data[*cs_sen].data = (struct word*)realloc(doc.data[*cs_para].data[*cs_sen].data,
                    (*cs_word + 1) *sizeof(struct word));
            }
            if (sen_end) {
                doc.data[*cs_para].data[*cs_sen].data = (struct word*)realloc(doc.data[*cs_para].data[*cs_sen].data,
                    *cs_word*sizeof(struct word));
                (*cs_sen)++;
                // con cs_para va cs_sen co hieu luc
                if (*cs_sen >= para_size) {
                    doc.data[*cs_para].data = (struct sentence*)realloc(doc.data[*cs_para].data,
                        (*cs_sen + 1)*sizeof(struct sentence));
                    doc.data[*cs_para].data[*cs_sen].word_count = 0;
                    doc.data[*cs_para].data[*cs_sen].data = (struct word*)malloc(sen_size*sizeof(struct word));
                    // cau moi thi word = 0;
                }
            }
            // bo ' ' thua
            do {
                n_ptr++;
            } while (*n_ptr == ' ');
            end_ptr = n_ptr;
        } else if (*n_ptr == '\n') {
            // ket thuc doan
            doc.data[*cs_para].data = (struct sentence*)realloc(doc.data[*cs_para].data,
                *cs_sen*sizeof(struct sentence));

            end_ptr = n_ptr + 1; // neu khong co thi *end_ptr la '\0'
            (*cs_para)++;
            // chi cs_para con hieu luc
            if (*cs_para >= doc_size) {
                doc.data = (struct paragraph*)realloc(doc.data, (*cs_para + 1)*sizeof(struct paragraph));
                doc.data[*cs_para].data = (struct sentence*)malloc(para_size*sizeof(struct sentence));
                doc.data[*cs_para].sentence_count = 0;
                for (int i = 0; i < para_size; i++) {
                    doc.data[*cs_para].data[i].data = (struct word*)malloc(sen_size*sizeof(struct word));
                    doc.data[*cs_para].data[i].word_count = 0;
                }
            }
        }

        if (!word_end) {
            n_ptr++;
        }
        cs_para = NULL;
        cs_sen = NULL;
        cs_word = NULL;
    }

    // con doan cuoi vi khong co \n
    doc.data[doc.paragraph_count].data = (struct sentence*)realloc(doc.data[doc.paragraph_count].data,
        doc.data[doc.paragraph_count].sentence_count*sizeof(struct sentence));

    doc.paragraph_count++;
    doc.data = (struct paragraph*)realloc(doc.data, doc.paragraph_count*sizeof(struct paragraph));

    return doc;
}

struct word kth_word_in_mth_sentence_of_nth_paragraph(struct document Doc, int k, int m, int n) {
    return Doc.data[n - 1].data[m - 1].data[k - 1];
}

struct sentence kth_sentence_in_mth_paragraph(struct document Doc, int k, int m) {
    return Doc.data[m - 1].data[k - 1];
}

struct paragraph kth_paragraph(struct document Doc, int k) {
    return Doc.data[k - 1];
}

void print_word(struct word word) {
    printf("%s", word.data);
}

void print_sentence(struct sentence sentence) {
    for (int i = 0; i < sentence.word_count; i++) {
        printf("%s", sentence.data[i].data);
        if (i != sentence.word_count - 1) {
            printf(" ");
        }
    }
}

void print_paragraph(struct paragraph paragraph) {
    for (int i = 0; i < paragraph.sentence_count; i++) {
        print_sentence(paragraph.data[i]);
        printf(".");
    }
}

int queryingDocument2() {
    char* text = get_input();
    struct document Doc = get_document(text);

    int query;
    scanf("%d", &query);
    while (query--) {
        int type;
        scanf("%d", &type);
        if (type == 3) {
            int k, m, n;
            scanf("%d%d%d", &k, &m, &n);
            struct word word = kth_word_in_mth_sentence_of_nth_paragraph(Doc, k, m, n);
            print_word(word);
        } else if (type == 2) {
            int k, m;
            scanf("%d%d", &k, &m);
            struct sentence sentence = kth_sentence_in_mth_paragraph(Doc, k, m);
            print_sentence(sentence);
        } else {
            int k;
            scanf("%d", &k);
            struct paragraph paragraph = kth_paragraph(Doc, k);
            print_paragraph(paragraph);
        }
        puts("\n");
    }
    for (int i = 0; i < Doc.paragraph_count; i++) {
        for (int j = 0; j < Doc.data[i].sentence_count; j++) {
            for (int k = 0; k < Doc.data[i].data[j].word_count; k++) {
                Doc.data[i].data[j].data[k].data = NULL;
            }
            free(Doc.data[i].data[j].data);
            Doc.data[i].data[j].data = NULL;
        }
        free(Doc.data[i].data);
        Doc.data[i].data = NULL;
    }
    free(Doc.data);
    Doc.data = NULL;

    free(text);
    text = NULL;
    return 0;
}