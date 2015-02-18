test: editor.o tester.o
	gcc editor.o tester.o -o test -lncurses

editor.o:
	gcc -c ../editor.c

tester.o:
	gcc -c tester.c

clean:
	rm -f *.o test
