#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "calc3.h"
#include "y.tab.h"
#include <stdbool.h>

static int lbl;

static int sp;

// Please set "capital" to:
//  - "true": if the nas instructions are in the form of "compGT"
//  - "false": if the nas instructions are in the form of "compgt"
static bool capital = true;

// when set to true, print out the current value of sp
static bool debug = false;

int find(int i){
	pair* curr;
	for(curr = head; curr->next != NULL; curr=curr->next){
		if (i == curr->name)
			return curr->pos;
	}
	return -1;
}

pair* getSpace(int name){
    printf("\tpush\t0\n");
    pair* newVar = (pair*) malloc(sizeof(pair));
    newVar->name = name;
    newVar->pos = sp;
    sp++;
    if (debug) printf("\t\t\tsp++: %d\n", sp);
    newVar->next = head;
    head = newVar;
    return head;
}

// for the compiler, not executing, only generates the (assembly) code
int ex(nodeType *p, int contTo, int breakTo) {
    int cond, after, update, lbl1, lbl2;
    int bf;

    int pos;

    if (!p) return 0;
    switch(p->type) {
    case typeCon:
        printf("\tpush\t%d\n", p->con.value);
        sp++;
        if (debug) printf("\t\t\tsp++: %d\n", sp);
        break;
    case typeId:
        pos = find(p->id.i);
		printf("\tpush\tfp[%d]\n", pos);
        sp++;
        if (debug) printf("\t\t\tsp++: %d\n", sp);
        break;
    case typeOpr:
        switch(p->opr.oper) {
		case FOR:
			ex(p->opr.op[0], contTo, breakTo);
			printf("L%03d:\n", lbl1 = lbl++);
			ex(p->opr.op[1], contTo, breakTo);
			printf("\tj0\tL%03d\n", lbl2 = lbl++);
            sp--;
            if (debug) printf("\t\t\tsp--: %d\n", sp);
            update = lbl++;
            ex(p->opr.op[3], update, lbl2);// loop body: 'update' for continue, 'lbl2' for break
            printf("L%03d:\n", update);
			ex(p->opr.op[2], contTo, breakTo); // update made in each iteration
			printf("\tjmp\tL%03d\n", lbl1);
			printf("L%03d:\n", lbl2);
			break;
        case WHILE:
            printf("L%03d:\n", lbl1 = lbl++);
            ex(p->opr.op[0], contTo, breakTo);
            printf("\tj0\tL%03d\n", lbl2 = lbl++);
            sp--;
            if (debug) printf("\t\t\tsp--: %d\n", sp);
            ex(p->opr.op[1], lbl1, lbl2); // loop body: 'lbl1' for continue, 'lbl2' for break
            printf("\tjmp\tL%03d\n", lbl1);
            printf("L%03d:\n", lbl2);
            break;
		case DO:
            printf("L%03d:\n", lbl1 = lbl++);
            cond = lbl++;
            after = lbl++;
            ex(p->opr.op[0], cond, after); // loop body: 'cond' for continue, 'after' for break
            printf("L%03d:\n", cond);
			ex(p->opr.op[1], contTo, breakTo);
            printf("\tj1\tL%03d\n", lbl1);
            sp--;
            if (debug) printf("\t\t\tsp--: %d\n", sp);
            printf("L%03d:\n", after); // label for break
            break;
        case BREAK:
            printf("\tjmp\tL%03d\n", breakTo);
            break;
        case CONTINUE:
            printf("\tjmp\tL%03d\n", contTo);
            break;
        case IF:
            ex(p->opr.op[0], contTo, breakTo);
            if (p->opr.nops > 2) {
                /* if else */
                printf("\tj0\tL%03d\n", lbl1 = lbl++);
                sp--;
                if (debug) printf("\t\t\tsp--: %d\n", sp);
                ex(p->opr.op[1], contTo, breakTo);
                printf("\tjmp\tL%03d\n", lbl2 = lbl++);
                printf("L%03d:\n", lbl1);
                ex(p->opr.op[2], contTo, breakTo);
                printf("L%03d:\n", lbl2);
            } else {
                /* if */
                printf("\tj0\tL%03d\n", lbl1 = lbl++);
                sp--;
                if (debug) printf("\t\t\tsp--: %d\n", sp);
                ex(p->opr.op[1], contTo, breakTo);
                printf("L%03d:\n", lbl1);
            }
            break;
		case READ:
            pos = find(p->opr.op[0]->id.i);
            if (pos == -1){ // new variable
                getSpace(p->opr.op[0]->id.i);
                pos = head->pos;
            }
            printf("\tgeti\n");
            printf("\tpop\tfp[%d]\n", pos);
		    break;
        case PRINT:     
            ex(p->opr.op[0], contTo, breakTo);
            printf("\tputi\n");
            sp--;
            if (debug) printf("\t\t\tsp--: %d\n", sp);
            break;
        case '=':
            pos = find(p->opr.op[0]->id.i);
			if (pos == -1){ // new variable
                head = getSpace(p->opr.op[0]->id.i);
                pos = head->pos;
            }
            ex(p->opr.op[1], contTo, breakTo);
			printf("\tpop\tfp[%d]\n", pos);
            sp--;
            if (debug) printf("\t\t\tsp--: %d\n", sp);
            break;
        case UMINUS:    
            ex(p->opr.op[0], contTo, breakTo);
            printf("\tneg\n");
            sp--;
            break;
        default:
            ex(p->opr.op[0], contTo, breakTo);
            ex(p->opr.op[1], contTo, breakTo);
            sp--;
            if (debug) printf("\t\t\tsp--: %d\n", sp);
            switch(p->opr.oper) {
            case '+':   printf("\tadd\n"); break;
            case '-':   printf("\tsub\n"); break; 
            case '*':   printf("\tmul\n"); break;
            case '/':   printf("\tdiv\n"); break;
            case '%':   printf("\tmod\n"); break;
            case '<':   if (capital) printf("\tcompLT\n"); else printf("\tcomplt\n"); break;
            case '>':   if (capital) printf("\tcompGT\n"); else printf("\tcompgt\n"); break;
            case GE:    if (capital) printf("\tcompGE\n"); else printf("\tcompge\n"); break;
            case LE:    if (capital) printf("\tcompLE\n"); else printf("\tcomple\n"); break;
            case NE:    if (capital) printf("\tcompNE\n"); else printf("\tcompne\n"); break;
            case EQ:    if (capital) printf("\tcompEQ\n"); else printf("\tcompeq\n"); break;
    	    case AND:   printf("\tand\n"); break;
    	    case OR:    printf("\tor\n"); break;
            }
        }
    }
    return 0;
}