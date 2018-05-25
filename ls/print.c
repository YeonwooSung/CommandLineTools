//
// Created by YeonwooSung on 2018. 5. 25..
//

#include "ls.h"
#include <time.h>


/**
 * This function gains the relative path for the file passed in
 *
 * Appends the file name to the path name stored in the global variable,
 * so that lstat will return the stat struct for the correct file.
 *
 * @param fileName the name of the file
 * @return the relative path based on the current directory
 */
char* getFileName(const char* fileName) {

    char* pathName = malloc( (strlen(path) + 2 + strlen(fileName)) * sizeof(char) );

    strcpy(pathName, path);

    //Add the path name of the corresponding file to the string "pathName".
    strcat(pathName, "/");
    strcat(pathName, fileName);
    strcat(pathName, "\0");

    return pathName;
}


/**
 * Prints the time last modified for the given stat struct
 *
 * Loads the modified time field into the ctime function, which produces a formatted string which is subsequently printed.
 * The function ctime() defaults to having a new line character at the end however.
 * So this is replaced with a terminating character before printing.
 *
 * @param entry the stat struct representing the current entry in the directory
 */
void printTime(struct stat* entry) {

    char* time = ctime(&(entry->st_mtime));
    int length = strlen(time)-1;

    //replace new line char with terminating char.
    if(time[length] == '\n') {
        time[length] = 0;
    }

    printf("%s ", time);
}


/**
 * Prints the permissions for the given stat struct
 *
 * This function performs bitwise operations to ascertain the different permissions,
 * and subsequently prints out the results of said operation.
 *
 * @param[in] entry the stat struct representing the current entry in the directory
 */
void print_permissions(struct stat* entry) {

    if (S_ISDIR(entry->st_mode)) {
        printf("d");
    } else if (S_ISLNK(entry->st_mode)) {
        printf("l");
    } else {
        printf("-");
    }

    // Print out the user's file permission
    printf( (entry->st_mode & S_IRUSR) ? "r" : "-");
    printf( (entry->st_mode & S_IWUSR) ? "w" : "-");
    printf( (entry->st_mode & S_IXUSR) ? "x" : "-");

    // Print out the group's file permission
    printf( (entry->st_mode & S_IRGRP) ? "r" : "-");
    printf( (entry->st_mode & S_IWGRP) ? "w" : "-");
    printf( (entry->st_mode & S_IXGRP) ? "x" : "-");

    // Print out the other's file permission
    printf( (entry->st_mode & S_IROTH) ? "r" : "-");
    printf( (entry->st_mode & S_IWOTH) ? "w" : "-");
    printf( (entry->st_mode & S_IXOTH) ? "x " : "- ");
}

/**
 * Prints the inode number of the given stat struct
 *
 * @param entry the stat struct representing the current entry in the directory
 */
void print_inode(struct stat* entry) {
    printf("%-7lu", entry->st_ino);
}


/**
 * This is a function for both the -n and -l flags
 *
 * This function prints out the long version of the file passed into it.
 * This includes permissions, user and group names / ids, the modification date and the size in bytes.
 *
 * @param entry the file to print the long information for.
 */
void list_l(struct stat* entry) {

    print_permissions(entry);
    printf("%1lu ", entry->st_nlink);

    //print ids
    if(n_flag) {

        printf("%5u ", entry->st_uid);
        printf("%5u ", entry->st_gid);

    } else { //print names

        struct group* group = getgrgid(entry->st_gid);
        struct passwd* user = getpwuid(entry->st_uid);
        user != NULL ? printf("%4s ", user->pw_name) : perror("Error retrieving user name");
        group != NULL ? printf("%7s ", group->gr_name) : perror("Error retrieving group name");

    }

    printf("%6lu ", entry->st_size);
    printTime(entry);

}

/**
 * Prints out the contents for each sub-directory.
 *
 * Implementation of the -R flag. This recursively calls the process_dir
 * function for each sub-directory within the specified path.
 *
 * @param[in] sub_dirs the char** of sub directories to print the contents of.
 */
void print_sub_dirs(char** sub_dirs, int num_sub_dirs) {

    char* original_path = malloc(strlen(path) + 1);
    strcpy(original_path, path);

    //for each sub directory
    for(int i = 0; i < num_sub_dirs; i++) {

        path = realloc(path, strlen(sub_dirs[i]) + 1);

        //if realloc didn't fail
        if(path != NULL) {

            strcpy(path, sub_dirs[i]);

            printf(BLUE "\n%s\n" RESET, path);

            processDir(sub_dirs[i]);

            free(sub_dirs[i]);
        }
    }

    //put path to original value
    path = realloc(path, strlen(original_path) + 1);

    if (path != NULL) {
        strcpy(path, original_path);
    }

    free(original_path);
}

/**
 * Prints the file name according to its type
 *
 * This function prints the file name in a different colour depending on whether or not it is a directory, or link.
 *
 * @param pathname the path to the current file being printed
 * @param buffer the current member of the directory to print the name of
 * @param entry the dirent struct representing the current file
 */
void print_file_name(char* pathname, struct stat* buffer, struct dirent* entry) {

    char* buf; //the path to where a link points to
    int FILE_SIZE = 255;

    //if directory print in BLUE
    if(S_ISDIR(buffer->st_mode)) {

        printf(BLUE "%s\n" RESET, entry->d_name);

    } else if(S_ISLNK(buffer->st_mode)) {

        buf = malloc(FILE_SIZE);
        int result = readlink(pathname, buf, FILE_SIZE);

        if(result) {
            printf(GREEN "%s -> %s\n" RESET, entry->d_name, buf);
        }

        free(buf);

    } else {

        printf("%s\n", entry->d_name);

    }
}

/**
 * This function prints out the directory's contents based on the respective flags
 *
 * Uses lstat to gather information of each member in a directory,
 * and then prints out its information according to the appropriate flags.
 *
 * @param directory the directory to print the contents of
 * @param files the number of files in the directory
 */
void printDir(struct dirent** directory, int files) {

    char** sub_dirs =malloc(0);
    int num_sub_dirs = 0;
    int size = 0;

    //list names of directory's contents based on flags, if applicable
    for(int i = 0; i < files; i++) {

        struct dirent* entry = directory[i];
        struct stat buffer;
        char* path_name = getFileName(entry->d_name);

        //no error
        if (lstat(path_name, &buffer) == 0) {

            if (i_flag) {
                print_inode(&buffer);
            }

            if (l_flag || n_flag) {
                list_l(&buffer);
            }

            print_file_name(path_name, &buffer, entry);

        } else {
            fprintf(stderr, USAGE_MESSAGE);
        }

        //check if a sub directory
        if(S_ISDIR(buffer.st_mode) && R_flag && strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {

            size += strlen(path_name) + 1;
            sub_dirs = realloc(sub_dirs, size);

            if (sub_dirs != NULL) {
                sub_dirs[num_sub_dirs++] = path_name;
            }
        } else {
            free(path_name);
        }

    }

    //print sub-directories' contents if R is active
    if(R_flag) {

        print_sub_dirs(sub_dirs, num_sub_dirs);

    }

    free(sub_dirs);
}

