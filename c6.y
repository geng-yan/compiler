%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
// #include <vector>
#include "calc3.h"

/* prototypes */
nodeType *opr(int oper, int nops, ...);
nodeType *id(char* var);
nodeType *gl(char* var);
nodeType *con(int value);
nodeType *cha(int value);
nodeType *strg(char *value);
void freeNode(nodeType *p);
int ex(nodeType *p);
int yylex(void);
void yyerror(char *s);
int sym[26];                    /* symbol table */
%}

%union {
    int iValue;                 /* integer value */
    char* sValue;                /* symbol table index & nas string value */
    nodeType *nPtr;             /* node pointer */
};

%token <iValue> INTEGER CHAR
%token <sValue> VARIABLE GLOBAL STRING
%token FOR WHILE IF PRINT READ DO BREAK CONTINUE FUNCALL FUNDCLR RETURN GETI GETC GETS READC PUTC PUTCN PUTI PUTIN PUTS PUTSN
%token PI PIN PC PCN PS PSN ARRAY ACCESS
%nonassoc IFX
%nonassoc ELSE

%left AND OR

%left GE LE EQ NE '>' '<'
%left '+' '-'
%left '*' '/' '%'
%nonassoc UMINUS

%type <nPtr> stmt expr stmt_list name_list single dims access expr_list var_list

%%

program:
        function                { exit(0); }
        ;

function:
          function stmt         { ex($2); freeNode($2); }
        | /* NULL */
        ;

stmt:
          ';'                            { $$ = opr(';', 2, NULL, NULL); }
        | expr ';'                          { $$ = $1; }
        | GETI '(' expr_list ')' ';'      { $$ = opr(GETI,1,$3);}
        | GETC '(' expr_list ')' ';'      { $$ = opr(GETC,1,$3);}
        | PRINT expr ';'                 { $$ = opr(PRINT, 1, $2); }
        | PUTI '(' expr_list ')' ';'      { $$ = opr(PUTI,1,$3);}
        | PUTIN '(' expr_list ')' ';'      { $$ = opr(PUTIN,1,$3);}
        | PUTI '(' STRING ',' expr_list ')' ';' {$$ = opr(PUTI,2,strg($3),$5);}
        | PUTC '(' expr_list ')' ';'      { $$ = opr(PUTC,1,$3);}
        | PUTCN '(' expr_list ')' ';'      { $$ = opr(PUTCN,1,$3);}
        | PUTS '(' expr_list ')' ';'      { $$ = opr(PUTS,1,$3);}
        | PUTSN '(' expr_list ')' ';'      { $$ = opr(PUTSN,1,$3);}
        | PUTC '(' STRING ',' expr_list ')' ';' {$$ = opr(PUTC,2,strg($3),$5);}
        | RETURN expr ';'                { $$ = opr(RETURN,1,$2); }
        | BREAK ';'                      { $$ = opr(BREAK,0);}
        | CONTINUE ';'                      { $$ = opr(CONTINUE,0);}
        | READ VARIABLE ';'      { $$ = opr(READ, 1, id($2)); }
        | VARIABLE '=' expr ';'          { $$ = opr('=', 2, id($1), $3); }
        | access '=' expr ';'            { $$ = opr('=', 2, $1, $3); }
        | ARRAY name_list ';'                { $$ = $2; }
        | FOR '(' stmt stmt stmt ')' stmt { $$ = opr(FOR, 4, $3, $4, $5, $7); }
        | WHILE '(' expr ')' stmt        { $$ = opr(WHILE, 2, $3, $5); }
        | DO stmt WHILE '(' expr ')'     { $$ = opr(DO,2,$2,$5); }
        | IF '(' expr ')' stmt %prec IFX { $$ = opr(IF, 2, $3, $5); }
        | IF '(' expr ')' stmt ELSE stmt { $$ = opr(IF, 3, $3, $5, $7); }
        | '{' stmt_list '}'              { $$ = $2; }
        | VARIABLE '(' expr_list ')' '{' stmt_list '}' {$$ = opr(FUNDCLR,3,id($1),$3,$6);}
        ;

stmt_list:
          stmt                  { $$ = $1; }
        | stmt_list stmt        { $$ = opr(';', 2, $1, $2); }
        ;

name_list:
          single                        { $$ = $1; }
        | single ',' name_list          { $$ = opr(',', 3, $1, $3, id(0)); }

single:
          VARIABLE '[' dims ']'     { $$ = opr(ARRAY, 2, id($1), $3); }
        | VARIABLE '[' dims ']' '=' INTEGER { $$ = opr(ARRAY, 4, id($1), $3, con($6), con(1)); }
        | VARIABLE '[' dims ']' '=' CHAR    { $$ = opr(ARRAY, 4, id($1), $3, cha($6), con(2)); }

dims:
          INTEGER                     { $$ = con($1); }
        | INTEGER ',' dims           { opr(',', 3, con($1), $3, id(0)); }

access:
          VARIABLE '[' expr ']' { $$ = opr(ACCESS, 2, id($1), $3); }

