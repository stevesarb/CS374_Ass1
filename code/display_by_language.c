#include "prototypes.h"

void display_movies_by_language(struct Movie* head) {
    char* lang = get_lang();

    int ctr = 0;
    search_by_language(head, lang, &ctr);
    if (ctr == 0) 
        printf("No data about movies released in %s\n", lang);

    free(lang);
}

char* get_lang() {
    char* line = NULL;
    size_t len = 0;
    ssize_t line_size = 0;

    printf("Enter the language for which you want to see movies: ");
    line_size = getline(&line, &len, stdin);

    char* lang = calloc(strlen(line), sizeof(char));
    strncpy(lang, line, line_size - 1); // eliminates the newline character that's stored in the input string

    free(line);

    return lang;
}

// recursive function that searches through linked list of movies
void search_by_language(struct Movie* node, char* lang, int* ctr) {
    if (node != NULL) {
        int has_lang = search_languages(node->langs_head, lang);
        if (has_lang != 0) {
            printf("%d %s\n", node->year, node->title);
            (*ctr)++;
        }
        search_by_language(node->next, lang, ctr); // recursive call
    }
}

// recursive function that searches through linked list of languages for each movie
int search_languages(struct Language* node, char* lang) {
    int has_lang = 0;
    if (node != NULL) {
        if (strcmp(node->lang, lang) == 0) {
            return 1;
        }
        has_lang = search_languages(node->next, lang); // recursive call
    }
    return has_lang;
}