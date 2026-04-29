#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "movies.h"

//year will always be 4 digit integer
#define YEAR_LEN 4
//You can assume that the maximum number of languages any movie can be released in is 5
#define MAX_LANGUAGES 5
//You can assume that the maximum length of a language string is 20 characters.
#define MAX_LANGUAGE_LEN 20
//rating can be an integer or a double with one digit after the decimal point
#define RATING_LEN 3
#define NUM_COLUMNS 4

// Description is in header file 
// Technical Requirement 3: Create a struct called movie with the data for that line
struct movie {
    //You cannot assume a maximum length of the movie title
    char* title; 
    char year[YEAR_LEN + 1];
    //One or more string values
    //You cannot assume any particular case for the letters in the language
    char languages[MAX_LANGUAGES][MAX_LANGUAGE_LEN + 1];
    //A number between 1 and 10 (inclusive of both 1 and 10)
    char rating[RATING_LEN + 1];
    struct movie* next;
};

// Description is in header file 
struct movies;

/*
* createMovie: initializes a movie struct by breaking up the line into tokens
*              and returns a pointer to that struct. Must be able to handle all 
*              possible cases of line specified in the assignment description:
*              "Format of CSV File".
* Parameters:
*   currLine (char*): a line from input file which holds all information about a
*                     movie. 
* Returns (struct movie*): a pointer to the initialized movie struct
*/
struct movie* createMovie(char* currLine) {
    struct movie* currMovie = malloc(sizeof(*currMovie));
    char* saveptr;

    // Technical Requirement 2: Break up the line into tokens
    char* token = strtok_r(currLine, ",", &saveptr);
    currMovie->title = calloc(strlen(token) + 1, sizeof(char));
    strcpy(currMovie->title, token);

    token = strtok_r(NULL, ",", &saveptr);
    strcpy(currMovie->year, token);

    token = strtok_r(NULL, ",", &saveptr);
    token = token + 1; //remove [
    token[strlen(token) - 1] = '\0'; //remove ]
    char* langSaveptr;
    char* lang;
    for(int i = 0; i < MAX_LANGUAGES; i++) {
        if(i == 0) {
            lang = strtok_r(token, ";", &langSaveptr);
        } else {
            lang = strtok_r(NULL, ";", &langSaveptr);
        }

        if(!lang) {
            currMovie->languages[i][0] = '\0';
            break;
        };

        //I assume length of languages doesn't excceed 20, so strncpy is not used
        strcpy(currMovie->languages[i], lang);
    }

    token = strtok_r(NULL, ",", &saveptr);
    //some lines in sample input had a newline character
    if(token[strlen(token) - 1] == '\n') {
        token[strlen(token) - 1] = '\0';
    }
    strcpy(currMovie->rating, token);

    currMovie->next = NULL;

    return currMovie;
}

// Description is in header file 
struct movies* getMovies(const char* filePath) {
    printf("Now processing the chosen file named %s\n", filePath);

    FILE* movieFile = fopen(filePath, "r");
    size_t len = 0;
    ssize_t nread; 
    char* currLine = NULL;

    struct movie* head = NULL;
    struct movie* tail = NULL;

    //extracting header line to get column order
    if((nread = getline(&currLine, &len, movieFile)) == -1) {
        printf("Failed extracting header line\n");
        return NULL;
    }
    
    int numMovies = 0;
    //loop until EOF or error
    while((nread = getline(&currLine, &len, movieFile)) != -1) {
        // Technical Requirement 1: Read data from the file line by line
        struct movie* newNode = createMovie(currLine);

        if(head == NULL) {
            // This is the first node in the linked link
            // Set the head and the tail to this node
            head = newNode;
            tail = newNode;
        } else {
            // This is not the first node.
            // Add this node to the list and advance the tail
            tail->next = newNode;
            tail = newNode;
        }

        numMovies++;
    }

    printf("Parsed data for %d movies from %s\n", numMovies, filePath);

    free(currLine);
    fclose(movieFile);

    struct movies* moviesList = malloc(sizeof(*moviesList));
    moviesList->head = head;
    moviesList->sortedByYear = false;

    // Technical Requirement 4: Create a linked list containing all the movie structures
    return moviesList;
}

// Description is in header file 
int getYear(struct movie* a) {
    return atoi(a->year);
}

// Description is in header file 
char* getTitle(struct movie* a) {
    return a->title;
}

// Description is in header file 
void nextMovie(struct movie** a) {
    *a = (*a)->next;
}

/*
* _SortedMerge: merges two sorted linked lists of movies by year
* Parameters:
*   a (struct movie*): a pointer to the first sorted linked list
*   b (struct movie*): a pointer to the second sorted linked list
* Returns (struct movie*): a pointer to the merged sorted linked list
*/
//Inspired by geeksforgeeks merge sort for linked list: https://www.geeksforgeeks.org/merge-sort-for-linked-list/
struct movie* _SortedMerge(struct movie* a, struct movie* b) {
    struct movie* result = NULL;

    if (a == NULL)
        return (b);
    else if (b == NULL)
        return (a);

    //ascending order, but for my goal i could pick any order
    if (atoi(a->year) <= atoi(b->year)) {
        result = a;
        result->next = _SortedMerge(a->next, b);
    } else {
        result = b;
        result->next = _SortedMerge(a, b->next);
    }

    return (result);
}

/*
* _FrontBackSplit: splits the source linked list into two halves
* Parameters:
*   source (struct movie*): a pointer to the source linked list
*   frontRef (struct movie**): a pointer to a pointer to the front of the split list
*   backRef (struct movie**): a pointer to a pointer to the back of the split list
* Side Effects: splits the source linked list into two halves
*/
//Inspired by geeksforgeeks merge sort for linked list: https://www.geeksforgeeks.org/merge-sort-for-linked-list/
void _FrontBackSplit(struct movie* source, struct movie** frontRef, struct movie** backRef) {
    struct movie* fast;
    struct movie* slow;
    slow = source;
    fast = source->next;

    while (fast != NULL) {
        fast = fast->next;
        if (fast != NULL) {
            slow = slow->next;
            fast = fast->next;
        }
    }

    *frontRef = source;
    *backRef = slow->next;
    slow->next = NULL;
}

/*
* _MergeSort: sorts a linked list of movies by release year using the merge sort algorithm
* Parameters:
*   headRef (struct movie**): a pointer to a pointer to the head of the linked list
* Side Effects: sorts the linked list of movies by release year
*/
//Inspired by geeksforgeeks merge sort for linked list: https://www.geeksforgeeks.org/merge-sort-for-linked-list/
void _MergeSort(struct movie** headRef) {
    struct movie* head = *headRef;
    struct movie* a; 
    struct movie* b;

    if ((head == NULL) || (head->next == NULL)) return;

    // Split head into 'a' and 'b' sublists 
    _FrontBackSplit(head, &a, &b);

    _MergeSort(&a);
    _MergeSort(&b);

    *headRef = _SortedMerge(a, b);
}

// Description is in header file 
void MergeSortMovies(struct movies* moviesList) {
    if(moviesList->sortedByYear) return;
    _MergeSort(&moviesList->head);
    moviesList->sortedByYear = true;
}

// Description is in header file 
void freeMovies(struct movies* moviesList) {
    struct movie* currMovie = moviesList->head;
    struct movie* temp;
    while(currMovie) {
        temp = currMovie;
        currMovie = currMovie->next;
        free(temp->title);
        free(temp);
    }
    free(moviesList);
}