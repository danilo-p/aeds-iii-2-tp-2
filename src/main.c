#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

int smallest(int a, int b, int c) {
    int s = a < b ? a : b;
    s = s < c ? s : c;
    return s;
}

int minimum_edit_distance(char *a, char *b) {
    int i, j;

    int len_a = strlen(a), len_b = strlen(b);

    int **distances = (int **) malloc((len_a + 1) * sizeof(int *));
    for (i = 0; i <= len_a; i += 1) {
        distances[i] = (int *) malloc((len_b + 1) * sizeof(int));
    }

    for (i = 0; i <= len_a; i += 1) {
        for (j = 0; j <= len_b; j += 1) {
            if (i == 0) {
                distances[i][j] = j;
            } else if (j == 0) {
                distances[i][j] = i;
            } else if (a[i - 1] == b[j - 1]) {
                distances[i][j] = distances[i - 1][j - 1];
            } else {
                distances[i][j] = 1 + smallest(
                    distances[i][j-1],
                    distances[i - 1][j],
                    distances[i - 1][j - 1]
                );
            }
        }
    }

    int minumum_distance = distances[len_a][len_b];
    for (i = 0; i <= len_a; i += 1) {
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
    dict_word_t *casted_a = (*((dict_word_t **) a));
    dict_word_t *casted_b = (*((dict_word_t **) b));

    int distance_a = casted_a->distance;
    int distance_b = casted_b->distance;

    if (distance_a == distance_b) {
        return strcmp(casted_a->word, casted_b->word);
    }

    return distance_a > distance_b;
}

void dict_order_by_distance(dict_t *dict) {
    qsort(
        dict->words,
        dict->curr_size,
        sizeof(dict_word_t *),
        compare_dict_words_distance
    );
}

void dict_list(dict_t *dict) {
    int i;
    dict_word_t *aux;
    for (i = 0; i < dict->curr_size; i += 1) {
        aux = dict->words[i];
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