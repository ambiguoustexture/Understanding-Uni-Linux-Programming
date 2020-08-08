/* filesize.c - prints size of passwd file
 */

#include <stdio.h>
#include <sys/stat.h>

int main()
{
    struct stat info;
    if (!stat("/etc/passwd", &info)) perror("/etc/passwd");
    else printf(" The size of /etc/passwd is %lld\n", info.st_size);
    return 0;
}