expr:
          INTEGER               { $$ = con($1);  }
        | access                { $$ = $1; }
        | VARIABLE              { $$ = id($1); }
        | STRING                { $$ = strg($1); }
        | GLOBAL                { $$ = gl($1); }
        | CHAR                  { $$ = cha($1);  }
        | VARIABLE '(' expr_list ')' {$$ = opr(FUNCALL,2,id($1),$3);}
        | '-' expr %prec UMINUS { $$ = opr(UMINUS, 1, $2); }
        | expr '+' expr         { $$ = opr('+', 2, $1, $3); }
        | expr '-' expr         { $$ = opr('-', 2, $1, $3); }
        | expr '*' expr         { $$ = opr('*', 2, $1, $3); }
        | expr '%' expr         { $$ = opr('%', 2, $1, $3); }
        | expr '/' expr         { $$ = opr('/', 2, $1, $3); }
        | expr '<' expr         { $$ = opr('<', 2, $1, $3); }
        | expr '>' expr         { $$ = opr('>', 2, $1, $3); }
        | expr GE expr          { $$ = opr(GE, 2, $1, $3); }
        | expr LE expr          { $$ = opr(LE, 2, $1, $3); }
        | expr NE expr          { $$ = opr(NE, 2, $1, $3); }
        | expr EQ expr          { $$ = opr(EQ, 2, $1, $3); }
	| expr AND expr		{ $$ = opr(AND, 2, $1, $3); }
	| expr OR expr		{ $$ = opr(OR, 2, $1, $3); }
        | '(' expr ')'          { $$ = $2; }
        ;

expr_list:
          expr_list ',' expr {$$ = opr(',',3,$1,$3, id(1));}
        | expr {$$ = $1;}
        | {$$ = NULL;}
        ;
var_list:
          VARIABLE              {$$ = $1;}
        | var_list ',' VARIABLE {$$ = opr(',',3,$1,$3,id(1));}
        | {$$ = NULL;}
        ;
%%

#define SIZEOF_NODETYPE ((char *)&p->con - (char *)p)


nodeType *con(int value) {
    nodeType *p;
    size_t nodeSize;

    /* allocate node */
    nodeSize = SIZEOF_NODETYPE + sizeof(conNodeType);
    if ((p = (nodeType*) malloc(nodeSize)) == NULL)
        yyerror("out of memory");

    /* copy information */
    p->type = typeCon;
    p->con.value = value;

    return p;
}

nodeType *cha(int value) {
    nodeType *p;
    size_t nodeSize;

    /* allocate node */
    nodeSize = SIZEOF_NODETYPE + sizeof(conNodeType);
    if ((p = (nodeType*) malloc(nodeSize)) == NULL)
        yyerror("out of memory");

    /* copy information */
    p->type = typeCha;
    p->con.value = value;

    return p;
}

// added: for nas string
nodeType *strg(char *value) {
    nodeType *p;
    size_t nodeSize;

    /* allocate node */
    nodeSize = (char *)&p->strg - (char *)p + sizeof(strgNodeType);
    if ((p = (nodeType*) malloc(nodeSize)) == NULL)
        yyerror("out of memory");

    /* copy information */
    p->type = typeString;
    p->strg.value = value;

    return p;
}

nodeType *id(char* var) {
    nodeType *p;
    size_t nodeSize;

    /* allocate node */
    nodeSize = SIZEOF_NODETYPE + sizeof(idNodeType);
    if ((p = (nodeType*) malloc(nodeSize)) == NULL)
        yyerror("out of memory");

    /* copy information */
    p->type = typeId;
    p->id.id = strdup(var);
    p->id.global = 0;
    return p;
}

nodeType *gl(char* var) {
    nodeType *p;
    size_t nodeSize;

    /* allocate node */
    nodeSize = SIZEOF_NODETYPE + sizeof(idNodeType);
    if ((p = (nodeType*) malloc(nodeSize)) == NULL)
        yyerror("out of memory");

    /* copy information */
    p->type = typeId;
    p->id.id = strdup(var);
    p->id.global = 1;
    return p;
}

nodeType *opr(int oper, int nops, ...) {
    va_list ap;
    nodeType *p;
    size_t nodeSize;
    int i;

    /* allocate node */
    nodeSize = SIZEOF_NODETYPE + sizeof(oprNodeType) +
        (nops - 1) * sizeof(nodeType*);
    if ((p = (nodeType*) malloc(nodeSize)) == NULL)
        yyerror("out of memory");

    /* copy information */
    p->type = typeOpr;
    p->opr.oper = oper;
    p->opr.nops = nops;
    va_start(ap, nops);
    for (i = 0; i < nops; i++)
        p->opr.op[i] = va_arg(ap, nodeType*);
    va_end(ap);
    return p;
}

void freeNode(nodeType *p) {
    int i;

    if (!p) return;
    if (p->type == typeOpr) {
        for (i = 0; i < p->opr.nops; i++)
            freeNode(p->opr.op[i]);
    }
    free (p);
}

void yyerror(char *s) {
    fprintf(stdout, "%s\n", s);
}

int main(int argc, char **argv) {
extern FILE* yyin;
    yyin = fopen(argv[1], "r");
    yyparse();
    return 0;
}
