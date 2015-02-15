#Foam
Foam is an extremely basic text editor written in C using ncurses.

#### Features
* write stuff on the screen
* read and edit from file

#### To Do
* allow for larger line lengths
* save to file

#### Design
The editor now uses a doubly linked list of Line structure pointers.
However, for now, within each Line is an array of fixed size.
