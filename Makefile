sudoku: sudoku.c
	gcc -std=c99 -pthread -o sudoku sudoku.c -I.

extra: extra_credit.c
	gcc -std=c99 -pthread -o extra extra_credit.c -I.