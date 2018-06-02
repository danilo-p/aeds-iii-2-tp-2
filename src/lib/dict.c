#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dict.h"

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

struct dict {
    dict_word_t **words;
    int max_size;
    int curr_size;
};

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