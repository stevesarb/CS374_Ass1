#include "prototypes.h"

void display_movies_by_year(struct Movie* list) {
    int year = get_year();

    // search through list to find movies of the specified year
    int ctr = 0;
    search_by_year(list, year, &ctr); // recursive function
    if (ctr == 0) 
        printf("No data about movies released in the year %d\n", year);
}

int get_year() {
    char* line = NULL;
    size_t len = 0;
    ssize_t line_size = 0;

    printf("Enter the year for which you want to see movies: ");
    line_size = getline(&line, &len, stdin);
    
    int year = atoi(line);

    free(line);

    return year;
}

void search_by_year(struct Movie* node, int year, int* ctr) {
    if (node != NULL) {
        if (node->year == year) {
            printf("%s\n", node->title);
            (*ctr)++;
        }
        search_by_year(node->next, year, ctr); // recursive call
    }
}