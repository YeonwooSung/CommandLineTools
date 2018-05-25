//
// Created by YeonwooSung on 2018. 5. 25..
//

#include "ls.h"

//Use the extern keyword to use the function pointers.
extern int filter(const struct dirent* entry);
extern int compare(const struct dirent** entry1, const struct dirent** entry2);

/**
 * This function calls scandir to list the contents of the directory.
 *
 * It calls scandir to load in the contents of the directory into
 * the variable struct dirent** directory, and subsequently lists
 * the names of its contents if it succeeded.
 *
 * @param path the path to the directory
 */
void processDir(char* path) {
    struct dirent** directory;
    int numOfFiles = scandir(path, &directory, filter, compare);

    if(numOfFiles < 0) { //scandir failed

        perror("Failed to call the scandir...");
        return;

    } else if (numOfFiles > 0) {

        printDir(directory, numOfFiles);

    } else { //empty directory

        printf("Empty Directory...\n");

    }

    freeDirectory(directory, numOfFiles);
}

/**
 * The starting point of the "ls" command.
 * Checks the number of arguments first, to check the options that are added.
 * After checking the command line options, print out the corresponding message (or file information).
 *
 * The ls command prints out the list of the file(s) and directory(s) in the current directory.
 *
 * @param argc the number of command line arguments.
 * @param argv the values of the command line arguments.
 * @return If succeeded, returns 0. Otherwise, returns -1.
 */
int main(int argc, char** argv) {

    path = (char *) malloc(sizeof(char) * 2); //allocate the memory for the file path.
    strcpy(path, ".");


    if(argc > 1) { //read directory which is specified

        int numOfPaths = checkArgs(argc, argv);

        //print out each directory passed into the program
        for (int i = 0; i < numOfPaths; i++) {
            path = realloc(path,strlen(paths[i]));
            strcpy(path, paths[i]);
            printf(RED "\n%s:\n" RESET, path);
            processDir(path);
        }

        //Free the allocated memory.
        free(paths);

    } else if(argc == 1) { //read current directory

        processDir(path);

    } else {

        //If the number of arguments is incorrect, print out the error message.
        fprintf(stderr, "Incorrect number of arguments\n");

        free(path);
        return -1; //If the number of arguments is incorrect, return -1.
    }

    free(path);
    return 0;
}
