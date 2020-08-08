/* statinfo.c - demonstrates using stat() 
 *              to obtain file information
 */

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>

void show_stat(char *, struct stat *);

int main(int argc, char ** argv)
{
    if (argc > 1) 
    {
        struct stat info;
        if (stat(argv[1], &info) != -1) {
            show_stat(argv[1], &info);
            return 0;
        }
        else perror(argv[1]);
    }
    return 1;
}

void show_stat(char * filename, struct stat * statp)
{   // displays some info from stat in a name=value format
    printf("   mode: %o\n",   statp->st_mode);         /* type and mode */
    printf("  links: %d\n",   statp->st_nlink);        /* # links       */
    printf("   user: %d\n",   statp->st_uid);          /* user id       */
    printf("  group: %d\n",   statp->st_gid);          /* group id      */
    printf("   size: %lld\n", statp->st_size);         /* file size     */
    printf("modtime: %ld\n",  statp->st_mtime);        /* modified      */
    printf("   name: %s\n",   filename );              /* filename      */
}
