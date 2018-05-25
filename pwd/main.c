//
// Created by YeonwooSung on 2018. 5. 25..
//

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#define BUFF_SIZE 1024

/**
 * The main function for the pwd command.
 * The pwd command helps the user to get the absolute file path of the current directory.
 *
 * @return 0
 */
int main() {
    char *cwd = (char *) malloc(sizeof(char) * BUFF_SIZE);

    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("%s\n", cwd);
    } else {
        perror("getcwd() error");
    }

    free(cwd); //free the allocated memory

    return 0;
}
