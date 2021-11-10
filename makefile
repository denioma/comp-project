lex = gocompiler.l
yacc = gocompiler.y
src = lex.yy.c y.tab.c
out = gocompiler.out

$(out): $(src) 
	clang-7 $^ -o $@

lex.yy.c: $(lex)
	flex $^
y.tab.c: $(yacc)
	yacc -d $^

.PHONY: clean
clean:
	rm -rf lex.yy.c y.tab.c