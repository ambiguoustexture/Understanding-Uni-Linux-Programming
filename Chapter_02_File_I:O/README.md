# Chapter02 File I/O

## Modified samples from the book "Understanding Unix/Linux Programming" by Bruce Molay.
- who1.c
    - use ```utmpx``` instead of ```utmp``` which was deprecated.
    - use ```getutxent()``` to get the next entry of the ```utmpx``` user accounting database.
    - use ```endutxent()``` to close the database.
- who2.c
    - use ```utmp->ut_type == USER_PROCESS``` to see if login 
    - use ```ctime``` to display readable time
```shell
➜  ./who2
here     console  Aug  7 19:28
here     ttys000  Aug  7 19:51
here     ttys001  Aug  7 23:16
➜  who
here     console  Aug  7 19:28
here     ttys000  Aug  7 19:51
here     ttys001  Aug  7 23:16
```
- cp1.c
