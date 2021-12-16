lex = src/gocompiler.l
yacc = src/gocompiler.y
src = src/lex.yy.c src/y.tab.c src/tree.c src/semantic.c src/symtab.c
out = gocompiler

$(out): $(src) 
	clang-7 -g $^ -o $@

src/lex.yy.c: $(lex)
	flex -o $@ $^

src/y.tab.c: $(yacc)
	bison -o $@ -d $^

###################################################

temp = src/lex.yy.c src/y.tab.c src/y.tab.h

.PHONY: clean
clean:
	rm -rf $(temp)
