#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lib/auto_suggest.h"

/**
 * @brief Reads the standard input until an '\n'
 * 
 * @return char* A pointer for the read content
 */
char *read_until_endl() {
    int length = 0;
    char c = getchar();
    char *content = NULL;

    while (c != '\n') {
        length += 1;
        content = (char *) realloc(content, length * sizeof(char));
        content[length - 1] = c;
        c = getchar();
    }

    length += 1;
    content = (char *) realloc(content, length * sizeof(char));
    content[length - 1] = '\0';

    return content;
}

/**
 * @brief The main function
 * 
 * Let n the dictionary size:
 * Program time complexity: O(n*log(n))
 * Program space complexity: O(n*log(n))
 * 
 * @return int The program status code
 */
int main() {
    int dict_size, distance_limit, i;
    scanf("%d\n", &dict_size);
    scanf("%d\n", &distance_limit);

    char *aux = read_until_endl();
    auto_suggest_t *auto_suggest = auto_suggest_create(
        aux,
        dict_size,
        distance_limit
    );

    for (i = 0; i < dict_size; i += 1) {
        aux = read_until_endl();
        if (!auto_suggest_add_suggestion(auto_suggest, aux)) {
            free(aux);
        }
    }

    auto_suggest_list(auto_suggest);

    auto_suggest_destroy(auto_suggest);

    return 0;
}