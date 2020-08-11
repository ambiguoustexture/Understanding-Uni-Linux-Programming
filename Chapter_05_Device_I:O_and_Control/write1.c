/* write0.c
 *
 *	purpose: send messages to another terminal
 *	 method: open the other terminal for output then 
 *	         copy from stdin to that terminal
 *	  shows: a terminal is just a file supporting regular i/o
 *	  usage: write0 ttyname
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <utmpx.h>

char * get_tty(char *);

int main(int argc, char ** argv) {
    int fd;
    char buf[BUFSIZ];

    // check args
    if (argc != 2) {
        fprintf(stderr,"usage: write1 [tty]\n");
        exit(1);
    }
    
    char * tty_for_user = get_tty( argv[1] );
    if (tty_for_user == NULL) return 1;

    sprintf(buf, "/dev/%s", tty_for_user);
    fd = open(buf, O_WRONLY);
    if (fd == -1) perror(buf); exit(1);

    while (fgets(buf, BUFSIZ, stdin) != NULL) {
        if (write(fd, buf, strlen(buf)) == -1) break;
    }
    close(fd);
    return 0;
}

char * get_tty(char * logname) {
/*
 * purpose: find the tty at which 'logname' is logged in
 * returns: a string or NULL if not logged in
 *  errors: does not handle multiple logins 
 */
    static struct utmpx utrec;
    int	      utfd;
    int	      namelen = sizeof(utrec.ut_user);
    char      * retval = NULL;

    if ((utfd = open(UTMPX_FILE, O_RDONLY)) == -1) return NULL;
    
    while(read(utfd, &utrec, sizeof(utrec)) == sizeof(utrec)) {
        if (strncmp(logname, utrec.ut_user, namelen) == 0) {
            retval = utrec.ut_line;
            break;
        }
    }
    close(utfd);
    return retval;
}
