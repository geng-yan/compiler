#include <stdio.h>
#include "calc3.h"
#include <string.h>
#include <stdlib.h>
#include "y.tab.h"
int isbreak=0,iscontinue=0;
static int loopcount=0;
struct dic{
    char *name;
    int value;
    struct dic *next;
};
static struct dic *head=NULL;
struct dic* addVar2Point(char* var)
{
    static struct dic *tail = NULL;
    //printf("---%s---%d\n",var,fpoffset-1);
    if(head==NULL)
    {
        head = (struct dic*)malloc(sizeof(struct dic));
        head->name = strdup(var);
        head->value = 0;
        head->next = NULL;
        tail = head;
        return head;
    }
    else
    {
        struct dic *tmp = (struct dic*)malloc(sizeof(struct dic));
        tmp->name = strdup(var);
        tmp->value = 0;
        tmp->next = NULL;
        tail->next = tmp;
        tail = tmp;
        return tmp;
    }
}
struct dic* findVar(char* var)
{
    struct dic *tmp;
    //printf("222\n");
    for(tmp = head;tmp!=NULL;tmp=tmp->next)
    {
        //printf("111\n");
        if(strcmp(tmp->name,var)==0)
        {   
            //printf("findla\n");
            return tmp;
        }
    }
    return addVar2Point(var);
}
int ex(nodeType *p) {
    int i;
    if (iscontinue||isbreak) return 0;  
    if (!p) return 0;
    switch(p->type) {
    case typeCon:       return p->con.value;
    case typeId:        return findVar(p->id.id)->value;
    case typeOpr:
        switch(p->opr.oper) {
    case BREAK: if(!loopcount) {printf("break must be inside a loop.\n");exit(0);} isbreak = 1; return 0;
    case CONTINUE: if(!loopcount) {printf("break must be inside a loop.\n");exit(0);} iscontinue = 1; return 0;
	case FOR:	ex(p->opr.op[0]);
            loopcount++;
			while (ex(p->opr.op[1])) {
				ex(p->opr.op[3]);
                if(isbreak)
                {
                    isbreak  = 0;
                    break;
                }
                if(iscontinue)
                {
                    iscontinue = 0;
                }
				ex(p->opr.op[2]);
			}
            loopcount--;
			return 0;
        case DO:        
                loopcount++;
                do{ex(p->opr.op[0]);if(isbreak)
                {
                    isbreak  = 0;
                    break;
                }
                if(iscontinue)
                {
                    iscontinue = 0;
                    continue;
                }}while(ex(p->opr.op[1])); 
                loopcount--;
                return 0;
        case WHILE:     loopcount++; while(ex(p->opr.op[0])) {ex(p->opr.op[1]);if(isbreak)
                {
                    isbreak  = 0;
                    break;
                }
                if(iscontinue)
                {
                    iscontinue = 0;
                    continue;
                }} loopcount--; return 0;
        case IF:        if (ex(p->opr.op[0]))
                            ex(p->opr.op[1]);
                        else if (p->opr.nops > 2)
                            ex(p->opr.op[2]);
                        return 0;
        case PRINT:     printf("%d\n", ex(p->opr.op[0])); return 0;
	case READ:	printf("? "); scanf("%d", &i);
			return findVar(p->opr.op[0]->id.id)->value = i;
        case ';':       ex(p->opr.op[0]); return ex(p->opr.op[1]);
        case '=':       return findVar(p->opr.op[0]->id.id)->value = ex(p->opr.op[1]);
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
