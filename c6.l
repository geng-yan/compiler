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
"do"        return DO;
"while"         return WHILE;
"if"            return IF;
"else"          return ELSE;
"print"         return PRINT;
"read"      return READ;
"continue"  return CONTINUE;
"break"     return BREAK;
"&&"        return AND;
"||"        return OR;

{VAR}       { 
                yylval.sIndex = tolowercase(strdup(yytext));
                return VARIABLE;
            }

0           {
                yylval.iValue = atoi(yytext);
                return INTEGER;
            }

[1-9][0-9]* {
                yylval.iValue = atoi(yytext);
                return INTEGER;
            }

\'[A-Za-z]\'    {
                    yylval.iValue = (int)(yytext[1]);
                    return CHAR;
                }

[-()<>=+*/;{}.%] {
                return *yytext;
             }

">="            return GE;
"<="            return LE;
"=="            return EQ;
"!="            return NE;


\/\/.*\n	;	/* skip comments */

[ \t\n]+        ;       /* ignore whitespace */

.               yyerror("Unknown character");
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
