#include "prototypes.h"

void prompt_search() {
    printf("\n1. Show movies released in the specified year\n");
    printf("2. Show highest rated movie for each year\n");
    printf("3. Show the title and year of release of all movies in a specific language\n");
    printf("4. Exit the program\n");
}

int get_choice() {
    char* line = NULL;
    size_t len = 0;
    ssize_t line_size = 0;

    int choice = 0;

    while (choice == 0) {
        prompt_search();
        printf("\nEnter a choice from 1-4: ");
        line_size = getline(&line, &len, stdin);
        choice = check_choice(line);

        if (choice == 0) {
            printf("You entered an incorrect choice. Try again.\n");
        }
    }
    
    free(line);
    return choice;
}

int check_choice(char* line) {
    if (strlen(line) > 2)
        return 0;

    int choice = atoi(line);
    if ((choice < 1) || (choice > 4)) 
        return 0;
    
    return choice;
}