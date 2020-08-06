/* who2@macOS.c  - use getutxent() instead of reading /etc/utmp 
 *                 which was deprecated
 *               - suppresses empty records
 *               - formats time nicely
 */
#include	<stdio.h>
#include	<utmpx.h>
#include	<fcntl.h>
#include	<unistd.h>
#include	<stdlib.h>
#include	<string.h>

#define	SHOWHOST	/* include remote machine on output */

void show_time(long);
void show_info(struct utmpx *);

int main()
{
	struct utmpx	*utmpxp;/* read info into here       */

	if ( (utmpxp = getutxent()) == NULL ){
		perror( UTMPX_FILE );	/* UTMPX_FILE is in utmpx.h    */
		exit(1);
	}

	while ((utmpxp = getutxent()))
		show_info(utmpxp);
	endutxent();
        return 0;			/* went ok */
}

/*
 *  show info()
 *	displays contents of the utmpx struct in human readable form
 *	*note* these sizes should not be hardwired
 */
void show_info( struct utmpx *utmpxp )
{
        if (utmpxp->ut_type != USER_PROCESS)
                return ;
	printf("%-8.8s", utmpxp->ut_user);	/* the logname	*/
	printf(" ");				/* a space	*/
	printf("%-8.8s", utmpxp->ut_line);	/* the tty	*/
	printf(" ");				/* a space	*/
    show_time(utmpxp->ut_tv.tv_sec);
	printf(" ");				/* a space	*/
#ifdef	SHOWHOST
	if (strlen(utmpxp->ut_host) != 0) {
            printf("(%s)", utmpxp->ut_host);	/* the host	*/
        }
#endif
	printf("\n");				/* newline	*/
}

void show_time(long timeval)
/*
 *      displays time in a format fit for human consumption
 *      uses ctime to build a string then picks parts out of it
 *      Note: %12.12s prints a string 12 chars wide and LIMITS
 *      it to 12chars.
 */
{
        char    *cp;                    /* to hold address of time      */

        cp = ctime(&timeval);           /* convert time to string       */
                                        /* string looks like            */
                                        /* Mon Feb  4 00:46:40 EST 1991 */
                                        /* 0123456789012345.            */
        printf("%12.12s", cp+4 );       /* pick 12 chars from pos 4     */
}
