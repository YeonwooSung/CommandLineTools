//
// Created by YeonwooSung on 2018. 5. 28..
//

//included libraries
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

//macros
#define USAGE_MESSAGE "Usage: touch [-A [-][[hh]mm]SS] [-acfhm] [-r file] [-t [[CC]YY]MMDDhhmm[.SS]] file ..."
#define FAILED_MESSAGE "failed to create file"


void checkOptions(int argc, char *argv[], char *fileName) {
    int opt;

    while ((opt = getopt(argc, argv, "A:r:t:acfhm")) != -1) {

        switch (opt) {
            //TODO cases
        } //switch statement ends

    } //the while loop ends
}

/**
 * The main function of the touch command.
 *
 * The touch command helps the user to create new file.
 *
 * @param argc the number of arguments.
 * @param argv the values of arguments.
 * @return -1 when the touch command failed, otherwise n where the n is the file descriptor of the created file.
 */
int main(int argc, char *argv[]) {

    //check the number of command line arguments.
    if (argc < 2) {
        fprintf(stderr, USAGE_MESSAGE);
        return -1;
    }

    char *fileName = (char *) malloc(sizeof(char) * 50);

    //TODO actions for the corresponding options?
    checkOptions(argc, argv, fileName);

    int n = open(fileName, O_WRONLY|O_CREAT|O_TRUNC);

    if (n != 0) {
        return n;
    }

    fprintf(stderr, FAILED_MESSAGE);

    return -1;
}
