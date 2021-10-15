lex = gocompiler.l
out = gocompiler.out

$(out): $(lex)
	flex $^ 
	clang-7 lex.yy.c -o $@
	rm lex.yy.c
