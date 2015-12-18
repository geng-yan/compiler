/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton interface for Bison's Yacc-like parsers in C

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

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

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     INTEGER = 258,
     CHAR = 259,
     VARIABLE = 260,
     FOR = 261,
     WHILE = 262,
     IF = 263,
     PRINT = 264,
     READ = 265,
     DO = 266,
     BREAK = 267,
     CONTINUE = 268,
     IFX = 269,
     ELSE = 270,
     OR = 271,
     AND = 272,
     NE = 273,
     EQ = 274,
     LE = 275,
     GE = 276,
     UMINUS = 277
   };
#endif
/* Tokens.  */
#define INTEGER 258
#define CHAR 259
#define VARIABLE 260
#define FOR 261
#define WHILE 262
#define IF 263
#define PRINT 264
#define READ 265
#define DO 266
#define BREAK 267
#define CONTINUE 268
#define IFX 269
#define ELSE 270
#define OR 271
#define AND 272
#define NE 273
#define EQ 274
#define LE 275
#define GE 276
#define UMINUS 277




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 21 "c6.y"
{
    int iValue;                 /* integer value */
    char* sIndex;                /* symbol table index */
    nodeType *nPtr;             /* node pointer */
}
/* Line 1529 of yacc.c.  */
#line 99 "y.tab.h"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;

