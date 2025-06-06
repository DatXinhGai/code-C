//
// Created by HP LAPTOP on 04/04/2025.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STRING_LENGTH 16

typedef struct PACKAGE {
    char* package_id;
    int package_weight;
    struct PACKAGE* next;
} PACKAGE;

typedef struct {
    int max_weight;
    int min_weight;
    PACKAGE* head;
    PACKAGE* tail;
    int package_count;
} OFFICE;

typedef struct {
    char* town_name;
    OFFICE* office;
    int office_count;
    int package_count;
} TOWN;

PACKAGE* create_package() {
    PACKAGE* node = (PACKAGE*)malloc(sizeof(PACKAGE));
    node->package_id = (char*)malloc(MAX_STRING_LENGTH);
    return node;
}



void print_all_package(TOWN t) {
    printf("%s:\n", t.town_name);
    for (int k = 0; k < t.office_count; k++) {
        printf("\t%d:\n", k);

        PACKAGE* current = t.office[k].head;

        while (current != NULL) {
            printf("\t\t%s\n", current->package_id);

            current = current->next;
        }
    }
}

void send_all_acceptable_package(TOWN* source_town,  int source_office_id, TOWN* target_town, int target_office_id) {
    OFFICE* target_office = &(target_town->office[target_office_id]);
    PACKAGE* source_package_head = source_town->office[source_office_id].head;

    int min_weight = target_office->min_weight;
    int max_weight = target_office->max_weight;

    PACKAGE* source_package_prev = NULL;

    while (source_package_head != NULL) {
        if (source_package_head->package_weight >= min_weight && source_package_head->package_weight <= max_weight) {
            if (target_office->head) {
                target_office->tail->next = source_package_head;
                target_office->tail = target_office->tail->next;
            } else {
                target_office->head = source_package_head;
                target_office->tail = target_office->head;
            }

            if (source_package_prev != NULL) {
                source_package_prev->next = source_package_head->next;
            }

            target_office->package_count++;
            target_town->package_count++;

            source_town->office[source_office_id].package_count--;
            source_town->package_count--;

        } else {
            if (source_package_prev == NULL) {

                source_town->office[source_office_id].head = source_package_head;
            }
            source_package_prev = source_package_head;
        }

        source_package_head = source_package_head->next;
    }

    // NẾU TARGET KHÔNG CÓ PACKAGE NÀO VÀ KHÔNG CÓ CÁI NÀO CỦA SOURCE ĐƯỢC THÌ SAO???
    if (target_office->tail) {
        target_office->tail->next = NULL;
    }

    // NẾU TẤT CẢ TỪ SOURCE CHUYỂN HẾT SANG THÌ SAO???
    if (source_package_prev == NULL) {
        source_town->office[source_office_id].head = NULL;
        source_town->office[source_office_id].tail = NULL;
    }
}

void print_town_with_most_packages(TOWN t) {
    printf("Town with the most number of packages is %s\n", t.town_name);
}

void free_package(PACKAGE* head) {
    while (head) {
        PACKAGE* temp = head;
        head = (head)->next;
        free(temp->package_id);
        free(temp);
    }
}

int postTransitionToiUu() {
    int town_count;
    scanf("%d", &town_count);
    TOWN* town = (TOWN*)malloc(town_count*sizeof(TOWN));

    for (int i = 0; i < town_count; i++) {
        town[i].town_name = (char*)malloc(MAX_STRING_LENGTH);
        scanf("%s", town[i].town_name);

        scanf("%d", &town[i].office_count);
        town[i].office = (OFFICE*)malloc(town[i].office_count*sizeof(OFFICE));
        town[i].package_count = 0;

        for (int j = 0; j < town[i].office_count; j++) {

            scanf("%d%d%d", &town[i].office[j].package_count, &town[i].office[j].min_weight, &town[i].office[j].max_weight);
            town[i].package_count += town[i].office[j].package_count;

            if (town[i].office[j].package_count == 0) {
                town[i].office[j].head = NULL;
                town[i].office[j].tail = NULL;
            } else {

                town[i].office[j].head = create_package();
                PACKAGE* current = town[i].office[j].head;
                scanf("%s", current->package_id);
                scanf("%d", &current->package_weight);

                for (int k = 1; k < town[i].office[j].package_count; k++) {
                    PACKAGE* new_node = create_package();
                    scanf("%s", new_node->package_id);
                    scanf("%d", &new_node->package_weight);
                    current->next = new_node;
                    current = current->next;
                }

                town[i].office[j].tail = current;
                current->next = NULL;
            }
        }
    }


    int queries_count;
    scanf("%d", &queries_count);

    for (int i = 0; i < queries_count; i++) {
        int query_type;
        scanf("%d", &query_type);

        switch (query_type) {
            case 1 : {
                char* town_name = (char*)malloc(MAX_STRING_LENGTH);
                scanf("%s", town_name);

                for (int j = 0; j < town_count; j++) {
                    if (strcmp(town[j].town_name, town_name) == 0) {
                        print_all_package(town[j]);
                        break;
                    }
                }

                free(town_name);
                break;
            }

            case 2 : {

                char* source_name = (char*)malloc(MAX_STRING_LENGTH);
                char* target_name = (char*)malloc(MAX_STRING_LENGTH);
                int source_office_id, target_office_id;

                scanf("%s", source_name);
                scanf("%d", &source_office_id);
                scanf("%s", target_name);
                scanf("%d", &target_office_id);

                int source_id = -1;
                int target_id = -1;
                for (int j = 0; j < town_count; j++) {
                    if (strcmp(source_name, town[j].town_name) == 0) {
                        source_id = j;
                    }
                    if (strcmp(target_name, town[j].town_name) == 0) {
                        target_id = j;
                    }
                }

                send_all_acceptable_package(&town[source_id], source_office_id, &town[target_id], target_office_id);

                free(source_name);
                free(target_name);
                break;
            }

            case 3 : {
                int max_package = -1;
                int town_idx = -1;
                for (int j = 0; j < town_count; j++) {
                    if (town[j].package_count > max_package) {
                        max_package = town[j].package_count;
                        town_idx = j;
                    }
                }

                print_town_with_most_packages(town[town_idx]);
                break;
            }
        }

    }


    for (int i = 0; i < town_count; i++) {
        for (int j = 0; j < town[i].office_count; j++) {
            free_package(town[i].office[j].head);
        }
        free(town[i].office);
    }
    free(town);

    return 0;
}