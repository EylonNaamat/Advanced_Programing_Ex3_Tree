#include <stdio.h>
#include <string.h>
#include <dirent.h>

void printAfterLastSlash(const char *str) {
    const char *lastSlash = strrchr(str, '/');
    if (lastSlash != NULL) {
        printf("Characters after the last '/': %s\n", lastSlash + 1);
    }
}
int countItemsInFolder(const char* folderPath) {
    int count = 0;
    DIR* directory = opendir(folderPath);
    if (directory == NULL) {
        perror("Failed to open directory");
        return -1;
    }

    struct dirent* entry;
    while ((entry = readdir(directory)) != NULL) {
        if (entry->d_type == DT_REG) {  // Only count regular files
            count++;
        }
    }

    closedir(directory);
    return count;
}

int main() {
    const char *str = ".";
    printf("Original string: %s\n", str);
//    printAfterLastSlash(str);
    printf("%d\n", countItemsInFolder(str));

    return 0;
}
