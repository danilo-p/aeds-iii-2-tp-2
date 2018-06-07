#ifndef AUTO_SUGGEST_H
#define AUTO_SUGGEST_H

typedef struct auto_suggest auto_suggest_t;

auto_suggest_t *auto_suggest_create(
    char *reference,
    int dict_max_size,
    int max_distance
);

int auto_suggest_add_suggestion(auto_suggest_t *auto_suggest, char *suggestion);

void auto_suggest_list(auto_suggest_t *auto_suggest);

void auto_suggest_destroy(auto_suggest_t *auto_suggest);

#endif
