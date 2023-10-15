// #define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct language {
    char* lang;
    struct language* next;
};

struct movie {
    char* title;
    int year;
    struct language* langs_head;
    double rating;
    struct movie* next;
};

// function declarations
struct movie* process_file(char*);
struct movie* create_movie(char*);
struct language* process_languages(char*);
struct language* create_language(char*);
void prompt_search();
int get_choice();
int check_choice(char*);
void display_movies_by_year(struct movie*);
void search_by_year(struct movie*, int, int*);
void display_top_rated_by_year(struct movie*);

void display_movies_by_language(struct movie*);
char* get_lang();
void search_by_language(struct movie*, char*, int*);
int search_languages(struct language*, char*);

// potentially not useful functions
void print_list(struct movie*);
void print_movie(struct movie*);
void print_languages(struct language*);




int main(int argc, char* argv[]) {

    if (argc < 2) {
        printf("You must provide the name of the file to process\n");
        return EXIT_FAILURE;
    }

    // process input file
    struct movie* movie_head = process_file(argv[1]);

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
    


    print_list(movie_head);

    return 0;
}





// function definitions
struct movie* process_file(char* file_path) {
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
    struct movie* head = NULL;
    struct movie* tail = NULL;

    struct movie* new_node = NULL;


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

struct movie* create_movie(char* curr_line) {
    // dynamically allocate memory for a new movie struct
    struct movie* curr_movie = malloc(sizeof(struct movie));

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

struct language* process_languages(char* langs) {
    // temp variable for strtok_r
    char* temp_str;

    // variables for languages linked list
    struct language* head = NULL;
    struct language* tail = NULL;
    
    // get first language from string
    char* token = strtok_r(langs, "[;]", &temp_str);
    // add that language to languages linked list
    struct language* new_node = create_language(token);
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

struct language* create_language(char* lang) {
    struct language* new_node = malloc(sizeof(struct language));

    // allocate new dynamic memory to store the string into, because the string that 
    // lang points to is going to be erased after the calling function ends
    new_node->lang = calloc(strlen(lang) + 1, sizeof(char));
    strcpy(new_node->lang, lang);

    new_node->next = NULL;

    return new_node;
}





void print_list(struct movie* list) {
    while (list != NULL) {
        print_movie(list);
        list = list->next;
    }
}

void print_movie(struct movie* mov) {
    printf("Title: %s\nYear: %d\nLanguages:\n", mov->title, mov->year);
    print_languages(mov->langs_head);
    printf("Rating: %0.1f\n\n", mov->rating);
}

void print_languages(struct language* node) {
    if (node != NULL) {
        printf("    %s\n", node->lang);
        print_languages(node->next); // recursive call
    }
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

void display_movies_by_year(struct movie* list) {
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

    printf("line size: %d\n", line_size);
    
    int year = atoi(line);

    return year;
}

void search_by_year(struct movie* node, int year, int* ctr) {
    if (node != NULL) {
        if (node->year == year) {
            printf("%s\n", node->title);
            (*ctr)++;
        }
        search_by_year(node->next, year, ctr); // recursive call
    }
}

void display_top_rated_by_year(struct movie* head) {

}



void display_movies_by_language(struct movie* head) {
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

    // I think the language I enter has a newline character appended to the end of it, causing it to fail my strcmp() attempts
    // Solution: edit string (line) so that it doesn't have newline character at the end
    printf("line size: %d\n", line_size);

    char* lang = calloc(strlen(line) + 1, sizeof(char));
    strcpy(lang, line);

    return lang;
}

void search_by_language(struct movie* node, char* lang, int* ctr) {
    if (node != NULL) {
        int has_lang = search_languages(node->langs_head, lang);
        if (has_lang != 0) {
            printf("%s\n", node->title);
            (*ctr)++;
        }
        search_by_language(node->next, lang, ctr); // recursive call
    }
}

int search_languages(struct language* node, char* lang) {
    if (node != NULL) {
        if (strcmp(node->lang, lang) == 0) {
            return 1;
        }
        int has_lang = search_languages(node->next, lang); // recursive call
    }
    return 0;
}