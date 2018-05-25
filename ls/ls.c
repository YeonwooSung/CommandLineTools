#include "ls.h"
#include <regex.h>
#include <errno.h>
#include <time.h>

/**
 * This function checks if the names of a file doesnt start with '.'
 *
 * It filters out contents that start with a '.' so that the scandir function will not return them.
 *
 * @param entry the content to be checked
 * @return 0 is the file starts with a '.', and otherwise 1
 */
int filter(const struct dirent* entry) {

    if(entry->d_name[0] == '.') { //if first char is a '.'
        return a_flag ? 1 : 0; //If the a_flag is non 0, returns 1. Otherwise, returns 0.
    }

    return 1;
}

/**
 * Gets scandir to sort the contents by time.
 *
 * If the t_flag is active, this function will create two stat structs for
 * the two dirent structs passed in, and will return the difference of their sizes in bytes.
 *
 * This sorts the directory's contents by size, insttead of alphabetically.
 *
 *
 * @param entry1 the first entry to be compared
 * @param entry2 the second entry to be compared
 */
int t_compare(const struct dirent** entry1, const struct dirent** entry2) {

    struct stat one, two;
    int comparator = 0;

    //Get the whole file paths that are related to the current directory
    char* name1 = getFileName((*entry1)->d_name);
    char* name2 = getFileName((*entry2)->d_name);

    //if can stat these files, compare their sizes
    if((lstat(name1, &one) == 0) && (lstat(name2, &two) == 0)) {

        //If the r flag is activated as well, sort ascending based on the file size. Otherwise, sort by descending order.
        comparator = (!r_flag) ? two.st_mtime - one.st_mtime : one.st_mtime - two.st_mtime;
    }

    //Free the allocated memory.
    free(name1);
    free(name2);

    return comparator;
}

/**
 * Gets scandir to sort the contents by size in bytes
 *
 * If the s_flag is active, this function will
 * create two stat structs for the two dirent structs passed in, and will return the
 * difference of their sizes in bytes. This sorts the directory's contents by size, insttead
 * of alphabetically.
 *
 * @param entry1 the first entry to be compared
 * @param entry2 the second entry to be compared
 */
int S_compare(const struct dirent** entry1, const struct dirent** entry2) {

    struct stat one, two;
    int comparator = 0;

    //get whole path relative to current directory
    char* name1 = getFileName((*entry1)->d_name);
    char* name2 = getFileName((*entry2)->d_name);

    //if can stat these files, compare their sizes
    if((lstat(name1, &one) == 0) && (lstat(name2, &two) == 0)) {

        //If the r flag is activated as well, sort ascending based on file size. Otherwise, sort by descending order.
        comparator = !r_flag ? two.st_size - one.st_size : one.st_size - two.st_size;
    }

    //Free the allocated memory.
    free(name1);
    free(name2);

    return comparator;
}

/**
 * The comparison function used with the function scandir.
 *
 * This function checks which flags are active to deside how to compare.
 * It utilises the strcasecmp function to check whether the first string is less than, equal to,
 * or greater than the second string.
 *
 * @param entry1 the first entry to be compared
 * @param entry2 the second entry to be compared
 * @return an integer -1, 0, or 1 if the first string was less than, equal to or greater than the second.
 */
int compare(const struct dirent** entry1, const struct dirent** entry2) {

    if(t_flag) { //check the value of t_flag to sort by time
        return t_compare(entry1, entry2); //Returns the value that the function t_compare returns.
    }

    if(S_flag) { //check the value of S_flag to sort by size in bytes.
        return S_compare(entry1, entry2); //Returns the value that the function S_compare returns.
    }

    if(!r_flag) { //check the value of r_flag to sort alphabetically by ascending order.
        return strcasecmp((*entry1)->d_name,(*entry2)->d_name);
    }

    //sort by descending order
    return strcasecmp((*entry2)->d_name,(*entry1)->d_name);

}

/**
 * Sets the flags for this particular arg
 *
 * This method gets called whenever a '-' is seen,
 * indicating a flag. It also iterates through the flag
 * if there is more than one char coming after the '-',
 * to recognise args like '-Sn,' and subsequently set the corresponding flags.
 *
 * @param arg the current flag being analysed
 */
void setFlags(char* arg) {

    //allows for args like '-Sn' instead of just '-S -n'
    for (unsigned int i = 1; i < strlen(arg); i++) {

        // The switch statement to check the command line options.
        switch(arg[i]) {
            case 'l':
                l_flag = 1;
                break;
            case 'S':
                S_flag = 1;
                break;
            case 'R':
                R_flag = 1;
                break;
            case 't':
                t_flag = 1;
                break;
            case 'i':
                i_flag = 1;
                break;
            case 'n':
                n_flag = 1;
                break;
            case 'r':
                r_flag = 1;
                break;
            case 'a':
                a_flag = 1;
                break;

                //unrecognised flag
            default: {
                errno = EINVAL;
                perror(USAGE_MESSAGE);
                exit(0);
            }
        } //the switch statement ends

    } //the for loop ends

}

/**
 * Looks at the different args passed into the program
 *
 * This method looks at all other arguments passed into the program
 * besides the first one (program name) to ascertain which flags are present.
 * It checks for the '-' char to indicate  a flag, as well as for a directory name.
 *
 * @param argc the total number of args
 * @param argv the list of command line args passed to main
 */
int checkArgs(int argc, char** argv) {

    paths = malloc(2);
    paths[0] = ".";
    int numPaths = 0;
    int size = 0;

    for(int i = 1; i < argc; i++) { //use the for loop to check all command line arguments.

        char* arg = argv[i];

        //if flag
        if(arg[0] != '-') {

            size += strlen(arg) +1;
            paths = realloc(paths, size);

            if(path != NULL) { //check if the path is not NULL.
                paths[numPaths++] = arg;
            }

        } else {
            setFlags(arg);
        }
    }
    return (numPaths != 0) ? numPaths : 1;
}

/**
 * Frees all entries within the directory global variable
 *
 * This function calls free on each different struct dirent* within the directory dirent**.
 *
 * @param directory the directory to be freed
 * @param numOfFiles the number of files within the directory to also be freed.
 */
void freeDirectory(struct dirent** directory, int numOfFiles) {

    for(int i = 0; i < numOfFiles; i++) {
        free(directory[i]);
    }

    free(directory);
}
