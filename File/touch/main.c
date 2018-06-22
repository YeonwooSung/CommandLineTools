//
// Created by YeonwooSung on 2018. 5. 28..
//

//included libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <utime.h>
#include <sys/stat.h>

//macros
#define USAGE_MESSAGE "Usage: touch [-A [-][[hh]mm]SS] [-acfhm] [-r file] [-t [[CC]YY]MMDDhhmm[.SS]] file ...\n"
#define FAILED_MESSAGE "failed to create file"
#define BASIC_SIZE 20
#define LARGER_SIZE 40

void setModificationTimeStat() {
    //TODO
}

void setAccessTimeStat() {
    //TODO
}

/**
 * Check the options that are given through the standard input.
 * If the user input the wrong option, print out the usage message and terminate the process.
 * After finish checking all options, this method does corresponding actions that the user asked.
 *
 * @param argc the number of command line arguments.
 * @param argv the values of command line arguments.
 * @param fileName the name of the file.
 * @param the FILE type pointer points the target file.
 */
void checkOptions(int argc, char *argv[], char *fileName, FILE *file) {
    char flag = 0; //the flag to check if the user input any options.

    //the pointers to store the option arguments.
    char *aTime;
    char *mTime;
    char *tTime;
    char *rFileName;

    int opt;
    struct stat fileStat;
    struct utimbuf newTimeInfo;

    //the while loop to check all command line options.
    while ((opt = getopt(argc, argv, "a:m:r:t:c:")) != -1) {

        switch (opt) { //the switch statement to check the command line options.

            case 'a' :
                flag += 1;
                aTime = (char *) malloc(sizeof(char) * BASIC_SIZE);
                strcpy(aTime, optarg);
                break;

            case 'm' :
                flag += 2;
                mTime = (char *) malloc(sizeof(char) * BASIC_SIZE);
                strcpy(mTime, optarg);
                break;

            case 'r' :
                flag += 4;
                rFileName = (char *) malloc(sizeof(char) * LARGER_SIZE);
                strcpy(rFileName, optarg);
                break;

            case 't' :
                flag += 8;
                tTime = (char *) malloc(sizeof(char) * BASIC_SIZE);
                strcpy(tTime, optarg);
                break;

            case 'c' :
                flag += 16;

            default:
                fprintf(stderr, USAGE_MESSAGE);
                exit(0); //terminate the touch process.

        } //switch statement ends

    } //the while loop ends

    strcpy(fileName, argv[optind]); //copy the file name by using variable optind to get the non-option argument.

    file = fopen(fileName, "a"); //open the speicified file (or create new file)

    //check the value of flag to know if there is any
    if (flag != 0) {
        //the stat function obtains information about the named file and writes it to the area pointed to by the buf argument.
        stat(fileName, &fileStat);

        //TODO set stats that are according to option arguments.
        if (flag >= 16) {
            flag -= 16;

            newTimeInfo.actime = time(NULL);
            newTimeInfo.modtime = time(NULL);
        }

        if (flag >= 8) {
            flag -= 8;

            //TODO what does the -t option really do?
        }

        if (flag  >= 4) {
            flag -= 4;

            //TODO rename
        }

        if (flag >= 2) {
            flag -= 2;

            //TODO set the modification time by using epoch time.
        }

        if (flag == 1) {
            //TODO set the access time by using epoch time.
        }
        utime(fileName, &newTimeInfo);
    }
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
    if (argc < 2 || argc %2 != 0) {
        fprintf(stderr, USAGE_MESSAGE);
        return -1;
    }

    char *fileName = (char *) malloc(sizeof(char) * 50);
    File *file; //the target file's pointer.

    checkOptions(argc, argv, fileName);

    int n = open(fileName, O_WRONLY|O_CREAT|O_TRUNC);

    if (n != 0) {
        return n;
    }

    fprintf(stderr, FAILED_MESSAGE);

    return -1;
}
