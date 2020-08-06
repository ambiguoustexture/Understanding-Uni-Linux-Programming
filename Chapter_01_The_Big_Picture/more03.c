/* more03.c  - version 0.3 of more 
 * read and print 24 lines then pause for a few special commands
 *     feature of version 0.3: gets <q>|<ENTER>|<SPACE> without another <ENTER> behind
 *     method:                 uses tcgetattr() and tcsetattr()
 */

#include <stdio.h>
#include <stdlib.h>

#include <termios.h>
#include <unistd.h>

#define PAGELEN 24
#define LINELEN 512

void do_more(FILE *);
int  see_more(FILE *);

int main(int argc, char *argv[])
{
    struct termios termios_p;
    {
        // get attribute and store in termios_p
        tcgetattr(0, &termios_p);
        /* 
         * ICANON:     canon flag in c_lflag determines whether the terminal is operating
         * in canonical mode (ICANON set) 
         * or noncanonical mode (ICANON unset).
         * c_cc[VMIN]: minimum number of characters for noncanonical read
         */
        termios_p.c_lflag &= ~ICANON; termios_p.c_cc[VMIN] = 1;
        // set attribute which
        // TCSANOW: the change occurs immediately.
        tcsetattr(0, TCSANOW, &termios_p);
    }

    {
	    if (argc == 1) do_more(stdin);
	    else 
	    {
	        while (--argc) 
	        {
	            FILE * stream;
	            if ((stream = fopen(* ++argv, "r")) != NULL)
	            {
	                do_more(stream);
	                fclose(stream);
	            }
	            else exit(1);
	        }
	    }
    }

    tcsetattr(0, TCSANOW, &termios_p);
    
    return 0;
}

void do_more(FILE * stream) 
/*
 * read PAGELEN lines, then call see_more() for further instructions
 */
{
    FILE * stream_tty = fopen("/dev/tty", "r");
    if (stream_tty == NULL) exit(1);
    else 
    {
        char line[LINELEN];
        int count = 0;
        while (fgets(line, LINELEN, stream)) 
        {
            if (count == PAGELEN)
            {
                int response_tty = see_more(stream_tty);
                if (response_tty == 0) break;
                else count -= response_tty;
            }

            if ((fputs(line, stdout)) == EOF) exit(1);
            else count++;
        }
    }
}

int see_more(FILE * stream_tty) 
/*
 * print message, wait for response, return # of lines to advance
 * q means no, space means yes, CR means one line
 */
{
    printf("\033[7m more? \033[m");
    
    int option;
    while ((option = getc(stream_tty)) != EOF)
    {
        if (option == 'q')  return 0;
        if (option == ' ')  return PAGELEN;
        if (option == '\n') return 1;
    }
    return 0;
}
