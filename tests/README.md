#### Test
Run `make`.  
This will create a small executable (called `test`) which will parse an input test file containing specific syntax (described below) and print out the matrix (the doubly linked list of Line objects) as it would appear after those transformations have been performed.

#### Syntax
```
INSSTR <string> : insert given string at cursor

BP : backspace

INSNL : insert newline at cursor

MVCRSR <row> <col> : move the cursor to specified row/col

MVCRSRUP: move the cursor up

MVCRSRDN : move the cursor down

MVCRSRRT : move the cursor right

MVCRSRLFT : move the cursor left
```
*Any line starting with a "#" is treated as a comment  
Blank lines are ignored  
The command length must not exceed 99 characters*

A file `test1` could be created which contains some combination of the above commands (one per line).  
It would then be run as `./test test1`
