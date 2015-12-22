%{
#include <stdlib.h>
#include <string.h>
#include <stdlib.h>
#include "ctype.h"
#include "calc3.h"
#include "y.tab.h"
void yyerror(char *);
char* tolowercase(char *);
%}
%option yylineno

VAR         [A-Za-z][A-Za-z0-9]{0,11}

%%
"for"       return FOR;
"geti"      return GETI;
"getc"      return GETC;
"gets"      return GETS;
"puti"      return PUTI;
"puti_"     return PUTIN;
"putc"      return PUTC;
"putc_"     return PUTCN;
"do"        return DO;
"return"    return RETURN;
"while"         return WHILE;
"if"            return IF;
"else"          return ELSE;
"print"         return PRINT;
"read"      return READ;
"continue"  return CONTINUE;
"break"     return BREAK;
"&&"        return AND;
"||"        return OR;
"array"     return ARRAY;

{VAR}       { 
                yylval.sValue = tolowercase(strdup(yytext));
                return VARIABLE;
            }

@{VAR}      {
                yylval.sValue = tolowercase(strdup(++yytext));
                return GLOBAL;
            }

0           {
                yylval.iValue = atoi(yytext);
                return INTEGER;
            }

[1-9][0-9]* {
                yylval.iValue = atoi(yytext);
                return INTEGER;
            }

\'.\'    {
                    yylval.iValue = (int)(yytext[1]);
                    return CHAR;
                }
\".*\"      { yylval.sValue = strdup(yytext);
                    return STRING;}
[-()<>=+*/;\[\]{}.%,] {
                return *yytext;
             }

">="            return GE;
"<="            return LE;
"=="            return EQ;
"!="            return NE;


\/\/.*\n	;	/* skip comments */

[ \t\n]+        ;       /* ignore whitespace */

.               {yyerror(yytext);}
%%
int yywrap(void) {
    return 1;
}

char* tolowercase(char* s) {
    int i = 0;
    for (i = 0; s[i]; i++) {
        s[i] = tolower(s[i]);
    }
    return s;
}
