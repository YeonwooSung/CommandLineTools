//
// Created by YeonwooSung on 2018. 5. 25..
//

#ifndef _LS_H
#define _LS_H

#define _XOPEN_SOURCE 700
#define BUFFER_SIZE 255

#define USAGE_MESSAGE "Usage: ./ls-program (-n -l -R -S -t -i -r -a) (File path)\n"

// To print out the colorful output messages.
#define BLUE "\x1b[34m"
#define GREEN "\x1b[32m"
#define RED "\x1b[31m"
#define RESET "\x1b[0m"

// The header files that I used for implementing "ls" command.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <dirent.h>
#include <sys/dir.h>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>
#include <sys/types.h>
#include <sys/stat.h>

char* path;
char** paths;

// To print info about permissions,group, user, modification, and size
int n_flag;
int l_flag;

// To print out sub-directories' contents recursively
int R_flag;

// To print out files based on size (ascending order)
int S_flag;

// To print based on time (ascending order)
int t_flag;

// To show the inode number
int i_flag;

// To print out the files with the reverse order
int r_flag;

// To show the hidden files
int a_flag;

// The prototype of functions in the print.c file.
char* getFileName(const char* name);
void printDir(struct dirent** dir, int files);

// The prototype of function in the ls.c file.
void freeDirectory(struct dirent** dir, int num_files);
int checkArgs(int argc, char** argv);

// The prototype of function in the main.c file.
void processDir(char* path);

#endif //_LS_H
