//
// Created by YeonwooSung on 2018. 5. 25..
//

#include <stdio.h>

/**
 * The main function for the echo command.
 *
 * The aim of the echo command is to print out the given string.
 *
 * @param argc the number of command line arguments.
 * @param argv the value of command line arguments.
 * @return 0
 */
int main(int argc, char *argv[]) {
    if (argc > 1) { //check the number of command line arguments.
        for (unsigned int i = 1; i < argc) {
            printf("%s", argv[i]);
        }
    }

    printf("\n");

    return 0;
}
