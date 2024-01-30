all:
	gcc -Wall -Werror -Wextra -o hangman *.c

clean:
	rm -f hangman.exe & rm -f hangman

fclean:
	rm -f hangman.exe & rm -f hangman

run: all
	./hangman