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
 *@param entry1 the first entry to be compared
 *@param entry2 the second entry to be compared
 */
int t_compare(const struct dirent** entry1, const struct dirent** entry2) {

    struct stat one, two;
    int comparator = 0;

    //Get the whole file paths that are related to the current directory
    char* name1 = get_file_name((*entry1)->d_name);
    char* name2 = get_file_name((*entry2)->d_name);

    //if can stat these files, compare their sizes
    if((lstat(name1, &one) == 0) && (lstat(name2, &two) == 0)) {

        //If the r flag activate as well, sort ascending based on the file size. Otherwise, sort by descending order.
        comparator = (!r_flag) ? two.st_mtime - one.st_mtime : one.st_mtime - two.st_mtime;
    }

    //Free the allocated memory.
    free(name1);
    free(name2);

    return comparator;
}

/*Gets scandir to sort the contents by size in bytes
 *
 *If the s_flag is active, this function will
 *create two stat structs for the two dirent structs passed in, and will return the
 *difference of their sizes in bytes. This sorts the directory's contents by size, insttead
 *of alphabetically.
 *@param[in] entry1 the first entry to be compared
 *@param[in] entry2 the second entry to be compared
 */
int S_compare(const struct dirent** entry1, const struct dirent** entry2)
{
    struct stat one, two;
    int compare = 0;
    //get whole path relative to current directory
    char* name1 = get_file_name((*entry1)->d_name);
    char* name2 = get_file_name((*entry2)->d_name);

    //if can stat these files, compare their sizes
    if((lstat(name1, &one) == 0) && (lstat(name2, &two) == 0))
    {
        //if r flag active as well, sort ascending based on file size
        //else descending
        compare = !r_flag ? two.st_size - one.st_size
                          : one.st_size - two.st_size;
    }
    free(name1);
    free(name2);
    return compare;
}

/**The comparison function used with scandir
 *
 *This function checks which flags are active to deside how to compare.
 *It utilises the strcasecmp function
 *to check whether the first string is less than, equal to,
 *or greater than the second string, and will return -1, 0, or 1
 *repectively.
 *@param[in] entry1 the first entry to be compared
 *@param[in] entry2 the second entry to be compared
 *@return an integer -1, 0, or 1 if the first string was less
 *than, equal to or greater than the second.
 */
int compare(const struct dirent** entry1, const struct dirent** entry2)
{
    //sort by time
    if(t_flag)
    {
        return t_compare(entry1, entry2);
    }
    //sort by size in bytes
    if(S_flag)
    {
        return S_compare(entry1, entry2);
    }

    //otherwise sort alphabetically (ascending)
    if(!r_flag) return strcasecmp((*entry1)->d_name,(*entry2)->d_name);

    //descending
    return strcasecmp((*entry2)->d_name,(*entry1)->d_name);

}

/**Sets the flags for this particular arg
 *
 *This method gets called whenever a '-' is seen,
 *indicating a flag. It also iterates through the flag
 *if there is more than one char coming after the '-',
 *to recognise args like '-Sn,' and subsequently set the corresponding
 *flags
 *@param[in] arg the current flag being analysed
 */
void set_flags(char* arg)
{
    //allows for args like '-Sn' instead of just '-S -n'
    for(unsigned int j = 1; j < strlen(arg); j++)
    {
        switch(arg[j])
        {
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
            default:
            {
                errno = EINVAL;
                perror("Invalid flag");
                exit(0);
            }
        }
    }
}

/**Looks at the different args passed into the program
 *
 *This method looks at all other arguments passed into the program
 *besides the first one (program name) to ascertain
 *which flags are present. It checks for the '-' char to indicate
 *a flag, as well as for a directory name.
 *@param[in] argc the total number of args
 *@param[in] argv the list of command line args passed to main
 */
int check_args(int argc, char** argv)
{
    paths = malloc(2);
    paths[0] = ".";
    int num_paths = 0;
    int size = 0;
    //for each command line arg
    for(int i = 1; i < argc; i++)
    {
        char* arg = argv[i];

        //if flag
        if(arg[0] == '-')
        {
            set_flags(arg);
        }
        else
        {
            size += strlen(arg) +1;
            paths = realloc(paths, size);
            if(path != NULL) paths[num_paths++] = arg;
        }
    }
    return num_paths == 0 ? 1 : num_paths;
}

/**Free all entries within the directory global variable
 *
 *This function calls free on each different struct dirent*
 *within the directory dirent**
 *@param[in] directory the directory to be freed
 *@param[in] num_files the number of files within the directory to also
 *be freed.
 */
void free_directory(struct dirent** directory, int num_files)
{
    for(int i = 0; i < num_files; i++)
    {
        free(directory[i]);
    }
    free(directory);
}
