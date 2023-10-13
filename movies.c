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
    float rating;
    struct movie* next;
};

// function declarations
struct movie* process_file(char*);
struct movie* create_movie(char*);
struct language* process_languages(char*);
struct language* create_language(char*);

int main(int argc, char* argv[]) {

    if (argc < 2) {
        printf("You must provide the name of the file to process\n");
        return EXIT_FAILURE;
    }

    struct movie* list = process_file(argv[1]);


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
    int ctr = 0;

    // head and tail of linked list
    struct movie* head = NULL;
    struct movie* tail = NULL;


    // read file line by line
    while ((n_read = getline(&curr_line, &len, movies_file)) != -1) {

        // if this is the first line of the file, skip the rest of the loop
        if (ctr == 0) {
            ctr++;
            continue;
        }

        // get a new movie node corresponding to the current line
        struct movie* new_node = create_movie(curr_line);

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

    new_node->lang = lang;
    new_node->next = NULL;

    return new_node;
}