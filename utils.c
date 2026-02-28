#include <stdio.h>
#include "utils.h"

void print_array(char **arr, int count, const char *label) {
    for (int i = 0; i < count; i++) {
        printf("%s[%d]: %s\n", label, i, arr[i]);
    }
}