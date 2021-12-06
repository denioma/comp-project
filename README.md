# comp-project

Compilers' Project

# Authors

José Miguel Rocha Gonçalves

Rodrigo Alexandre da Mota Machado


# Index

[Go To Meta 1](#meta-1)

[Go To Meta 2](#meta-2)

[Go To Meta 3](#meta-3)

# Meta 3

## Mooshak 

```
B: 25/25
C: 58/58
D: 84/84
E: 22/33
F: 28/50

Total: 217/250
Missing: 33
```

## Test Status

```
✅ PASSED meta3/errors.dgo
✅ PASSED meta3/errors_parseargs.dgo
✅ PASSED meta3/factorial.dgo
✅ PASSED meta3/logical_operators.dgo
✅ PASSED meta3/statements_expressions.dgo
✅ PASSED meta3/valid_program.dgo
✅ PASSED meta3/variable_func_assign_errors.dgo
✅ PASSED meta3/variable_func_symtab_aast.dgo
```

# Meta 2

## Test Status

```
✅ PASSED testcases/meta2/blocks.dgo
✅ PASSED testcases/meta2/braces.dgo
✅ PASSED testcases/meta2/circle.dgo
✅ PASSED testcases/meta2/eof.dgo
✅ PASSED testcases/meta2/eof1.dgo
✅ PASSED testcases/meta2/error_unbalanced_par.dgo
✅ PASSED testcases/meta2/errors.dgo
✅ PASSED testcases/meta2/expressionHard.dgo
✅ PASSED testcases/meta2/expressions.dgo
✅ PASSED testcases/meta2/expressions_2.dgo
✅ PASSED testcases/meta2/factorial.dgo
✅ PASSED testcases/meta2/funcParams.dgo
✅ PASSED testcases/meta2/multiple_funcdecl_blocks.dgo
✅ PASSED testcases/meta2/precedence.dgo
✅ PASSED testcases/meta2/smallest.dgo
✅ PASSED testcases/meta2/stat_error.dgo
✅ PASSED testcases/meta2/stat_error2.dgo
✅ PASSED testcases/meta2/statements.dgo
✅ PASSED testcases/meta2/statements_ast.dgo
✅ PASSED testcases/meta2/sum.dgo
✅ PASSED testcases/meta2/syntax_errors_stmt.dgo
✅ PASSED testcases/meta2/testeErros.dgo
✅ PASSED testcases/meta2/varfuncdecl.dgo
✅ PASSED testcases/meta2/variable_func_declarations.dgo
```

# Meta 1

## Mooshak

```
We did it gaymers!
```

## Implemented

* Lexical error throws:
  * illegal character
  * unterminated string literal
  * invalid escape sequence
  * invalid octal constant
  * unterminated comment (occurs when a multi line comment isn't closed before EOF)
* RESERVED tokens
* Automatic `SEMICOLON` after `LIT`, `ID`, `RETURN`, `RPAR`, `RSQ` ou `RBRACE`

## Tests Status

```
✅ PASSED testcases/auto_semicolon_1.dgo
✅ PASSED testcases/blank_1.dgo
✅ PASSED testcases/comments_1.dgo
✅ PASSED testcases/comments_2.dgo
✅ PASSED testcases/comments_bad_1.dgo
✅ PASSED testcases/comments_bad_2.dgo
✅ PASSED testcases/empty_1.dgo
✅ PASSED testcases/factorial.dgo
✅ PASSED testcases/ids_keywords_1.dgo
✅ PASSED testcases/ids_keywords_2.dgo
✅ PASSED testcases/ids_keywords_3.dgo
✅ PASSED testcases/illegalchar_1.dgo
✅ PASSED testcases/literals_1.dgo
✅ PASSED testcases/literals_2.dgo
✅ PASSED testcases/literals_3.dgo
✅ PASSED testcases/literals_4.dgo
✅ PASSED testcases/operators_1.dgo
✅ PASSED testcases/operators_2.dgo
✅ PASSED testcases/programs_1.dgo
✅ PASSED testcases/semicolons_1.dgo
✅ PASSED testcases/semicolons_2.dgo
✅ PASSED testcases/song_1.dgo
✅ PASSED testcases/song_2.dgo
✅ PASSED testcases/song_3.dgo
✅ PASSED testcases/str_bad_1.dgo
✅ PASSED testcases/text_1.dgo
✅ PASSED testcases/text_2.dgo
✅ PASSED testcases/unt-comment--escape-chars.dgo
```
