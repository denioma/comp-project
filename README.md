# comp-project
Compilers' Project

## TODO

Submit to mooshak and despair.

## Implemented

* Lexical error throws:
  * illegal character
  * unterminated string literal
  * invalid escape sequence
  * invalid octal constant 
  * unterminated comment (occurs when a multi line comment isn't closed before EOF)
* RESERVED tokens
* Automatic `SEMICOLON` after `LIT`, `ID`, `RETURN`, `RPAR`, `RSQ` ou `RBRACE`

## Authors
José Miguel Rocha Gonçalves
Rodrigo Alexandre da Mota Machado

## Mooshak

```
H - Illegal Characters: 0/20
J - Unterminated Comments: 0/10
```

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
✅ PASSED testcases/str_bad_1.dgo
✅ PASSED testcases/text_1.dgo
✅ PASSED testcases/text_2.dgo
```
