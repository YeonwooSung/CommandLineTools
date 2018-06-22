//
// Created by YeonwooSung on 2018. 5. 28..
//

//included libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdint.h>
#include <time.h>
#include <utime.h>
#include <sys/stat.h>

//macros
#define USAGE_MESSAGE "Usage: touch [-acm] [-r file] [-t [[CC]YY]MMDDhhmm[.SS]] file ...\n"
#define FAILED_MESSAGE "failed to create file"
#define BASIC_SIZE 20
#define LARGER_SIZE 40

/**
 * This functions helps the program to convert the date format string to the epoch time to get the UNIX timestamp.
 * @param str the date format string
 * @return the UNIX timestamp
 */
time_t convertStringToEpochTime(char *str) {
    struct tm tmVar;
    time_t timeVar;

    char yearArray[5];
    char monthArray[3];
    char dayArray[3];
    char hourArray[3];
    char minuteArray[3];
    char secArray[3];

    int i;

    //use the for loop to get the year
    for (i = 0; i < 4; i++) {
        year[i] = *str;
        str += 1; //add 1 to str so that the for loop could access 4 characters in the str.
    }
    yearArray[4] = '\0'; //put the terminator at the end of the yearArray.
    tmVar.tm_year = atoi(yearArray);

    //use the for loop to get the month
    for (i = 0; i < 2; i++) {
        month[i] = *str;
        str += 1;
    }
    monthArray[2] = '\0';
    tmVar.tm_mon = atoi(monthArray);

    //use the for loop to get the day
    for (i = 0; i < 2; i++) {
        dayArray[i] = *str;
        str += 1;
    }
    dayArray[2] = '\0';
    tmVar.tm_mday = atoi(dayArray);

    //use the for loop to get the hour
    for (i = 0; i < 2; i++) {
        hourArray[i] = *str;
        str += 1;
    }
    hourArray[2] = '\0';
    tmVar.tm_hour = atoi(hourArray);

    //use the for loop to get the minute
    for (i = 0; i < 2; i++) {
        minuteArray[i] = *str;
        str += 1;
    }
    minuteArray[2] = '\0';
    tmVar.tm_min = atoi(minuteArray);

    //use the if statement to check if the user input the numeric characters for second.
    if (*str != '.') {
        str += 1;

        //use the for loop to get the second
        for (i = 0; i < 2; i++) {
            secArray[i] = *str;
        }
        secArray[2] = '\0';

        tmVar.tm_sec = atoi(secArray);
    }

    timeVar = mktime(&tmVar);

    return timeVar; //return the UNIX timestamp (epoch time)
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

        if (flag >= 16) {
            flag -= 16;

            newTimeInfo.actime = time(NULL); //change the access time of the target file to now.
            newTimeInfo.modtime = time(NULL); //change the modification time of the target file to now.
        }

        if (flag >= 8) {
            flag -= 8;

            time_t epochTime = convertStringToEpochTime(tTime); //convert the input date string to the epoch time

            newTimeInfo.actime = epochTime; //change the access time of the target file to the specific time.
            newTimeInfo.modtime = epochTime; //change the modification time of the target file to the specific time.
        }

        if (flag  >= 4) {
            flag -= 4;

            rename(fileName, rFileName); //rename the target file.
        }

        if (flag >= 2) {
            flag -= 2;

            newTimeInfo.modtime = time(NULL); //change the modification time of the target file to now.
        }

        if (flag == 1) {
            newTimeInfo.actime = time(NULL); //change the access time of the target file to now.
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
