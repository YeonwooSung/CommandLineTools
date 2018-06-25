//
// Created by YeonwooSung on 2018. 6. 23..
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>

#define USAGE_MESSAGE "usage: chown [-cfhv] [-R] owner[:group] file ...\n\tchown [-cfhv] [-R] :group file ..."
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

void checkArguments(int argc, char *argv[], char *optFlag) {
    int opt;

    while ((opt = getopt(argc, argv, "cfhv:R:"))) {

        switch (opt) {
            case 'c' :
                break;
            case 'f' :
                break;
            case 'h' :
                break;
            case 'v' :
                break;
            case 'R' :
                break;
            default:
                fprintf(stderr, USAGE_MESSAGE);
                exit(1);
        }//switch statement ends

    }//while loop ends
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

    return 0;
}
