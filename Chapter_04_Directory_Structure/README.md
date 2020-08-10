# Chapter03 Directory Structure

- pwd1.c
    1. Tracing up from the current directory and continuing into the parent directory.
    2. Each time you enter the parent directory, check the inode number of "."
    3. And store the name of the inode in the struct `dirent` of the parent directory.
    4. Until it reaches the root `/`

    Bug: pwd() ends at the root of current user's file system not the root of whole.
