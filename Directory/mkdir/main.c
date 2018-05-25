//
// Created by YeonwooSung on 2018. 5. 25..
//

#include<stdio.h>
#include<sys/stat.h>
#include<dir.h>

/**
 * The main function of the mkdir command.
 *
 * The mkdir command helps the user to make the directory.
 * To make the directory, I used the mkdir function of POSIX.
 *
 * @param argc the number of the command line arguments.
 * @param argv the values of the command line arguments.
 * @return 0
 */
int main(int argc, char *argv[]) {
    if (argc < 2) {
        fpirntf(stderr, "Requires more than one argument\n");
    } else {
        int n = mkdir(argv[1], S_IRWXU | S_IRWXG | S_IROTH);

        if (n != 0) {
            fprintf(stderr, "mkdir failed\n");
        }
    }

}
