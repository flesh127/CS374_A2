/*
* Header: directory.h
* Implementation: directory.c
* Description: This file contains the declarations for the directory-related functions.
*   Addition of this header file exposed some flawbacks with current design, because
*   in order to avoid circular dependancies I had to rely on substantial amount of getters
*   from movies.h and in turn jeoperdizing its encapsulation.
*/

#ifndef DIRECTORY
#define DIRECTORY
#include "movies.h"

/*
* processLargestFileByYear: finds the largest file in the current directory with the format movies_*.csv
*     and processes it by creating directory with files for each year with movies with matching year.
* Returns (bool): true if default next menu should be shown. false if the fallback menu should be shown.
*                 Usually fallback is reserved for error handling or branching.
* Side effects: creates directory with files for each year with movies with matching year if a file is found.
*/
bool processLargestFileByYear();

/*
* processSmallestFileByYear: finds the smallest file in the current directory with the format movies_*.csv
*     and processes it by creating directory with files for each year with movies with matching year.
* Returns (bool): true if default next menu should be shown. false if the fallback menu should be shown.
*                 Usually fallback is reserved for error handling or branching.
* Side effects: creates directory with files for each year with movies with matching year if a file is found.
*/
bool processSmallestFileByYear();

/*
* processSpecifyFileByYear: allows the user to specify filename and processes it by creating directory with 
*     files for each year with movies with matching year. If filename is not found, user is prompted to try again 
*     and this function returns false to show fallback menu, which is option selection menu in this case.
* Returns (bool): true if default next menu should be shown. false if the fallback menu should be shown.
*                 Usually fallback is reserved for error handling or branching.
* Side effects: creates directory with files for each year with movies with matching year if a file is found.
*              IF filename is not found, try again message is printed.
*/
bool processSpecifyFileByYear();

#endif