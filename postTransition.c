//
// Created by HP LAPTOP on 05/02/2025.
//
/*
2
A
2
2 1 3
a 2
b 3
1 2 4
c 2
B
1
4 1 4
d 1
e 2
f 3
h 4
5
3
2 B 0 A 1
3
1 A
1 B
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_STRING_LENGTH 32

typedef struct {
    char id[MAX_STRING_LENGTH];
    int weight;
} package;

typedef struct {
    package** packages;
    int packages_count;
    int min_weight;
    int max_weight;
} office;

typedef struct {
    char name[MAX_STRING_LENGTH];
    office* offices;
    int offices_count;
} town;

void print_all_packages(town t) {
    printf("%s:\n", t.name);
    for (int i = 0; i < t.offices_count; i++) {
        printf("\t%d:\n", i);
        for (int j = 0; j < t.offices[i].packages_count; j++) {
            printf("\t\t%s\n", t.offices[i].packages[j]->id);
        }
    }
}

// tạo mảng để lưu những thằng còn lại
void send_all_acceptable_packages(town* source, int source_index, town* target, int target_index) {
    office* target_office = &target->offices[target_index];
    office* source_office = &source->offices[source_index];

    int* t_count = &target_office->packages_count;
    int* s_count = &source_office->packages_count;

    int min = target_office->min_weight;
    int max = target_office->max_weight;

    int accepted_pkgs = 0;
    for (int i = 0; i < source_office->packages_count; i++) {
        int weight = source_office->packages[i]->weight;
        if (weight >= min && weight <= max) {
            accepted_pkgs++;
        }
    }
    // realloc cho target
    // xem xem target có đơn hàng nào không


    /*
    if (!target_office->packages) {
        target_office->packages = (package**)malloc((*t_count + accepted_pkgs)*sizeof(package*));
    } else {
        target_office->packages = (package**)realloc(target_office->packages, (*t_count + accepted_pkgs)*sizeof(package*));
    }
    */

    target_office->packages = (package**)realloc(target_office->packages,
    (*t_count + accepted_pkgs) * sizeof(package*));

    // realloc NULL tương đương với malloc


    // tạo mảng phụ lưu source
    if (*s_count == accepted_pkgs) { // tất cả đủ đk
        for (int i = 0; i < accepted_pkgs; i++) {
            target_office->packages[(*t_count)++] = source_office->packages[i];
        }
        free(source_office->packages);
        source_office->packages = NULL;
        *s_count = 0;
    } else {    // có cái không đủ đk
        // package 2 sao là chuẩn vì khi đó các remain trỏ vào các vùng nhớ của source
        // hình dung việc con trỏ trỏ
        package** remaining = (package**)malloc((*s_count - accepted_pkgs)*sizeof(package*));
        int remaining_count = 0;

        for (int i = 0; i < *s_count; i++) {
            int weight = source_office->packages[i]->weight;
            if (weight >= min && weight <= max) {
                target_office->packages[(*t_count)++] = source_office->packages[i];
            } else {
                remaining[remaining_count] = source_office->packages[i];
                remaining_count++;
            }
        }

        free(source_office->packages);
        source_office->packages = NULL;
        source_office->packages_count = remaining_count;
        source_office->packages = remaining;
    }

}

town town_with_most_packages(town* towns, int towns_count) {
    int max = 0;
    int chiSo = -1;
    for (int i = 0; i < towns_count; i++) {
        int tong = 0;
        for (int j = 0; j < towns[i].offices_count; j++) {
            tong += towns[i].offices[j].packages_count;
        }
        if (tong > max) {
            max = tong;
            chiSo = i;
        }
    }
    return towns[chiSo];
}

town* find_town(town* towns, int towns_count, char name[]) {
    for (int i = 0; i < towns_count; i++) {
        if (strcmp(name, towns[i].name) == 0) {
            return &towns[i];
        }
    }
    return NULL;
}

int postTransition() {
    int towns_count;
    scanf("%d", &towns_count);
    town* towns = malloc(sizeof(town) * towns_count);

    for (int i = 0; i < towns_count; i++) {
        scanf("%s", towns[i].name);
        scanf("%d", &towns[i].offices_count);
        towns[i].offices = malloc(sizeof(office) * towns[i].offices_count);

        for (int j = 0; j < towns[i].offices_count; j++) {
            int pkg_count;
            scanf("%d%d%d", &pkg_count, &towns[i].offices[j].min_weight, &towns[i].offices[j].max_weight);

            towns[i].offices[j].packages_count = pkg_count;
            towns[i].offices[j].packages = malloc(sizeof(package*) * pkg_count);

            for (int k = 0; k < pkg_count; k++) {
                // pck trỏ đến 1 package, nên phải cấp phát cho package này
                // không cấp phát thì không trỏ đến đâu cả

                package pkg;
                scanf("%s %d", pkg.id, &pkg.weight);
                towns[i].offices[j].packages[k] = &pkg;
                // dùng vầy cho ngắn
                // nếu dùng package* pkg thì phải cấp phát rồi gán nó là NULL, tí lại phải free towns[i].offices[j].packages[k]
                // biến pkg chỉ tồn tại tạm thời trong hàm(stack hết hàm thì free)
                // còn cách bên dưới cấp phát trong heap thì sống mãi, cần lưu ý khi dùng hàm khác
                /*
                package* pkg = malloc(sizeof(package));
                scanf("%s %d", pkg->id, &pkg->weight);
                towns[i].offices[j].packages[k] = pkg;
                pkg = NULL;
                */


                /*
                scanf("%s %d",
                    towns[i].offices[j].packages[k]->id,
                    &towns[i].offices[j].packages[k]->weight);
                    */
                // hai con trỏ cùng trỏ đến 1 bộ nhớ thì cùng quản lí
                // free con nào cũng sẽ giải phóng bộ nhớ được cấp phát
                // tốt nhất khi không dùng nữa thì gán NULL
            }
        }
    }

    int queries;
    scanf("%d", &queries);
    char town_name[MAX_STRING_LENGTH];

    while (queries--) {
        int type;
        scanf("%d", &type);

        if (type == 1) {
            scanf("%s", town_name);
            town* t = find_town(towns, towns_count, town_name);
            print_all_packages(*t);
        } else if (type == 2) {
            scanf("%s", town_name);
            town* source = find_town(towns, towns_count, town_name);
            int source_index;
            scanf("%d", &source_index);

            scanf("%s", town_name);
            town* target = find_town(towns, towns_count, town_name);
            int target_index;
            scanf("%d", &target_index);

            send_all_acceptable_packages(source, source_index, target, target_index);
        } else if (type == 3) {
            printf("Town with the most number of packages is %s\n", town_with_most_packages(towns, towns_count).name);
        }
    }


    for (int i = 0; i < towns_count; i++) {
        for (int j = 0; j < towns[i].offices_count; j++) {
            for (int k = 0; k < towns[i].offices[j].packages_count; k++) {
                //free(towns[i].offices[j].packages[k]);
                towns[i].offices[j].packages[k] = NULL;
            }
            free(towns[i].offices[j].packages);
            towns[i].offices[j].packages = NULL;
        }
        free(towns[i].offices);
        towns[i].offices = NULL;
    }
    free(towns);
    towns = NULL;

    return 0;
}