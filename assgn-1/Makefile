###### Sun Apr 21 12:26:47 PDT 2019

# Referenced https://www.freebsd.org/doc/en/books/developers-handbook/tools-make.html & Piazza
make:
	flex lex.l
	cc -c lex.yy.c
	cc -c myshell.c
	cc -o myshell myshell.o lex.yy.o -lfl

infer:
	make clean
	infer-capture -- make
	infer-analyze -- make

clean:
	rm myshell.o lex.yy.o lex.yy.c