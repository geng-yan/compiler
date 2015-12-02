#include <stdio.h>
#include "calc3.h"
#include "y.tab.h"

static int cont = 0;
static int brk = 0;
static int outmost = 1;

int ex(nodeType *p) {
    int i;
    if ((brk == 1 || cont == 1) && outmost == 0) return 0;
    if (!p) return 0;
    switch(p->type) {
    case typeCon:       return p->con.value;
    case typeId:        return sym[p->id.i];
    case typeOpr:
        switch(p->opr.oper) {
	case FOR:
            ex(p->opr.op[0]);
            outmost = 0;
			while (brk == 0 && ex(p->opr.op[1])) {
                cont = 0;
				ex(p->opr.op[3]);
                cont = 0;
				ex(p->opr.op[2]);
			}
            brk = 0;
			return 0;
    case WHILE:     outmost = 0; while(brk == 0 && ex(p->opr.op[0])) { cont = 0; ex(p->opr.op[1]);} brk = 0; return 0;
    case DO:        outmost = 0; ex(p->opr.op[0]); while(brk == 0 && ex(p->opr.op[1])) { cont = 0; ex(p->opr.op[0]);} brk = 0; return 0;
    case IF:        if (ex(p->opr.op[0]))
                        ex(p->opr.op[1]);
                    else if (p->opr.nops > 2)
                        ex(p->opr.op[2]);
                    return 0;
    case BREAK:     brk = 1; return 0;
    case CONTINUE:  cont = 1; return 0;
    case PRINT:     printf("%d\n", ex(p->opr.op[0])); return 0;
	case READ:	    printf("? "); scanf("%d", &i);
			        return sym[p->opr.op[0]->id.i] = i;
    case ';':       ex(p->opr.op[0]); return ex(p->opr.op[1]);
    case '=':       return sym[p->opr.op[0]->id.i] = ex(p->opr.op[1]);
    case UMINUS:    return -ex(p->opr.op[0]);
    case '+':       return ex(p->opr.op[0]) + ex(p->opr.op[1]);
    case '-':       return ex(p->opr.op[0]) - ex(p->opr.op[1]);
    case '*':       return ex(p->opr.op[0]) * ex(p->opr.op[1]);
    case '/':       return ex(p->opr.op[0]) / ex(p->opr.op[1]);
    case '%':       return ex(p->opr.op[0]) % ex(p->opr.op[1]);
    case '<':       return ex(p->opr.op[0]) < ex(p->opr.op[1]);
    case '>':       return ex(p->opr.op[0]) > ex(p->opr.op[1]);
    case GE:        return ex(p->opr.op[0]) >= ex(p->opr.op[1]);
    case LE:        return ex(p->opr.op[0]) <= ex(p->opr.op[1]);
    case NE:        return ex(p->opr.op[0]) != ex(p->opr.op[1]);
    case EQ:        return ex(p->opr.op[0]) == ex(p->opr.op[1]);
	case AND:	return ex(p->opr.op[0]) && ex(p->opr.op[1]);
	case OR:	return ex(p->opr.op[0]) || ex(p->opr.op[1]);
        }
    }
    return 0;
}
