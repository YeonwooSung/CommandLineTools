//
// Created by YeonwooSung on 2018. 6. 23..
//

#include <stdio.h>
#include <sys/types.h>
#include <pwd.h>

#define USAGE_MESSAGE "usage: chown [-fhv] [-R [-H | -L | -P]] owner[:group] file ...\n\tchown [-fhv] [-R [-H | -L | -P]] :group file ..."
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

int main(int argc, char *argv[]) {
    if (argc == 1) {
        fprintf(stderr, USAGE_MESSAGE);
    }
}
