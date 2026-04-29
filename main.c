/*
* Class: CS 374
* Assignment 2: Files & Directories
* Description: This program reads directory entries; finds a file in the current directory 
*   (from which the program is run) based on user-specified criteria; reads and processes the 
*   data in the chosen file; creates a directory; creates new files in the newly created 
*   directory and writes processed data to these files. Specific permission are assigned.
* Name:           Pavlo Kinyshchuk
* ONID:           934-615-543
* Email:          kinyshcp@oregonstate.edu
* Date Submitted: April 27, 2026
*/

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include "menu.h"


/*
* main: entry point of the program. Initializes all menus, then enters a loop where 
*       it prints the menu, gets the user's choice, and processes the user's choice 
*       until the user chooses to exit the program. After the user chooses to exit, 
*       it frees the menus. In every loop iteration, terminal is cleared before printing 
*       the menu again.
* Parameters:
*   argc (int): the number of command-line arguments. Expected to be 1 for Assignment 2
*   argv (char**): an array of command-line arguments
* Returns (int): EXIT_SUCCESS or 0 if the program exits successfully and EXIT_FAILURE or 1 
*   if the program exits with an error. The program exits with an error if the user provides 
*   any command-line arguments.
*/
int main(int argc, char *argv[]) {
    if(argc > 1) {
        fprintf(stderr, "Usage: %s\n", argv[0]);
        return EXIT_FAILURE;
    }

    //1 line copied from:
    //https://www.geeksforgeeks.org/c/clear-console-c-language/
    system("clear"); //linux specific command to clear terminal
                        //os1 server is linux based, hence the command

    int numMenus = 2;
    struct menu** allMenus = initializeAllMenus(numMenus);
    struct menu* menu = allMenus[0];

    srandom(time(NULL));

    while(1) {
        //1 line copied from:
        //https://www.geeksforgeeks.org/c/clear-console-c-language/
        system("clear"); //linux specific command to clear terminal
                         //os1 server is linux based, hence the command

        printMenu(menu);
        int userChoice = getUserChoice(menu);
    
        menu = processUserChoice(menu, userChoice);

        if(!menu) {
            uid_t uid = getuid();  
            struct passwd *pw = getpwuid(uid);
            char* user_name = pw->pw_name;
            printf("\nBye bye %s!\n", user_name);
            break;
        }
    }

    freeMenu(allMenus, numMenus);

    return EXIT_SUCCESS;
}