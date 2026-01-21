
expr_g:expr.l expr.y
	flex -t expr.l > expr.c
	bison -d expr.y
	gcc expr.c expr.tab.c -lfl -o expr_p

clean:
	rm -f *.c *.h expr_p
