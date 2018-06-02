#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int smallest(int a, int b, int c) {
    int s = a < b ? a : b;
    s = s < c ? s : c;
    return s;
}

char * read_until_endl() {
    int len = 0;
    char c = getchar();
    char *word = NULL;

    while(c != '\n') {
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

int minimum_edit_distance(char *a, char *b) {
    int i, j, **distances, len_a, len_b;

    len_a = strlen(a);
    len_b = strlen(b);

    distances = (int **) malloc(len_a * sizeof(int *));
    for (i = 0; i < len_a; i += 1) {
        distances[i] = (int *) malloc(len_b * sizeof(int));
    }

    for (i = 0; i < len_a; i += 1) {
        for (j = 0; j < len_b; j += 1) {
            if (i == 0) {
                distances[i][j] = j;
                continue;
            }

            if (j == 0) {
                distances[i][j] = i;
                continue;
            }

            int cost = 1;
            if (a[i] == b[j]) {
                cost = 0;
            }

            distances[i][j] = smallest(
                cost + distances[i - 1][j - 1], // Replace
                1 + distances[i][j-1], // Insert
                1 + distances[i - 1][j] // Delete
            );
        }
    }

    int minumum_distance = distances[len_a - 1][len_b - 1];
    for (i = 0; i < len_a; i += 1) {
        free(distances[i]);
    }
    free(distances);

    return minumum_distance;
}

typedef struct dict_word {
    char *word;
    int distance;
} dict_word_t;

dict_word_t *dict_word_create(char *word, int distance) {
    dict_word_t *dict_word = (dict_word_t *) malloc(sizeof(dict_word_t));
    dict_word->word = word;
    dict_word->distance = distance;
    return dict_word;
}

void dict_word_destroy(dict_word_t *dict_word) {
    free(dict_word->word);
    free(dict_word);
}

typedef struct dict {
    dict_word_t **words;
    int max_size;
    int curr_size;
} dict_t;

dict_t *dict_create(int max_size) {
    dict_t *dict = (dict_t *) malloc(sizeof(dict_t));
    dict->words = (dict_word_t **) malloc(max_size * sizeof(dict_word_t *));
    dict->max_size = max_size;
    dict->curr_size = 0;
    return dict;
}

void dict_add_word(dict_t *dict, char *word, int distance) {
    dict_word_t *dict_word = dict_word_create(word, distance);
    dict->words[dict->curr_size] = dict_word;
    dict->curr_size += 1;
}

int compare_dict_words_distance(const void *a, const void *b) {
    return (
        (*((dict_word_t **) a))->distance >
        (*((dict_word_t **) b))->distance
    );
}

void dict_order_by_distance(dict_t *dict) {
    qsort(
        dict->words,
        dict->curr_size,
        sizeof(dict_word_t *),
        compare_dict_words_distance
    );
}

void dict_list_until_distance(dict_t *dict, int max_distance) {
    int i;
    dict_order_by_distance(dict);
    dict_word_t *aux;
    for (i = 0; i < dict->curr_size; i += 1) {
        aux = dict->words[i];
        if (aux->distance > max_distance) {
            break;
        }

        printf("%s\n", aux->word);
    }
}

void dict_destroy(dict_t *dict) {
    int i;
    for (i = 0; i < dict->curr_size; i += 1) {
        dict_word_destroy(dict->words[i]);
    }
    free(dict->words);
    free(dict);
}

int main() {
    int dict_size, distance_limit, i;
    scanf("%d\n", &dict_size);
    scanf("%d\n", &distance_limit);

    char *word = read_until_endl();
    char *aux_word = NULL;
    int aux_distance = 0;

    dict_t *dict = dict_create(dict_size);
    for (i = 0; i < dict_size; i += 1) {
        aux_word = read_until_endl();
        aux_distance = minimum_edit_distance(word, aux_word);
        dict_add_word(dict, aux_word, aux_distance);
    }

    dict_list_until_distance(dict, distance_limit);

    dict_destroy(dict);
    free(word);

    return 0;
}