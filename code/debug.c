#include "prototypes.h"

void print_list(struct Movie* list) {
    while (list != NULL) {
        print_movie(list);
        list = list->next;
    }
}

void print_movie(struct Movie* mov) {
    printf("Title: %s\nYear: %d\nLanguages:\n", mov->title, mov->year);
    print_languages(mov->langs_head);
    printf("Rating: %0.1f\n\n", mov->rating);
}

void print_languages(struct Language* node) {
    if (node != NULL) {
        printf("    %s\n", node->lang);
        print_languages(node->next); // recursive call
    }
}
