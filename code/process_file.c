#include "prototypes.h"

struct Movie* process_file(char* file_path) {
    // open file for reading only
    FILE* movies_file = fopen(file_path, "r");

    // check that file opened properly
    if (movies_file == NULL) {
        printf("The file %s cannot be opened\n", file_path);
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
    char* save_ptr = NULL;

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
    char* ptr = NULL;
    curr_movie->rating = strtod(token, &ptr);

    return curr_movie;
}

struct Language* process_languages(char* langs) {
    // temp variable for strtok_r
    char* temp_str = NULL;

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