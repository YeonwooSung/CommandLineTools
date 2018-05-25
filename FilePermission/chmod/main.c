//
// Created by YeonwooSung on 2018. 5. 25..
//

#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>

/**
 * The checkPermission function checks the new file permission.
 * The bitwise operator is used to combine all permission bits.
 *
 * @param fp the string for the new file permissions.
 * @return newMode the bits for the new file permissions.
 */
int checkPermission(char *fp) {
    int newMode = 0;

    for (int i = 0; i < 3; i++) {
        int newPermission = (*fp - '0');

        //If the value of newPermission is invalid, then exit the process.
        if (newPermission > 7 || newPermission < 0) {
            fprintf(stderr, "Wrong permission!");
            exit(1);
        }

        //Use the bitwise operator OR to combine all permission bits.
        newMode = newMode | newPermission;

        newMode << 3; //left shift to move bits.
        fp += 1;
    }

    return newMode;
}

/**
 * The main function of the chmod command.
 *
 * The aim of the chmod command is to change the file permissions of the given file.
 * I used the POSIX function chmod to implement this command.
 *
 * @param argc the number of command line arguments.
 * @param argv the values of command lien arguments.
 * @return 0 or -1 for success or failure.
 */
int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "invalid number of arguments");
        return -1;
    }

    int n = chmod(argv[1], checkPermission(argv[2])); //call the checkPermission function first to check the new file permission.

    if (n != 0) {
        perror("chmod failed\n");
        return -1;
    }

    return 0;
}
