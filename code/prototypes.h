#ifndef PROTOTYPES_H
#define PROTOTYPES_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// for displaying top rated movie for each year
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

//
// function prototypes
//

// process file functions
struct Movie* process_file(char*);
struct Movie* create_movie(char*);
struct Language* process_languages(char*);
struct Language* create_language(char*);

// user input functions
void prompt_search();
int get_choice();
int check_choice(char*);

// display movies by year functions
void display_movies_by_year(struct Movie*);
void search_by_year(struct Movie*, int, int*);

// display top rated by year functions
void display_top_rated_by_year(struct Movie*);
void check_for_same_year(struct Movie*, struct Movies_By_Distinct_Year*);

// display movies by language functions
void display_movies_by_language(struct Movie*);
char* get_lang();
void search_by_language(struct Movie*, char*, int*);
int search_languages(struct Language*, char*);

// free memory functions
void free_distinct_year_list(struct Movies_By_Distinct_Year*);
void free_movie_list(struct Movie*);
void free_languages(struct Language*);

// debugging functions
void print_list(struct Movie*);
void print_movie(struct Movie*);
void print_languages(struct Language*);

#endif