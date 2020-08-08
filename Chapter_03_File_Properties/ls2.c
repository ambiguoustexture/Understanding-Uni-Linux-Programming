/* ls1.c
 *     purpose    list contents of directory or directories
 *     action     if no args, use .  else list files in args
 *     note       uses stat and pwd.h and grp.h
 */

/*
 * open directory
 * +-> read directory    - end of directoey ? -+
 * |__ display directory                       |
 *     close directory   <---------------------+
 */

#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>            // where readdir is

#include <sys/stat.h>          // where stat() is
#include <time.h>              // where ctime() is
#include <string.h>            // where strcpy() is
#include <pwd.h>               // where getpwuid() is
#include <grp.h>               // where getgrgid() is

void ls(char *);
void dotstat(char *);
void show_file_info(char *, struct stat *);
void show_mode(int, char *);
char * uid_to_name(uid_t);
char * gid_to_name(gid_t);

int main(int argc, char ** argv) 
{
    if (argc == 1) 
    {   
        char current[2] = "."; // silent the warning on
        ls(current);           // "conversion from string literal to 'char *'"
    }
    else while (--argc) 
    {
        printf("%s:\n", *++argv); ls(*argv);
    }
    return 0;
}

void ls(char * where) 
{   // list files in directory called dirname
    DIR * dirp;                // the directory
    struct dirent * direntp;   // each entry
    if ((dirp = opendir(where)) == NULL) 
        fprintf(stderr, "ls1: cannot open %s\n", where);
    else 
    {
        while ((direntp = readdir(dirp))) 
            dotstat(direntp->d_name);
        closedir(dirp);
    }
}

void dotstat(char * where)
{
    struct stat info;
    if (stat(where, &info) == -1) perror(where);
    else show_file_info(where, &info);
}

void show_file_info(char * where, struct stat * infop)
{   // display file info stored in struct at *info_p
    char mode[11];
    show_mode(infop->st_mode, mode);
    printf("%s",    mode);
    printf("%4d",   (int)infop->st_nlink);
    printf("%-8s",  uid_to_name(infop->st_uid));
    printf("%-8s" , gid_to_name(infop->st_gid));
    printf("%.12s", 4 + ctime(&infop->st_mtimespec.tv_nsec));
    printf("%s\n",  where);
}

void show_mode(int st_mode, char * mode) 
{
    strcpy(mode, "----------");           // default=no perms
    // file type
    if (S_ISDIR(st_mode))  mode[0] = 'd'; // directory
    if (S_ISCHR(st_mode))  mode[0] = 'c'; // char devices
    if (S_ISBLK(st_mode))  mode[0] = 'b'; // block device
    // file permissions for user
    if (st_mode & S_IRUSR) mode[1] = 'r';
    if (st_mode & S_IWUSR) mode[2] = 'w';
    if (st_mode & S_IXUSR) mode[3] = 'x';
    // file permissions for group
    if (st_mode & S_IRGRP) mode[4] = 'r';
    if (st_mode & S_IRGRP) mode[5] = 'w';
    if (st_mode & S_IRGRP) mode[6] = 'x';
    // file permissions for other
    if (st_mode & S_IROTH) mode[7] = 'r';
    if (st_mode & S_IROTH) mode[8] = 'w';
    if (st_mode & S_IROTH) mode[9] = 'x';
}

char * uid_to_name(uid_t uid) 
{   // returns pointer to username associated with uid, uses getpw()
    struct passwd * passwdp;
    static char restrictstr[10];
    if (!(passwdp = getpwuid(uid)))
    {
        sprintf(restrictstr, "%d", uid);
        return restrictstr;
    }
    else return passwdp->pw_name;
}

char * gid_to_name(gid_t gid)
{   // returns pointer to group number gid, used getgrgid()
    struct group * groupp;
    static char restrictstr[10];
    if (!(groupp = getgrgid(gid)))
    {
        sprintf(restrictstr, "%d", gid);
        return restrictstr;
    }
    else return groupp->gr_name;
}
