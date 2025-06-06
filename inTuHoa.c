//
// Created by HP LAPTOP on 09/02/2025.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>




int inTuHoa() {

    const char kitu[7] = " ,.!?:";

    char* text = (char*)malloc(1024);
    fgets(text, 1023, stdin);


    int tokens_index = 0;
    int max_tokens = 32;
    char** tokens = (char**)malloc(max_tokens*sizeof(char*));

    char* n_ptr = text;
    char* end_ptr = text;
    char* null_ptr = text;

    bool end = false;
    bool end_word = false;
    bool start_word = false;

    while (*n_ptr != '\0') {
        if (strchr(kitu, *n_ptr)) {	// gặp kí tự ngăn cách thì hết 1 từ
            if (start_word == true && end_word == false) {
                end_word = true;
                null_ptr = n_ptr;
            }
        } else {	// gặp kí tự khác
            if (*n_ptr >= 'A' && *n_ptr <= 'Z') {	// kí tự hoa
                if (end_word) {						// hết 1 từ và bắt đầu từ tiếp theo
                    end_word = false;
                }
                if (!start_word) {					// bắt đầu token mới
                    start_word = true;
                    end_ptr = n_ptr;
                }
            } else {								// gặp từ thường
                if (end_word) {						// có từ trước rồi thì hết
                    *null_ptr = '\0';
                    tokens[tokens_index] = end_ptr;
                    tokens_index++;
                    if (tokens_index >= max_tokens) {
                        max_tokens <<= 1;
                        tokens = (char**)malloc(max_tokens*sizeof(char*));
                    }
                }
            }
        }
        n_ptr++;
    }

    for (int i = 0; i < tokens_index; i++) {
        puts(tokens[i]);
    }

    return 0;
}