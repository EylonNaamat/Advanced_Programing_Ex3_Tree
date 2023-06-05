//
// Created by eylon on 6/1/23.
//
#define _XOPEN_SOURCE 600 /* Get nftw() */
#include <ftw.h>
#include <stdio.h>        /* Standard I/O functions */
#include <stdlib.h>       /* Prototypes of commonly used library functions,
                             plus EXIT_SUCCESS and EXIT_FAILURE constants */
#include <string.h>       /* Commonly used string-handling functions */
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <dirent.h>
#define LINE_LEN 42

const char* getStringAfterLastSlash(const char* str) {
    int length = strlen(str);
    for (int i = length - 1; i >= 0; i--) {
        if (str[i] == '/') {
            return str + i + 1;
        }
    }
    return str;
}

int getNumberLength(long number) {
    int length = 0;

    // Handle the case when the number is 0 separately
    if (number == 0) {
        return 1;
    }

    // Count the number of divisions by 10
    while (number != 0) {
        number /= 10;
        length++;
    }

    return length;
}

int printFileDetails(const char *filepath, const struct stat *sb, int type, struct FTW *ftwb) {
    if(filepath[2] == '.' && strlen(filepath) != 1){
        return 0;
    }
    if (type == FTW_NS) {                  /* Could not stat() file */
        printf("?");
    } else {
        printf(" %*s", 4 * ftwb->level, " ");
        if(type == FTW_D){
            printf("└── ");
        }else if(type == FTW_F){
            printf("├── ");
        }else{
            printf("| ");
        }
    }
    struct stat fileStat;

    // Retrieve file information using stat()
    if (stat(filepath, &fileStat) == -1) {
        return -1;
    }
    // Permissions
    printf((S_ISDIR(fileStat.st_mode)) ? "[d" : "[-");
    printf((fileStat.st_mode & S_IRUSR) ? "r" : "-");
    printf((fileStat.st_mode & S_IWUSR) ? "w" : "-");
    printf((fileStat.st_mode & S_IXUSR) ? "x" : "-");
    printf((fileStat.st_mode & S_IRGRP) ? "r" : "-");
    printf((fileStat.st_mode & S_IWGRP) ? "w" : "-");
    printf((fileStat.st_mode & S_IXGRP) ? "x" : "-");
    printf((fileStat.st_mode & S_IROTH) ? "r" : "-");
    printf((fileStat.st_mode & S_IWOTH) ? "w" : "-");
    printf((fileStat.st_mode & S_IXOTH) ? "x" : "-");


    // Owner and group
    struct passwd *owner = getpwuid(fileStat.st_uid);
    struct group *group = getgrgid(fileStat.st_gid);
    printf(" %s    %s", owner->pw_name, group->gr_name);

    int curr_char_used_in_line = 12 + strlen(owner->pw_name) + strlen(group->gr_name) + 5;
    int size_len = getNumberLength((long long)fileStat.st_size);
    for(int i = 0; i < (LINE_LEN - curr_char_used_in_line - size_len); ++i){
        printf(" ");
    }

    // File size
    printf("%lld", (long long)fileStat.st_size);
    printf("]  ");

    // File name
    const char* name = getStringAfterLastSlash(filepath);
    printf("%s\n", name);

    return 0;
}

int main() {
    int flags = 0;
    printf(".\n");

    // Traverse the directory tree starting from the current directory (".")
    nftw(".", printFileDetails, 10, flags);

    return 0;
}