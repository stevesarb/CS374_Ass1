// #define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int DEBUG_MODE = 0;

struct Movies_By_Distinct_Year {
    struct Movie* mov;
    struct Movies_By_Distinct_Year* next;
};

struct Language {
    char* lang;
    struct Language* next;
};

struct Movie {
    char* title;
    int year;
    struct Language* langs_head;
    double rating;
    struct Movie* next;
};

// function declarations
struct Movie* process_file(char*);
struct Movie* create_movie(char*);
struct Language* process_languages(char*);
struct Language* create_language(char*);
void prompt_search();
int get_choice();
int check_choice(char*);
void display_movies_by_year(struct Movie*);
void search_by_year(struct Movie*, int, int*);
void display_top_rated_by_year(struct Movie*);
void check_for_same_year(struct Movie*, struct Movies_By_Distinct_Year*);
void display_movies_by_language(struct Movie*);
char* get_lang();
void search_by_language(struct Movie*, char*, int*);
int search_languages(struct Language*, char*);
void free_distinct_year_list(struct Movies_By_Distinct_Year*);
void free_movie_list(struct Movie*);

// debugging functions
void print_list(struct Movie*);
void print_movie(struct Movie*);
void print_languages(struct Language*);




int main(int argc, char* argv[]) {

    if (argc < 2) {
        printf("You must provide the name of the file to process\n");
        return EXIT_FAILURE;
    }

    // process input file
    struct Movie* movie_head = process_file(argv[1]);

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



//
// function definitions
//

struct Movie* process_file(char* file_path) {
    // open file for reading only
    FILE* movies_file = fopen(file_path, "r");

    // check that file opened properly
    if (movies_file == NULL) {
        printf("The provided file cannot be opened\n");
        return NULL;
    }

    char* curr_line = NULL;
    size_t len = 0;
    ssize_t n_read;
    int ctr = -1;

    // head and tail of linked list
    struct Movie* head = NULL;
    struct Movie* tail = NULL;

    struct Movie* new_node = NULL;


    // read file line by line
    while ((n_read = getline(&curr_line, &len, movies_file)) != -1) {

        // if this is the first line of the file, skip the rest of the loop
        if (ctr == -1) {
            ctr++;
            continue;
        }

        // get a new movie node corresponding to the current line
        new_node = create_movie(curr_line);
        ctr++;

        // if this is the first movie in the list
        if (head == NULL) {
            head = new_node;
            tail = new_node;
        }
        // if this is not the first movie in the list
        else {
            tail->next = new_node;
            tail = new_node;
        }
    }

    // output file processed message
    printf("\nProcessed file %s and parsed data for %d movies\n", file_path, ctr);

    // free dynamically allocated memory
    free(curr_line);

    // close file
    fclose(movies_file);

    return head;
}

struct Movie* create_movie(char* curr_line) {
    // dynamically allocate memory for a new movie struct
    struct Movie* curr_movie = malloc(sizeof(struct Movie));

    // for use with strtok_r
    char* save_ptr;

    // first token is the title
    char* token = strtok_r(curr_line, ",", &save_ptr);
    curr_movie->title = calloc(strlen(token) + 1, sizeof(char));
    strcpy(curr_movie->title, token);

    // next token is the year
    token = strtok_r(NULL, ",", &save_ptr);
    curr_movie->year = atoi(token);

    // next token is the languages
    token = strtok_r(NULL, ",", &save_ptr);
    curr_movie->langs_head = process_languages(token);

    // next token is the rating
    token = strtok_r(NULL, ",", &save_ptr);
    char* ptr;
    curr_movie->rating = strtod(token, &ptr);

    return curr_movie;
}

struct Language* process_languages(char* langs) {
    // temp variable for strtok_r
    char* temp_str;

    // variables for languages linked list
    struct Language* head = NULL;
    struct Language* tail = NULL;
    
    // get first language from string
    char* token = strtok_r(langs, "[;]", &temp_str);
    // add that language to languages linked list
    struct Language* new_node = create_language(token);
    head = new_node;
    tail = new_node;

    // continue through the rest of the language (if there are any)
    while (token != NULL) {
        token = strtok_r(NULL, "[;]", &temp_str);
        
        if (token != NULL) {
            new_node = create_language(token);
            tail->next = new_node;
            tail = new_node;
        }
    }

    return head;
}

struct Language* create_language(char* lang) {
    struct Language* new_node = malloc(sizeof(struct Language));

    // allocate new dynamic memory to store the string into, because the string that 
    // lang points to is going to be erased after the calling function ends
    new_node->lang = calloc(strlen(lang) + 1, sizeof(char));
    strcpy(new_node->lang, lang);

    new_node->next = NULL;

    return new_node;
}

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


void display_movies_by_language(struct Movie* head) {
    char* lang = get_lang();

    int ctr = 0;
    search_by_language(head, lang, &ctr);
    if (ctr == 0) 
        printf("No data about movies released in %s\n", lang);

    free(lang);
}

char* get_lang() {
    char* line;
    size_t len = 0;
    ssize_t line_size = 0;

    printf("Enter the language for which you want to see movies: ");
    line_size = getline(&line, &len, stdin);

    char* lang = calloc(strlen(line), sizeof(char));
    strncpy(lang, line, line_size - 1); // eliminates the newline character that's stored in the input string

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

void free_distinct_year_list(struct Movies_By_Distinct_Year* node) {
    if (node->next != NULL) {
        free_distinct_year_list(node->next);
    }
    free(node);
}

void free_movie_list(struct Movie* node) {
    if (node->next != NULL) {
        free_movie_list(node->next);
    }
    free(node);
}







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
