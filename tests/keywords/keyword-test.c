#include <stdio.h>
#include <stdlib.h>
#include "../../utils/hashmap.h"
#include "../../clox/scanner.h"

int main(void) {
    map_t map = create_keywords_hashmap();

    for (int i = 0; i < HASHMAP_SIZE; i++) {
        any_t any_type = NULL;
        any_t * p_any_type = &any_type;
        int code = hashmap_get(map, keys_str[i], p_any_type);
        if (code == MAP_FULL) {
            fprintf(stderr, "ERROR in scanner.c - identifier(): hashmap_get returned code %i. Hashmap is full.\n", code);
            exit(1);
        }
        if (code == MAP_OMEM) {
            fprintf(stderr, "ERROR in scanner.c - identifier(): hashmap_get returned code %i. Out of memory.\n", code);
            exit(1);
        }

        if (*(int*)any_type == keys[i]) {
            printf("'%s' OK\n", keys_str[i]);
        } else {
            printf("'%s' FAIL\n", keys_str[i]);
        }
    }

    return 0;
}
