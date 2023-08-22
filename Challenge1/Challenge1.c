#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <ctype.h>
#include <string.h>
//Kai Cottrell - CS 3100 
//Part 4
int stringHasNumber(const char* ndx) {
    while (*ndx) {
        if (isdigit(*ndx))
            return 1;
        ndx++;
    }
    return 0;
}

int main() {
    //Part 1 and 2
    int pid;
    printf("Hello From Linux Land \n");
    pid = getpid();
    printf("PID: %d\n", pid);

    //Part 3
    DIR *pDir;
    pDir = opendir("/proc");
    struct dirent *entry;
    if (pDir == NULL) {
        perror("unable to read Directory");
        return 1;
    }
    while ((entry = readdir(pDir)) != NULL) {
        if (stringHasNumber(entry->d_name)) { // if directory has a number it will be printed 
            FILE* fp; //file pointer
            char cmdline_cmd[256];
            char cmdline_path[270];  // Increased buffer size
            snprintf(cmdline_path, sizeof(cmdline_path), "/proc/%s/cmdline", entry->d_name); //the path to the cmdline is created with a limited size as denoted by the 2nd argument
            fp = fopen(cmdline_path, "r"); //open file with the given cmdline path
            if (fp) { // if the the file in the directory entry is found and opened
                if (fgets(cmdline_cmd, sizeof(cmdline_cmd), fp) != NULL) { //file get string (where the line will be stored, number of characters to be read, file stream source)
                    cmdline_cmd[sizeof(cmdline_cmd) - 1] = '\0';  // Ensure null-termination - setting the last character of the string to the null charcter
                    printf("%s   %s\n",entry->d_name, cmdline_cmd);
                }
                fclose(fp);
            }
        }
    }
    closedir(pDir);

    return 0;
}
