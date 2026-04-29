#include <stdbool.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include "directory.h"

/* 
* _createDirectoryForYear: creates a directory in current directory with the format onid.movies.rand 
*                          and permissions rwxr-x---. rand is a random number between 0 and 99999.
* Returns (char*): the name of the created directory. Returns NULL if there was an error creating 
*                  the directory.
*/
char* _createDirectoryForYear() {
    int rand = (int)random() % 100000; // 0 to 99999
    char* onid = "kinyshcp";
    char* midPart = "movies";
    size_t dirNameSize = strlen(onid) + strlen(midPart) + 5 + 2 + 1; // 5 for rand, 2 for dots, 1 for null terminator
    char* dirName = malloc(dirNameSize);
    sprintf(dirName, "%s.%s.%d", onid, midPart, rand);
    
    //permission of directory: rwxr-x---
    if(mkdir(dirName, 0750) != 0) {
        perror("Error creating directory");
        return 0;
    }
    printf("Created directory with name %s\n", dirName);

    return dirName;
}

/*
* _createFilesByYear: creates text files for each year in the movies list. Each file is named YYYY.txt 
*       and contains the titles of the movies released in that year, one title per line. The files are 
*       created in a directory with the format onid.movies.rand. The files have permissions rw-r-----.
* Parameters:
*   moviesList (struct movies*): a pointer to the list of movies to process. moviesList->head linked list
        is not assumed to be sorted by year. If moviesList->head is NULL, no files will be created.
* Side effects: files for each year in the movies list are created in a directory with the format 
*    onid.movies.rand. The files have permissions rw-r-----.
*/
void _createFilesByYear(struct movies* moviesList) {
    //permission of directory: rwxr-x---
    char* dirName = _createDirectoryForYear();
    if(!dirName) return;

    MergeSortMovies(moviesList);

    struct movie* currMovie = moviesList->head;
    if(currMovie == NULL) {
        printf("No movies to process\n");
        return;
    }
    int latestYear = -1;
    FILE* currFile = NULL;
    char fileName[9]; //YYYY.txt
    char filePath[strlen(dirName) + 1 + 9]; //dirName/YYYY.txt

    bool sameYear = false;
    while(currMovie) {
        if(getYear(currMovie) == latestYear) {
            sameYear = true;
        } else {
            sameYear = false;
            latestYear = getYear(currMovie);
        }

        if(!sameYear) {
            if(currMovie != moviesList->head) fclose(currFile);
            sprintf(fileName, "%d.txt", latestYear);
            sprintf(filePath, "%s/%s", dirName, fileName);
            currFile = fopen(filePath, "a");
            if (!currFile) {
                perror("fopen");
                return;
            }
            chmod(filePath, 0640); //permission of file: rw-r-----
        }

        char* title = getTitle(currMovie);
        fprintf(currFile, "%s\n", title);

        nextMovie(&currMovie);
    }
    fclose(currFile);
    free(dirName);
}

// Description is in header file 
bool processLargestFileByYear() {
    DIR* dir = opendir(".");
    struct dirent* entry;
    struct stat* st = calloc(1, sizeof(struct stat));
    char* largestFileName = calloc(256, sizeof(char));
    size_t largestFileSize = 0;

    while((entry = readdir(dir)) != NULL) {
        char* ext = strrchr(entry->d_name, '.');
        if(!ext || strcmp(ext, ".csv")) continue; // skip non-csv files
        if(strncmp(entry->d_name, "movies_", 7) != 0) continue;

        stat(entry->d_name, st);
        if(S_ISREG(st->st_mode)) {
            if(st->st_size > largestFileSize) {
                largestFileSize = st->st_size;
                strcpy(largestFileName, entry->d_name);
            }
        }
    }

    closedir(dir);
    free(st);

    if(largestFileSize == 0) {
        printf("No files found matching the pattern movies_*.csv\n");
        free(largestFileName);
        return false;
    }

    struct movies* moviesList = getMovies(largestFileName);

    if(!moviesList) {
        freeMovies(moviesList);
        free(largestFileName);
        return false;
    }

    _createFilesByYear(moviesList);

    free(largestFileName);
    freeMovies(moviesList);

    return true;
}

// Description is in header file 
bool processSmallestFileByYear() {
    DIR* dir = opendir(".");
    struct dirent* entry;
    struct stat* st = calloc(1, sizeof(struct stat));
    char* smallestFileName = calloc(256, sizeof(char));
    size_t smallestFileSize = -1;

    while((entry = readdir(dir)) != NULL) {
        char* ext = strrchr(entry->d_name, '.');
        if(!ext || strcmp(ext, ".csv")) continue; // skip non-csv files
        if(strncmp(entry->d_name, "movies_", 7) != 0) continue;

        stat(entry->d_name, st);
        if(S_ISREG(st->st_mode)) {
            if(st->st_size < smallestFileSize) {
                smallestFileSize = st->st_size;
                strcpy(smallestFileName, entry->d_name);
            }
        }
    }

    closedir(dir);
    free(st);

    if(smallestFileSize == -1) {
        printf("No files found matching the pattern movies_*.csv\n");
        free(smallestFileName);
        return false;
    }

    struct movies* moviesList = getMovies(smallestFileName);

    if(!moviesList) {
        freeMovies(moviesList);
        free(smallestFileName);
        return false;
    }

    _createFilesByYear(moviesList);

    free(smallestFileName);
    freeMovies(moviesList);

    return true;
}

// Description is in header file 
bool processSpecifyFileByYear() {
    char* filename = NULL;
    size_t len = 0;

    printf("Enter the complete file name: ");
    getline(&filename, &len, stdin);
    filename[strcspn(filename, "\n")] = '\0';

    DIR* dir = opendir(".");
    struct dirent* entry;
    struct stat* st = calloc(1, sizeof(struct stat));
    
    bool exists = false;
    while((entry = readdir(dir)) != NULL) {
        char* ext = strrchr(entry->d_name, '.');
        if(strcmp(entry->d_name, filename) != 0) continue; 

        stat(entry->d_name, st);
        if(S_ISREG(st->st_mode)) {
            exists = true;
            break;
        }
    }

    closedir(dir);
    free(st);

    if(!exists) {
        printf("The file %s was not found. Try again.\n", filename);
        free(filename);
        return false;
    }

    struct movies* moviesList = getMovies(filename);

    if(!moviesList) {
        freeMovies(moviesList);
        free(filename);
        return false;
    }

    _createFilesByYear(moviesList);

    free(filename);
    freeMovies(moviesList);

    return true;
}