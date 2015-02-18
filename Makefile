all:
	gcc -c editor.c
	gcc -c run.c
	gcc -Wall run.o editor.o -lncurses
	$(MAKE) -C tests/

clean:
	rm -f a.out *.o
	$(MAKE) clean -C tests/
