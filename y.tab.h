/* A Bison parser, made by GNU Bison 3.5.1.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2020 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    INTLIT = 258,
    STRLIT = 259,
    REALLIT = 260,
    SEMICOLON = 261,
    COMMA = 262,
    BLANKID = 263,
    ASSIGN = 264,
    ID = 265,
    VAR = 266,
    PLUS = 267,
    MINUS = 268,
    STAR = 269,
    DIV = 270,
    MOD = 271,
    EQ = 272,
    GE = 273,
    GT = 274,
    LE = 275,
    LT = 276,
    NE = 277,
    NOT = 278,
    AND = 279,
    OR = 280,
    LPAR = 281,
    RPAR = 282,
    LBRACE = 283,
    RBRACE = 284,
    LSQ = 285,
    RSQ = 286,
    PACKAGE = 287,
    RETURN = 288,
    PRINT = 289,
    PARSEINT = 290,
    FUNC = 291,
    CMDARGS = 292,
    RESERVED = 293,
    IF = 294,
    ELSE = 295,
    FOR = 296,
    INT = 297,
    FLOAT32 = 298,
    BOOL = 299,
    STRING = 300
  };
#endif
/* Tokens.  */
#define INTLIT 258
#define STRLIT 259
#define REALLIT 260
#define SEMICOLON 261
#define COMMA 262
#define BLANKID 263
#define ASSIGN 264
#define ID 265
#define VAR 266
#define PLUS 267
#define MINUS 268
#define STAR 269
#define DIV 270
#define MOD 271
#define EQ 272
#define GE 273
#define GT 274
#define LE 275
#define LT 276
#define NE 277
#define NOT 278
#define AND 279
#define OR 280
#define LPAR 281
#define RPAR 282
#define LBRACE 283
#define RBRACE 284
#define LSQ 285
#define RSQ 286
#define PACKAGE 287
#define RETURN 288
#define PRINT 289
#define PARSEINT 290
#define FUNC 291
#define CMDARGS 292
#define RESERVED 293
#define IF 294
#define ELSE 295
#define FOR 296
#define INT 297
#define FLOAT32 298
#define BOOL 299
#define STRING 300

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
