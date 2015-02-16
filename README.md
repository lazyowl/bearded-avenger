#Foam
Foam is an extremely basic text editor written in C using ncurses.

#### Features
* write stuff on the screen
* read and edit from file

#### To Do
* save to file
* add scrolling

#### Design
The editor now uses a doubly linked list of Line structure pointers.
Each line contains a dynamically allocated array for storing text.
Nothing fancy about the data structures being used (not optimized in any way).
