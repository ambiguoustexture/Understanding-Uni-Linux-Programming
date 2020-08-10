/* pwd1.c a simplified version of pwd
 *     starts in current directory
 *     and recursively climbs up to root of filesystem, 
 *     prints top part then prints current part
 *
 *     uses readdir() to get info about each thing 
 */

#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h> 
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

ino_t get_inode(char *);
void  get_dirname(ino_t, char *, int);
void  pwd(ino_t);

char currentdir[2] = ".";
char parrentdir[3] = "..";

int main()
{
    pwd(get_inode(currentdir));
    putchar('\n');
    return 0;
}

void pwd(ino_t currentinode)
{
    ino_t parrentinode;
    char  dirname[BUFSIZ];
    if (get_inode(parrentdir) != currentinode) 
    {
        chdir(parrentdir);
        get_dirname(currentinode, dirname, BUFSIZ);
        parrentinode = get_inode(currentdir);
        pwd(parrentinode);
        printf("/%s", dirname);
    } 
}

ino_t get_inode(char * fname)
{
    struct stat info;
    if (stat(fname, &info) == -1)
    {
        fprintf(stderr, "Cannot stat ");
        perror(fname);
        exit(1);
    }
    return info.st_ino;
}

void get_dirname(ino_t currentinode, char * dirname, int buflen)
{
    DIR    * dirp = opendir(currentdir);
    if (!dirp)
    {
        perror(currentdir); exit(1);
    }
    struct dirent * direntp;
    while ((direntp = readdir(dirp)))
    {
        if (direntp->d_ino == currentinode)
        {
            strncpy(dirname, direntp->d_name, buflen);
            dirname[buflen - 1] = '\0';
            closedir(dirp);
            return ;
        }
    }

    // if exit here, the call stack will be lost. 
    // fprintf(stderr, "error looking for inum %llu\n", currentinode);
    // exit(1);
}
