%{
#include <stdlib.h>
<<<<<<< HEAD
#include <stdio.h>
#include <ctype.h>
#include <iostream>
#include <map>
#include <string.h>
=======
#include <string.h>
#include <stdlib.h>
>>>>>>> master
#include "calc3.h"
#include "y.tab.h"
using namespace std;
void yyerror(char *);
map<string, int> id_list;
int id_index = 0;
int get_id();

%}

<<<<<<< HEAD
VAR         [A-Za-z][A-Za-z0-9]{0,11}

%%

=======
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

[a-z]+       { 
                yylval.sIndex = strdup(yytext);
                return VARIABLE;
            }

>>>>>>> master
0           {
                yylval.iValue = atoi(yytext);
                return INTEGER;
            }

[1-9][0-9]* {
                yylval.iValue = atoi(yytext);
                return INTEGER;
            }

[-()<>=+*/;{}.%] {
                return *yytext;
             }

">="            return GE;
"<="            return LE;
"=="            return EQ;
"!="            return NE;
<<<<<<< HEAD
"for"		    return FOR;
"while"         return WHILE;
"if"            return IF;
"else"          return ELSE;
"print"         return PRINT;
"read"		    return READ;
"&&"		    return AND;
"||"		    return OR;
"do"		    return DO;
"break"		    return BREAK;
"continue"	    return CONTINUE;

{VAR}      {    
                yylval.sIndex = get_id();
                return VARIABLE;
            }
=======

>>>>>>> master

\/\/.*\n	;	/* skip comments */

[ \t\n]+        ;       /* ignore whitespace */

<<<<<<< HEAD
.           {yyerror("Unknown Variable");}

=======
.               yyerror("Unknown character");
>>>>>>> master
%%

int yywrap(void) {
    return 1;
}

int get_id() { // get the id of the variable, if new, assign a new index
    string name = "";
    for (int i = 0; i < yyleng; i++) {
        name += tolower(yytext[i]);
    }
    map<string, int>::iterator it = id_list.find(name);
    if (it == id_list.end())
        id_list[name] = id_index; id_index++;
    return id_list[name];
}
