#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "auto_suggest.h"

/**
 * @brief Take the smallest integer specified
 * 
 * Time complexity: O(1)
 * 
 * @param a Any integer
 * @param b Any integer
 * @param c Any integer
 * @return int The smallest integer.
 */
int smallest(int a, int b, int c) {
    int s = a < b ? a : b;
    s = s < c ? s : c;
    return s;
}

/**
 * @brief Calculates the minimum edit distance for source to become dest
 * 
 * Let m the source string length and n the destination string length:
 * Time complexity: O(m*n).
 * Space complexity: O(m*n).
 * 
 * @param source The source word
 * @param dest The destination word
 * @return int The minimum edit distance (aka. Levenshtein distance)
 */
int levenshtein_distance(char *source, char *dest) {
    int i, j;

    int source_length = strlen(source);
    int dest_length = strlen(dest);

    int **distances = (int **) malloc((source_length + 1) * sizeof(int *));
    for (i = 0; i <= source_length; i += 1) {
        distances[i] = (int *) malloc((dest_length + 1) * sizeof(int));
    }

    for (i = 0; i <= source_length; i += 1) {
        for (j = 0; j <= dest_length; j += 1) {
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

    int minumum_distance = distances[source_length][dest_length];
    for (i = 0; i <= source_length; i += 1) {
        free(distances[i]);
    }
    free(distances);

    return minumum_distance;
}

/**
 * @brief Defines the structure of a suggestion
 * 
 * It is composed by the word and the levenshtein distance of the word related
 * to the auto_suggest reference.
 */
typedef struct suggestion {
    char *word;
    int distance;
} suggestion_t;

/**
 * @brief Creates a new suggestion
 * 
 * Time complexity: O(1)
 * Space complexity: O(1)
 * 
 * @param word 
 * @param distance 
 * @return suggestion_t* 
 */
suggestion_t *suggestion_create(char *word, int distance) {
    suggestion_t *suggestion = (suggestion_t *) malloc(sizeof(suggestion_t));
    suggestion->word = word;
    suggestion->distance = distance;
    return suggestion;
}

/**
 * @brief Deallocate the memory of the given suggestion
 * 
 * Time complexity: O(1)
 * 
 * @param suggestion The suggestion to be destroyed
 */
void suggestion_destroy(suggestion_t *suggestion) {
    free(suggestion->word);
    free(suggestion);
}

/**
 * @brief Defines the auto suggestion structure.
 * 
 * This is an abstraction that holds the reference word and the dictionary.
 * Intends to be used to list suggestions based on a maximum levenshtein
 * distance.
 */
struct auto_suggest {
    char *reference;
    int max_distance;
    suggestion_t **dict;
    int dict_max_size;
    int dict_current_size;
};

/**
 * @brief Creates a new auto_suggest
 * 
 * Time complexity: O(1)
 * Space complexity: O(1)
 * 
 * @param reference The word that will be the reference for calculating distances
 * @param dict_max_size The maximum size of the dictionary
 * @param max_distance The maximum levenshtein distance of the suggestions
 * @return auto_suggest_t* A pointer to the new auto_suggest
 */
auto_suggest_t *auto_suggest_create(
    char *reference,
    int dict_max_size,
    int max_distance
) {
    auto_suggest_t *auto_suggest = (auto_suggest_t *) malloc(sizeof(auto_suggest_t));
    auto_suggest->reference = reference;
    auto_suggest->max_distance = max_distance;
    auto_suggest->dict = (suggestion_t **) malloc(dict_max_size * sizeof(suggestion_t *));
    auto_suggest->dict_max_size = dict_max_size;
    auto_suggest->dict_current_size = 0;
    return auto_suggest;
}

/**
 * @brief Adds a new suggestion to the auto_suggest.
 * 
 * Let m the reference word length and n the new suggestion length:
 * Time complexity: O(n*m)
 * Space complexity: O(1)
 * 
 * @param auto_suggest 
 * @param word 
 * @return int True if the suggestion was added. False instead.
 */
int auto_suggest_add_suggestion(auto_suggest_t *auto_suggest, char *word) {
    if (auto_suggest->dict_current_size + 1 > auto_suggest->dict_max_size) {
        return 0;
    }

    if (
        abs(strlen(word) - strlen(auto_suggest->reference)) >
        auto_suggest->max_distance
    ) {
        return 0;
    }

    int distance = levenshtein_distance(word, auto_suggest->reference);
    if (distance > auto_suggest->max_distance) {
        return 0;
    }

    suggestion_t *suggestion = suggestion_create(word, distance);
    auto_suggest->dict[auto_suggest->dict_current_size] = suggestion;
    auto_suggest->dict_current_size += 1;
    return 1;
}

/**
 * @brief Compares two suggestions
 * 
 * It is used to sort the dictionary using the distances. If two suggestions
 * have the same levenshtein distance, the alphabetical order is used instead.
 * 
 * Time complexity: O(1)
 * 
 * @param a A suggestion
 * @param b Another suggestion
 * @return int "1" if a is greater than b. 0 instead.
 */
int compare_suggestions(const void *a, const void *b) {
    suggestion_t *casted_a = (*((suggestion_t **) a));
    suggestion_t *casted_b = (*((suggestion_t **) b));

    int distance_a = casted_a->distance;
    int distance_b = casted_b->distance;

    if (distance_a == distance_b) {
        return strcmp(casted_a->word, casted_b->word);
    }

    return distance_a > distance_b;
}

/**
 * @brief Sorts the auto_suggest dictionary based on the distance.
 * 
 * Let n the auto_suggest dictionary size:
 * Time complexity: O(n*log(n))
 * 
 * @param auto_suggest The auto_suggest that holds the dictionary
 */
void auto_suggest_order_by_distance(auto_suggest_t *auto_suggest) {
    qsort(
        auto_suggest->dict,
        auto_suggest->dict_current_size,
        sizeof(suggestion_t *),
        compare_suggestions
    );
}

/**
 * @brief List the suggestions that are lower than the specified distance.
 * 
 * Let n the auto_suggest dictionary size:
 * Time complexity: O(n*log(n))
 * 
 * @param auto_suggest The auto_suggest that holds the dictionary
 */
void auto_suggest_list(auto_suggest_t *auto_suggest) {
    auto_suggest_order_by_distance(auto_suggest);
    int i;
    suggestion_t *aux;
    for (i = 0; i < auto_suggest->dict_current_size; i += 1) {
        aux = auto_suggest->dict[i];
        printf("%s\n", aux->word);
    }
}

/**
 * @brief Destroys the auto_suggest, deallocating all the memory
 * 
 * Time complexity: O(1)
 * 
 * @param auto_suggest The auto_suggest to be destroyed
 */
void auto_suggest_destroy(auto_suggest_t *auto_suggest) {
    int i;
    for (i = 0; i < auto_suggest->dict_current_size; i += 1) {
        suggestion_destroy(auto_suggest->dict[i]);
    }
    free(auto_suggest->reference);
    free(auto_suggest->dict);
    free(auto_suggest);
}