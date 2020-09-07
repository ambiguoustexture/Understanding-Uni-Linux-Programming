/* play_again4.c
 *      purpose: ask if user wants another transaction
 *       method: set tty int chr-by-chr, no-echo mode
 *               set tty into no-delay mode
 *               read char, return result
 *               resets char, return result
 *      returns: 0=>yes, 1=>no, 2=>timeout
 *       better: rest terminal mode on Interrupt
 */
#include <stdio.h>
#include <termios.h>
#include <fcntl.h>
#include <string.h>
#include <signal.h>

#include <unistd.h>
#include <ctype.h>
#include <stdlib.h>

#define ASK       "Do you want another transaction"
#define TRIES     3                                 // max tries
#define SLEEPTIME 2                                 // time per try
#define BEEP      putchar('\a')                     // alert user

void tty_mode(int how);
void set_cr_noecho_mode();
void set_no_delay_mode();
int get_ok_char();
int  get_response(char *, int);
void ctrl_c_handler(int);

int main() 
{
    int response;
    void ctrl_c_handler(int);

    tty_mode(0);                         // save tty mode
    set_cr_noecho_mode();				 // set -icanon, -echo
    set_no_delay_mode();                 // noinput => EOF
    signal(SIGINT, ctrl_c_handler);      // handle INT
    signal(SIGQUIT, SIG_IGN);            // ignore QUIT signals
    response = get_response(ASK, TRIES); // get some answer
    tty_mode(1);						 // restore tty mode
    return response;
}

// skip over non-legal chars and return y, Y, n, N or EOF
int get_ok_char()
{
    int c;
    while ((c = getchar()) != EOF && strchr("yYnN", c) == NULL)
        ;
    return c;
}

int get_response(char *question, int maxtries)
/*
 * purpose: ask a question and wait for a y/n answer
 *  method: use aetchar and complain about non y/n answer
 * returns: 0=>yes, 1=>no, 2=>timeout
 */
{
    int input;

    printf("%s (y/n)?", question);      // ask
    fflush(stdout);                     // force output
    while (1) {
        sleep(SLEEPTIME);               // wait a bit
        input = tolower(get_ok_char()); // get next chr
        if (input == 'y') 
            return 0;
        if (input == 'n')
            return 1;
        if (maxtries-- == 0)            // outatime?
            return 2;                   // sayno
        BEEP;
    }
}

void set_cr_noecho_mode()
/*
 * purpose: put file descriptor 0 into chr-by-chr mode and noecho mode
 *  method: use bits in termios
 */
{
    struct termios ttystate;

    tcgetattr(0, &ttystate);             // read curr, settin
    ttystate.c_lflag       &= -ICANON;   // no buffering
    ttystate.c_lflag       &= -ECHO;     // no echo either
    ttystate.c_cc[VMIN]    =  1;         // get 1 char at a time
    tcsetattr(0, TCSANOW, &ttystate);    // install settings
}

void set_no_delay_mode()
/*
 * Purpose: put file descriptor 0 into no-delay mode
 *  method: use fcntl to set bits
 *   notes: tcsetattr() will do something similar, but it is complicated
 */
{
    int termflags;

    termflags  = fcntl(0, F_GETFL);       // read curr. settings
    termflags |= O_NDELAY;                // flip on nodelay bit
    fcntl(0, F_SETFL, termflags);         // and install 'em
}


// how == 0 => save current node, how == 1 => restore node
// this version handles termios and fcntl flags
void tty_mode(int how)
{
	static struct termios original_mode;
    static int            original_flags;
	if (how == 0) {
		tcgetattr(0, &original_mode);
        original_flags = fcntl(0, F_GETFL);
    }
	else {
		tcsetattr(0, TCSANOW, &original_mode);
        fcntl(0, F_SETFL, original_flags);
    }
}

void ctrl_c_handler(int signum)
/*
 * purpose: called if SIGINT is detected
 *  action: reset tty and scram
 */
{
    tty_mode(1);
    exit(2);
}
