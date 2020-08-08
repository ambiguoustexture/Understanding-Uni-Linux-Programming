#include <stdio.h>
#include <sys/types.h>

#include <dirent.h>            // where readdir is

void ls(char *);

int main(int argc, char ** argv) 
{
    if (argc == 1) 
    {   
        // silent the warning on "conversion from string literal to 'char *'"
        char current[2] = ".";
        ls(current);
    }
    else while (--argc) 
    {
        printf("%s:\n", *++argv); ls(*argv);
    }
    return 0;
}

void ls(char * where) 
{
    DIR * dirp;                // the directory
    struct dirent * direntp;   // each entry
    if ((dirp = opendir(where)) == NULL) 
        fprintf(stderr, "ls1: cannot open %s\n", where);
    else 
    {
        while ((direntp = readdir(dirp))) 
            printf("%s\n", direntp->d_name);
        closedir(dirp);
    }
}
