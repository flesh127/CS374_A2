/*
* Header: movies.h
* Implementation: movies.c
* Description: contains the struct definitions and function prototypes for the movies module
* Original source: Assignment 1 submission
* Changes:
*   - removed void showHighestRatedMovieForEachYear(struct movie* head);
*   - removed void showMoviesInLanguage(struct movie* head);
*   - removed void showMoviesAtGivenYear(struct movie* head);
*   - added void MergeSortMovies(struct movies* moviesList);
*   - added movies struct to wrap the linked list of movies
*   - updated freeMovies to handle the movies struct
*   - added getters for movie->year, movie->title, and movie->next
*   - renamed processMovies to getMovies and updated it to return a movies struct
*   - exposed movies struct members
*/

#ifndef MOVIES
#define MOVIES

#include <stdbool.h>

/*
* movie: a struct representing a movie 
* Members:
*   title (char*): movie title. You cannot assume a maximum length of the movie title
*   year (char[YEAR_LEN + 1]): movie release year. YEAR_LEN is 4.
*   language (char[MAX_LANGUAGES][MAX_LANGUAGE_LEN + 1]): languages in which the movie 
*       was released. MAX_LANGUAGES is 5, and MAX_LANGUAGE_LEN is 20.
*   rating (char[RATING_LEN + 1]): movie rating. RATING_LEN is 3 because rating can 
*       be an integer 1 to 10 or a double with one digit after the decimal point.
*   next (struct movie*): pointer to the next movie in the linked list
*/
struct movie;

/*
* movies: a struct representing a list of movies
* Members:
*   head (struct movie*): pointer to the head of the linked list of movies
*   sortedByYear (bool): a flag indicating whether the linked list is sorted by year
*/
struct movies{
    struct movie* head;
    bool sortedByYear;
};

/*
* getMovies: creates a linked list of all movie structs by parsing the input file
* Parameters:
*   filePath (const char*): path to the input file
* Returns (struct movies*): a pointer to the movies struct containing the linked list of 
*   movie structs
*/
struct movies* getMovies(const char* filePath);

/*
* getYear: returns the release year of a movie
* Parameters:
*   a (struct movie*): a pointer to the movie struct
* Returns (int): the release year of the movie
*/
int getYear(struct movie* a);

/*
* getTitle: returns the title of a movie
* Parameters:
*   a (struct movie*): a pointer to the movie struct
* Returns (char*): the title of the movie
*/
char* getTitle(struct movie* a);

/*
* nextMovie: advances a pointer to the next movie in the linked list
* Parameters:
*   a (struct movie**): a pointer to a pointer to the current movie struct
* Side Effects: advances the pointer to the next movie in the linked list
*/
void nextMovie(struct movie** a);

/*
* MergeSortMovies: sorts the linked list of movies by release year using the merge sort algorithm
* Parameters:
*   moviesList (struct movies*): a pointer to the movies struct containing the linked list of movies
* Side Effects: sorts the linked list of movies by release year
*/
void MergeSortMovies(struct movies* moviesList);

/*
* freeMovies: frees the memory allocated for the linked list of movies
* Parameters:
*   moviesList (struct movies*): a pointer to the movies struct containing the linked list of movies
* Side Effects: frees the memory allocated for the linked list of movies
*/
void freeMovies(struct movies* moviesList);

#endif