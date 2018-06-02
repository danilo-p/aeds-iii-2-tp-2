#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lib/dict.h"

char *read_until_endl() {
    int len = 0;
    char c = getchar();
    char *word = NULL;

    while (c != '\n') {
        len += 1;
        word = (char *) realloc(word, len * sizeof(char));
        word[len - 1] = c;
        c = getchar();
    }

    len += 1;
    word = (char *) realloc(word, len * sizeof(char));
    word[len - 1] = '\0';

    return word;
}

int smallest(int a, int b, int c) {
    int s = a < b ? a : b;
    s = s < c ? s : c;
    return s;
}

int minimum_edit_distance(char *source, char *dest) {
    int i, j;

    int len_source = strlen(source);
    int len_dest = strlen(dest);

    int **distances = (int **) malloc((len_source + 1) * sizeof(int *));
    for (i = 0; i <= len_source; i += 1) {
        distances[i] = (int *) malloc((len_dest + 1) * sizeof(int));
    }

    for (i = 0; i <= len_source; i += 1) {
        for (j = 0; j <= len_dest; j += 1) {
            if (i == 0) {
                distances[i][j] = j;
            }
            else if (j == 0) {
                distances[i][j] = i;
            }
            else if (source[i - 1] == dest[j - 1]) {
                distances[i][j] = distances[i - 1][j - 1];
            }
            else {
                distances[i][j] = 1 + smallest(
                                          distances[i][j - 1],
                                          distances[i - 1][j],
                                          distances[i - 1][j - 1]);
            }
        }
    }

    int minumum_distance = distances[len_source][len_dest];
    for (i = 0; i <= len_source; i += 1) {
        free(distances[i]);
    }
    free(distances);

    return minumum_distance;
}

int main() {
    int dict_size, distance_limit, i;
    scanf("%d\n", &dict_size);
    scanf("%d\n", &distance_limit);

    char *word = read_until_endl();
    dict_t *dict = dict_create(dict_size);

    char *aux_word;
    int aux_distance;
    for (i = 0; i < dict_size; i += 1) {
        aux_word = read_until_endl();
        aux_distance = minimum_edit_distance(word, aux_word);
        if (aux_distance <= distance_limit) {
            dict_add_word(dict, aux_word, aux_distance);
        }
    }

    dict_order_by_distance(dict);
    dict_list(dict);

    dict_destroy(dict);
    free(word);

    return 0;
}