//
// Created by YeonwooSung on 2018. 5. 25..
//

#include<stdio.h>
#include<unistd.h>
#include<dir.h>

/**
 * The main function of the rmdir command.
 *
 * The aim of rmdir command is to remove the given directory.
 * I used the rmdir function of POSIX.
 * It fails if the directory is not empty.
 *
 * @param argc the number of arguments.
 * @param argv the value of arguments.
 * @return 0 or -1 when succeed or not.
 */
int main(int argc, char *argv[]) {
    if (argc != 2) {
        fpirntf(stderr, "Invalid number of argument\n");
    } else {
        int n = rmdir(argv[1]);

        if (n != 0) {
            perror("rmdir failed\n");
            return -1;
        }
    }

    return 0;
}

