/*
* Header: menu.h
* Implementation: menu.c
* Description: This file contains the declarations for the menu struct and its associated functions.
* Original source: Assignment 1 submission
* Major changes:  
*   - Updated the menu struct to include a description field.
*   - Updated the choice struct to include a fallback menu pointer and updated the process function
*       to return a boolean indicating whether to use the next menu or the fallback menu.
*   - Updated the processUserChoice function to use the output of the process function to determine
*       which menu to return.
*   - Swapped initializeMenu with initializeAllMenus which initializes all menus at once. In A1,
*       only 1 menu was used so it was sufficient but not for A2.
*   - Updated freeMenu to free all menus and their choices instead of just 1 menu.
*/

#ifndef MENU
#define MENU

#include "movies.h"
#include "directory.h"

/*
* menu: a struct representing a menu
* Members:
*   choices (struct choice**): a pointer to an array of pointers to menu choices
*   numChoices (int): the number of menu choices
*   description (char*): a string describing the menu. Can be thought of as header
*       or title for the menu. Used when printing to terminal.
*/
struct menu;

/*
* initializeAllMenus: initializes all the menus for the application.
* Parameters:
*   numMenus (int): the number of menus to initialize. Optional since 
*       the number of menus is known at compile time, but included for flexibility.
* Returns (struct menu**): a pointer to an array of pointers to the initialized menus
*/
struct menu** initializeAllMenus(int numMenus);

/*
* printMenu: prints the menu options. 
* Parameters:
*   menu (struct menu*): a pointer to the menu struct
* Side Effects: prints the menu options to the terminal. The format of each choice is 
*            "choice_number. choice_description"
*/
void printMenu(struct menu* menu);

/*
* clickEnterToContinue: prompts the user to press enter to continue and waits 
*       for the user to do so. This function is needed because after processing 
*       a choice, terminal is cleared and menu is printed again. This function
*       gives the user a chance to read the output before clearing the terminal.
* Side Effects: prints a message to the terminal and waits for the user to press enter.
*/
void clickEnterToContinue();

/*
* getUserChoice: prompts the user to enter a choice and validates the input.
* Parameters:
*   menu (struct menu*): a pointer to the menu struct. Used to access number of 
*       choices for input validation.
* Returns (int): valid user choice
*/
int getUserChoice(struct menu* menu);

/*
* processUserChoice: processes the user's choice by calling the corresponding 
*       process function for that choice. If the process function pointer for 
*       that choice is NULL, than no processing is done. Output of process function
*       determines whether next or fallback menu is returned. Either can be NULL
*       and if so, that represents the exit program outcome.
* Parameters:
*   menu (struct menu*): a pointer to the menu struct
*   userChoice (int): the user's choice
* Returns (struct menu*): a pointer to the menu to be processed next. NULL is deemed
*       the exit program outcome.
*/
struct menu* processUserChoice(struct menu* menu, int userChoice);

/*
* freeMenu: frees the memory allocated for all menus and their choices.
* Parameters:
*   allMenus (struct menu**): a pointer to an array of pointers to the menus
*   numMenus (int): the number of menus
* Side Effects: frees the memory allocated for the menu structs and their choices.
*/
void freeMenu(struct menu** allMenus, int numMenus);

#endif