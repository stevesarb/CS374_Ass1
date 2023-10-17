#include "prototypes.h"

const int DEBUG_MODE = 0;

int main(int argc, char* argv[]) {

    if (argc < 2) {
        printf("You must provide the name of the file to process\n");
        return EXIT_FAILURE;
    }

    // process input file
    struct Movie* movie_head = process_file(argv[1]);

    if (movie_head == NULL) 
        return EXIT_FAILURE;
    

    int user_choice = 0;

    // operate searching functionality
    do {
        user_choice = get_choice();

        switch (user_choice) {
            case 1:
                display_movies_by_year(movie_head);
                break;
            case 2:
                display_top_rated_by_year(movie_head);
                break;
            case 3:
                display_movies_by_language(movie_head);
                break;
            case 4:
                break;
        }
    }
    while (user_choice != 4);
    
    // debugging functionality
    if (DEBUG_MODE == 1)
        print_list(movie_head);

    // free movie list
    free_movie_list(movie_head);
    
    return 0;
}