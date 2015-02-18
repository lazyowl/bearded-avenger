all:
	gcc -c editor.c
	gcc -c run.c
	gcc -Wall run.o editor.o -lncurses

clean:
	rm -f a.out *.o
