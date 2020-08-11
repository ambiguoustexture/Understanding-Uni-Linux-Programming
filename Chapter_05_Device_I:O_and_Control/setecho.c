/* setecho.c
 *     usage:  setecho [y|n]
 *     shows:  how to read, change, reset tty attributes
 */

#include <stdio.h>
#include <stdlib.h>
#include <termios.h>

#define  oops(s,x) { perror(s); exit(x); }

int main(int argc, char ** argv) {
    struct termios info;

    if (argc == 1) exit(0);
    // get attribs
    if ((tcgetattr(0, &info)) == -1) oops("tcgettattr", 1);
    
    // turn on bit
    if (argv[1][0] == 'y') 
        info.c_lflag |= ECHO;
    // turn off bit
    else 
        info.c_lflag &= ~ECHO;
    // set attribs
    if (tcsetattr(0, TCSANOW, &info) == -1) oops("tcsetattr",2);
    return 0;
}
