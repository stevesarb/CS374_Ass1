#include "prototypes.h"

void display_top_rated_by_year(struct Movie* mov_head) {
    // initialize linked list to use to keep track of pointers that correspond to the top rated movie of each year
    struct Movies_By_Distinct_Year* head = malloc(sizeof(struct Movies_By_Distinct_Year));
    struct Movies_By_Distinct_Year* curr_distinct = head;
    head->mov = mov_head;
    head->next = NULL;
    struct Movie* curr = mov_head->next;

    while (curr != NULL) {
        // check if curr (current movie in movie list) has the same year a movie pointed to by any node in years linked list
        check_for_same_year(curr, head);
        curr = curr->next;
    }

    // print Movies by distinct year list
    while (curr_distinct != NULL) {
        printf("%d %0.1f %s\n", curr_distinct->mov->year, curr_distinct->mov->rating, curr_distinct->mov->title);
        curr_distinct = curr_distinct->next;
    }

    // free movies by distinct year list
    free_distinct_year_list(head);
}

void check_for_same_year(struct Movie* curr, struct Movies_By_Distinct_Year* node) {
    int has_year = 0;
    struct Movies_By_Distinct_Year* last = node;

    // iterate through the whole list of movies by distict year
    while (node != NULL) {
        // if there is already a movie of the same year in the list
        if (curr->year == node->mov->year) {
            has_year = 1;
            // compare their ratings
            if (curr->rating > node->mov->rating) {
                node->mov = curr;
            }
        }
        // advance pointers
        if (node->next != NULL)
            last = node->next;
        node = node->next;
    }

    if (has_year == 0) {
        // add new node to list and have it point to the current movie
        last->next = malloc(sizeof(struct Movies_By_Distinct_Year));
        last = last->next;
        last->mov = curr;
        last->next = NULL;
    }
}