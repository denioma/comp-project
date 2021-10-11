all:
	flex gocompiler.l
	clang-7 lex.yy.c -o gocompiler.out
	rm lex.yy.c
