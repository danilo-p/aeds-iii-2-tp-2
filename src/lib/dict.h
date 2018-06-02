#ifndef DICT_H
#define DICT_H

typedef struct dict dict_t;

dict_t *dict_create(int max_size);

void dict_add_word(dict_t *dict, char *word, int distance);

void dict_order_by_distance(dict_t *dict);

void dict_list(dict_t *dict);

void dict_destroy(dict_t *dict);

#endif
