/* who1@macOS.c  - use getutxent() instead of reading /etc/utmp 
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
	printf("%-8.8s", utmpxp->ut_user);	/* the logname	*/
	printf(" ");				/* a space	*/
	printf("%-8.8s", utmpxp->ut_line);	/* the tty	*/
	printf(" ");				/* a space	*/
	printf("%10ld", utmpxp->ut_tv.tv_sec);	/* login time	*/
	printf(" ");				/* a space	*/
#ifdef	SHOWHOST
	if (strlen(utmpxp->ut_host) != 0) {
            printf("(%s)", utmpxp->ut_host);	/* the host	*/
        }
#endif
	printf("\n");				/* newline	*/
}
