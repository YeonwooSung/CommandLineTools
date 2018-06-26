//
// Created by YeonwooSung on 2018. 6. 23..
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>

#define USAGE_MESSAGE "usage: chown [-f] [-h] [-R] owner[:group] file ...\n\tchown [-f] [-h] [-R] :group file ..."
#define FAILED "chown failed\n"
#define UID_FAILED "Failed to get uid\n"
#define GID_FAILED "Failed to get gid\n"

/**
 * This function uses the POSIX system call "chown" to change the user name and group name of the specific file.
 *
 * @param file_path the path of the target file.
 * @param user_name the new user name.
 * @param group_name the new group name.
 */
void doChown (const char *file_path, const char *user_name, const char *group_name) {
    uid_t uid; //user id
    gid_t gid; //group id
    struct passwd *pwd;
    struct group  *grp;

    pwd = getpwnam(user_name);

    //check if the user information is correct
    if (pwd == NULL) {
        die(UID_FAILED);
    }

    uid = pwd->pw_uid;

    grp = getgrnam(group_name);

    //check if the group information is correct.
    if (grp == NULL) {
        die(GID_FAILED);
    }
    gid = grp->gr_gid;

    if (chown(file_path, uid, gid) == -1) {
        die(FAILED);
    }
}

/**
 * This function checks the command line options and modify the value of the option flag.
 *
 * @param argc the argument count
 * @param argv the values of command line arguments
 * @param optFlag the option flag to check which options are selected.
 */
void checkArguments(int argc, char *argv[], char *optFlag) {
    int opt;
    char flag = 0;

    while ((opt = getopt(argc, argv, "f:h:R:"))) {

        switch (opt) {
            case 'f' : //The -f option suppresses all error messages except usage message.
                flag += 1;
                break;
            case 'h' : //Just change the owner name of the symbolic link, not the file that of the file pointed to by it.
                flag += 2;
                break;
            case 'R' : //Descends directories recursively, changing the ownership for each file.
                flag += 4;
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
    }

    char option = 0;

    checkArguments(argc, argv, &option);

    return 0;
}
