lex = src/gocompiler.l
yacc = src/gocompiler.y
src = src/lex.yy.c src/y.tab.c $(shell echo src/*.c)
out = gocompiler
flags = -Wall -Wextra

$(out): $(src) 
	clang-7 $(flags) -g $^ -o $@

src/lex.yy.c: $(lex)
	flex -o $@ $^

src/y.tab.c: $(yacc)
	yacc -o $@ -d $^

###################################################

temp = src/lex.yy.c src/y.tab.c src/y.tab.h

.PHONY: clean
clean:
	rm -rf $(temp)