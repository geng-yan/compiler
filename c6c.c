#include <stdio.h>
#include <string.h>
#include "calc3.h"
#include "y.tab.h"
#include <stdlib.h>
//typedef enum {INTEGER,CHAR,STRING} printype;
static int lbl;
static int fpoffset = 0;
static struct dic *head=NULL;
int outx=-1,outy=-1;
int flag = 0;
int ischar = 1;
extern int yylineno;
//printype typ;
struct dic{
    char *name;
    int pos;
    int ischar;
    struct dic *next;
};
struct dic* addVar2Point(char* var)
{
    static struct dic *tail = NULL;
    //printf("---%s---%d\n",var,fpoffset-1);
    if(head==NULL)
    {
        head = (struct dic*)malloc(sizeof(struct dic));
        head->name = strdup(var);
        head->pos = fpoffset-1;
        head->ischar = 0;
        head->next = NULL;
        tail = head;
        return head;
    }
    else
    {
        struct dic *tmp = (struct dic*)malloc(sizeof(struct dic));
        tmp->name = strdup(var);
        tmp->pos = fpoffset-1;
        tmp->ischar = 0;
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
            flag = 0;
            return tmp;
        }
    }
    flag = 1;
    return addVar2Point(var);
}
int ex(nodeType *p) {
    int oldx,oldy,lbl1, lbl2,lbl3;
    struct dic *tmp;
    if (!p) return 0;
    switch(p->type) {
    case typeCon:
        // printf("---typeCon---\n");       
        printf("\tpush\t%d\n", p->con.value);
        ischar = 0;
        ++fpoffset; 
        break;
    case typeCha:
        // printf("---typeCha---\n");
        printf("\tpush\t%d\n", p->cha.value);
        ischar = 1;
        ++fpoffset;
        break;
    case typeId:
        // printf("---typeId:%s---\n",p->id.id);
        tmp = findVar(p->id.id);        
        printf("\tpush\tfp[%d]\n", tmp->pos);
        ischar = tmp->ischar;
        ++fpoffset;
        break;
    case typeOpr:
        // printf("---typeOpr---\n");
        switch(p->opr.oper) {
        case BREAK:
            if(outy!=-1)
                printf("\tjmp\tL%03d\n",outy);
            else
            {
                printf("break must be inside a loop.\n");
                exit(0);
            }
            break;
        case CONTINUE:
            if(outx!=-1)
                printf("\tjmp\tL%03d\n",outx);
            else
            {
                printf("continue must be inside a loop.\n");
                exit(0);
            }
            break;
    	case FOR:
            lbl1 = lbl++;
            lbl2 = lbl++;
            lbl3 = lbl++;
            oldx = outx;
            oldy = outy;
            outx = lbl3;
            outy = lbl2;
    		ex(p->opr.op[0]);
    		printf("L%03d:\n", lbl1);
    		ex(p->opr.op[1]);
    		printf("\tj0\tL%03d\n", lbl2);
            --fpoffset;
    		ex(p->opr.op[3]);
            printf("L%03d:\n", lbl3);
    		ex(p->opr.op[2]);
    		printf("\tjmp\tL%03d\n", lbl1);
    		printf("L%03d:\n", lbl2);
            outx = oldx;
            outy = oldy;
    		break;
        case WHILE:
            lbl1 = lbl++;
            lbl2 = lbl++;
            oldx = outx;
            oldy = outy;
            outx = lbl1;
            outy = lbl2;
            printf("L%03d:\n", lbl1);
            ex(p->opr.op[0]);
            printf("\tj0\tL%03d\n", lbl2);
            --fpoffset;
            ex(p->opr.op[1]);
            printf("\tjmp\tL%03d\n", lbl1);
            printf("L%03d:\n", lbl2);
            outx = oldx;
            outy = oldy;    
            break;
        case DO:
            lbl1 = lbl++;
            lbl2 = lbl++;
            oldx = outx;
            oldy = outy;
            outx = lbl1;
            outy = lbl2;
            printf("L%03d:\n",lbl1);
            ex(p->opr.op[0]);
            ex(p->opr.op[1]);
            printf("\tj0\tL%03d\n",lbl2);
            --fpoffset;
            printf("\tjmp\tL%03d\n",lbl1);
            printf("L%03d:\n",lbl2);
            outx = oldx;
            outy = oldy;
            break;
        case IF:
            ex(p->opr.op[0]);
            if (p->opr.nops > 2) {
                /* if else */
                printf("\tj0\tL%03d\n", lbl1 = lbl++);
                --fpoffset;
                ex(p->opr.op[1]);
                printf("\tjmp\tL%03d\n", lbl2 = lbl++);
                printf("L%03d:\n", lbl1);
                ex(p->opr.op[2]);
                printf("L%03d:\n", lbl2);
            } else {
                /* if */
                printf("\tj0\tL%03d\n", lbl1 = lbl++);
                --fpoffset;
                ex(p->opr.op[1]);
                printf("L%03d:\n", lbl1);
            }
            break;
    	case READ:
            flag = 0;
            tmp = findVar(p->opr.op[0]->id.id);
            if(!flag)
            {
                printf("\tgeti\n");
                printf("\tpop\tfp[%d]\n",tmp->pos);
            }
            else
            {
                printf("\tpush 0\n");
                ++fpoffset;
                ++tmp->pos;
                ex(p->opr.op[0]);
                printf("\tgeti\n");
                printf("\tpop\tfp[%d]\n",tmp->pos);
            }
    	    break;
        case PRINT:     
            ex(p->opr.op[0]);
            --fpoffset;
            if (ischar) printf("\tputc\n");
            else printf("\tputi\n");
            ischar = 1;
            break;
        case '=':       
            ex(p->opr.op[1]);
            tmp = findVar(p->opr.op[0]->id.id);
            if(!flag)
            {
                --fpoffset;
                tmp->ischar = ischar; // Disable and exit if type change is not allowed.
                printf("\tpop\tfp[%d]\n",tmp->pos);
            }
            else
            {
                ex(p->opr.op[1]);
                --fpoffset;
                tmp->ischar = ischar;
                printf("\tpop\tfp[%d]\n",tmp->pos);
            }
            break;
        case UMINUS:    
            ex(p->opr.op[0]);
            ischar = 0;
            printf("\tneg\n");
            break;
        default:
            ex(p->opr.op[0]); int aischar = ischar;
            ex(p->opr.op[1]); int charopr = ischar || aischar; ischar = 0;
                switch(p->opr.oper) { 
                case '+':   printf("\tadd\n"); --fpoffset; break;
                case '-':   printf("\tsub\n"); --fpoffset; break; 
                case '*':   printf("\tmul\n"); --fpoffset;break;
                case '/':   printf("\tdiv\n"); --fpoffset;break;
                case '%':   printf("\tmod\n"); --fpoffset;break;
                case '<':   printf("\tcomplt\n"); --fpoffset;break;
                case '>':   printf("\tcompgt\n"); --fpoffset;break;
                case GE:    printf("\tcompge\n"); --fpoffset;break;
                case LE:    printf("\tcomple\n"); --fpoffset;break;
                case NE:    printf("\tcompne\n"); --fpoffset;break;
                case EQ:    printf("\tcompeq\n"); --fpoffset;break;
    	    case AND:   printf("\tand\n"); --fpoffset;break;
    	    case OR:    printf("\tor\n"); --fpoffset;break;
                }
            }
    }
    return 0;
}

