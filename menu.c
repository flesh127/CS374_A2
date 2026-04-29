#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "menu.h"

/*
* choice: a struct representing a menu choice
* Members:
*  process (bool (*)(void)): a pointer to a function that processes the choice
*           and return true to use default next menu and false to use fallback menu.
*  description (char*): a string describing the choice to be printed in the menu
*  next_menu (struct menu*): a pointer to the menu struct that represents the next menu 
*       to be printed after processing the choice
*  fallback_menu (struct menu*): a pointer to the menu struct that represents the fallback menu
*      to be printed if the process function returns false. This is needed for error handling
*      or branching.
*/
struct choice {
    bool (*process)();
    char* description;
    struct menu* next_menu;
    struct menu* fallback_menu; // I assume i will need at most 2 menus
};

// Description is in header file 
struct menu {
    struct choice** choices;
    int numChoices;
    char* description;
};

/* 
* initializeChoice: initializes a menu choice struct with the given process function,
*                   description, next and fallback menus.
* Parameters:
*   process (void (*)(void)): a pointer to the function that will process 
*       the choice
*   description (char*): a string describing the choice
*   next_menu (struct menu*): a pointer to default next menu
*   fallback_menu (struct menu*): a pointer to fallback menu
* Returns (struct choice*): pointer to the initialized menu choice struct
*/
struct choice* initializeChoice(bool (*process)(), char* description, 
        struct menu* next_menu, struct menu* fallback_menu) {
    struct choice* choice = malloc(sizeof(*choice));

    choice->process = process;
    choice->description = description;
    choice->next_menu = next_menu;
    choice->fallback_menu = fallback_menu;

    return choice;
}

/*
* initializeMenu: initializes a menu struct with the given number of choices. Choices
*       are not initialized in this function because they are predefined and their
*       descriptions are known at compile time.
* Parameters:
*   numChoices (int): the number of choices for the menu. Used for allocating memory
* Returns (struct menu*): pointer to the initialized menu struct
*/
struct menu* initializeMenu(int numChoices) {
    struct menu* menu = malloc(sizeof(struct menu));
    menu->choices = calloc(numChoices, sizeof(struct choice*));
    menu->numChoices = numChoices;
    menu->description = NULL;

    return menu;
}

// Description is in header file 
struct menu** initializeAllMenus(int numMenus) {
    struct menu* mainMenu = initializeMenu(2);
    struct menu* fileOptionsMenu = initializeMenu(3);
    struct menu** allMenus = calloc(numMenus, sizeof(struct menu*));
    allMenus[0] = mainMenu;
    allMenus[1] = fileOptionsMenu;

    mainMenu->description = "Main Menu";
    mainMenu->choices[0] = initializeChoice(NULL, "Select file to process", fileOptionsMenu, mainMenu);
    mainMenu->choices[1] = initializeChoice(NULL, "Exit the program", NULL, NULL);

    fileOptionsMenu->description = "Which file do you want to process?";
    fileOptionsMenu->choices[0] = initializeChoice(processLargestFileByYear, 
        "Pick the largest file", mainMenu, mainMenu);
    fileOptionsMenu->choices[1] = initializeChoice(processSmallestFileByYear, 
        "Pick the smallest file", mainMenu, mainMenu);
    fileOptionsMenu->choices[2] = initializeChoice(processSpecifyFileByYear, 
        "Specify the name of a file", mainMenu, fileOptionsMenu);

    return allMenus;
}

// Description is in header file 
void printMenu(struct menu* menu) {
    printf("%s\n", menu->description);
    for(int i = 0; i < menu->numChoices; i++) {
        printf("%d. %s\n", i + 1, menu->choices[i]->description);
    }
    printf("\n");
}

// Description is in header file 
void clickEnterToContinue() {
    printf("\nPlease press [enter] to continue.\n");
    while (getchar() != '\n' );
}

// Description is in header file 
int getUserChoice(struct menu* menu) {
    int userChoice;

    printf("Enter a choice from %d to %d: ", 1, menu->numChoices);
    // i am validating that input is a number despite assimption in assigmment description
    while(scanf("%d", &userChoice) != 1 || userChoice < 1 || userChoice > menu->numChoices) {
        printf("Invalid input. Please enter a number between %d and %d: ", 1, menu->numChoices);

        //1 line copied from:
        //https://stackoverflow.com/questions/7898215/how-can-i-clear-an-input-buffer-in-c
        while ( getchar() != '\n' );
    }
    while ( getchar() != '\n' );

    return userChoice;
}

// Description is in header file 
struct menu* processUserChoice(struct menu* menu, int userChoice) {
    bool notFallback = true;

    if(menu->choices[userChoice - 1]->process) {
        notFallback = menu->choices[userChoice - 1]->process();
        clickEnterToContinue();
    }

    if(notFallback) {
        return menu->choices[userChoice - 1]->next_menu;
    } else { 
        return menu->choices[userChoice - 1]->fallback_menu;
    }
}

// Description is in header file 
void freeMenu(struct menu** allMenus, int numMenus) {
    for(int i = 0; i < numMenus; i++) {
        struct menu* menu = allMenus[i];
        for(int j = 0; j < menu->numChoices; j++) {
            free(menu->choices[j]);
        }
        free(menu->choices);
        free(menu);
    }
    free(allMenus);
}