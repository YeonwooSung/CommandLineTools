//
// Created by YeonwooSung on 2018. 6. 23..
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <grp.h>
#include <pwd.h>

#define USAGE_MESSAGE "usage: chown [-f] [-h] [-R] owner[:group] file ...\n\tchown [-f] [-h] [-R] :group file ..."
#define FAILED "chown failed\n"
#define UID_FAILED "Failed to get uid\n"
#define GID_FAILED "Failed to get gid\n"

/**
 * This function uses the POSIX system call "chown" to change the user name and group name of the specific file.
 *
 * @param file_path the path of the target file.
 * @param suppressMessage If 1, suppress all messages except error messages.
 */
void doChown(const char *file_path, char suppressMessage) {
    struct stat info;
    stat(file_path, &info); //get the file stat
    struct passwd *pwd = getpwuid(info.st_uid);

    //check if the user information is correct
    if (pwd == NULL) {
        fprintf(stderr, UID_FAILED);
        exit(0);
    }

    struct group *grp = getgrgid(info.st_gid);

    //check if the group information is correct.
    if (grp == NULL) {
        fprintf(stderr, GID_FAILED);
        exit(0);
    }

    uid_t uid; //user id
    gid_t gid; //group id

    uid = pwd->pw_uid;

    gid = grp->gr_gid;

    if (chown(file_path, uid, gid) == -1) {
        fprintf(stderr, FAILED);
        exit(0);
    }

    if (suppressMessage != 1) {
        printf("chown success!\n");
    }
}

/**
 * This function checks the command line options and modify the value of the option flag.
 *
 * @param argc the argument count
 * @param argv the values of command line arguments
 * @param optFlag the option flag to check which options are selected.
 * @param filePath the file path name.
 */
void checkArguments(int argc, char *argv[], char *optFlag, char *filePath) {
    int opt;
    char flag = 0;

    while ((opt = getopt(argc, argv, "f:h:R:"))) {

        switch (opt) {
            case 'f' : //The -f option suppresses all error messages except usage message.
                flag += 1;
                if (optarg) {
                    filePath = (char *) malloc(strlen(optarg) + 1);
                    strcpy(filePath, optarg);
                }
                break;
            case 'h' : //Just change the owner name of the symbolic link, not the file that of the file pointed to by it.
                flag += 2;
                if (optarg) {
                    filePath = (char *) malloc(strlen(optarg) + 1);
                    strcpy(filePath, optarg);
                }
                break;
            case 'R' : //Descends directories recursively, changing the ownership for each file.
                flag += 4;
                if (optarg) {
                    filePath = (char *) malloc(strlen(optarg) + 1);
                    strcpy(filePath, optarg);
                }
                break;
            default:
                fprintf(stderr, USAGE_MESSAGE);
                exit(1);
        }//switch statement ends

    }//while loop ends

    *optFlag = flag;
}

/**
 * The chown command supports the user to change the user name and group name of the specific file.
 *
 * @param argc the count of the command line arguments
 * @param argv the values of the command line arguments
 * @return 0
 */
int main(int argc, char *argv[]) {
    if (argc <= 1) {
        fprintf(stderr, USAGE_MESSAGE);
        exit(0);
    }

    char option = 0;
    char suppressMessages = 0;
    char *filePath = NULL;

    checkArguments(argc, argv, &option, filePath);

    if (filePath == NULL) {
        fprintf(stderr, USAGE_MESSAGE);
        exit(0);
    }

    char hFlag = 0; //The "-h" option is for changing the owner of the symbolic link.
    char rFlag = 0; //The "-R" option is for recursive chown.

    //check if the user used the -R option
    if (option >= 4) {
        rFlag = 1;

        option -= 4;
    }

    //check if the user used the -h option
    if (option >= 2) {
        hFlag = 1;

        option -= 2;
    }

    //check if the user used the -f option
    if (option >= 1) {
        suppressMessages = 1;

        option -= 1;
    }

    if (rFlag) {
        //TODO recursive chown
        //TODO also, need to consider if the user also used the -h option!
    } else if (hFlag) {
        //TODO change ownership of the encountered symbolic link
    } else {
        doChown(filePath, suppressMessages); //do chown
    }

    return 0;
}
