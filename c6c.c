#include <stdio.h>
#include <string.h>
#include "calc3.h"
#include "y.tab.h"
#include <stdlib.h>
//typedef enum {INTEGER,CHAR,STRING} printype;
int ex(nodeType*);
extern nodeType* opr(int oper, int nops, ...) ;
static int lbl;
static int fpoffset = 0;
static struct dic *head=NULL;
struct fun *fhead = NULL;
struct dic *global=NULL;
int outx=-1,outy=-1;
int flag = 0;
int ischar = 1;
int fnum = 0;
int outest = 1;
int isg = 0;
int canadd = 0;
int para = 0;
extern int yylineno;
//printype typ;
struct dic{
    char *name;
    int pos;
    int ischar;
    struct dic *next;
};
struct fun{
    char *name;
    int no;
    int para_num;
    struct fun *next;
};
struct fun* addFun(char* name)
{
    static struct fun *tail = NULL;
    if(fhead == NULL)
    {
        fhead = (struct fun*)malloc(sizeof(struct fun));
        fhead->name = strdup(name);
        fhead->para_num = 0;
        fhead->no = 0;
        fhead->next = NULL;
        tail = fhead;
        return fhead;
    }
    else
    {
        struct fun *tmp = (struct fun*)malloc(sizeof(struct fun));
        tmp->name = strdup(name);
        tmp->no = 0;
        tmp->para_num = 0;
        tmp->next = NULL;
        tail->next = tmp;
        tail = tmp;
        return tmp;
    }
}
struct fun* findFun(char* name)
{
    struct fun *tmp;
    for(tmp = fhead;tmp!=NULL;tmp=tmp->next)
    {
        if(strcmp(tmp->name,name)==0)
            return tmp;
    }
    return NULL;
}
struct dic* addVar2Point(char* var,struct dic** thead)
{
    //printf("---%s---%d\n",var,fpoffset-1);
    if(*thead==NULL)
    {
        *thead = (struct dic*)malloc(sizeof(struct dic));
        (*thead)->name = strdup(var);
        (*thead)->pos = fpoffset-1;
        (*thead)->ischar = 0;
        (*thead)->next = NULL;
        return (*thead);
    }
    struct dic *tail = *thead;
    while(tail->next!=NULL) tail=tail->next;
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

struct dic* findVar(char* var,struct dic* thead)
{
    struct dic *tmp;
    //printf("222\n");
    if(thead)
        for(tmp = thead;tmp!=NULL;tmp=tmp->next)
        {
        //printf("111\n");
            if(strcmp(tmp->name,var)==0)
            {   
            //printf("findla\n");
                flag = 0;
                isg = 0;
                return tmp;
            }
        }
    else
        for(tmp = global;tmp!=NULL;tmp=tmp->next)
        {
            if(strcmp(tmp->name,var)==0)
            {      
                //printf("findla\n");
                flag = 0;
                isg = 1;
                return tmp;
            }
        }
    flag = 1;
    if(outest) isg = 1;
    else isg = 0;
    return NULL;
}
void parsegeti(nodeType *p)
{
    if(p==NULL)
    {
        printf("geti cannot receive NULL\n");
        exit(1);
    }
    else if(p->type==typeId)
    {
        ex(opr(READ,1,p));
    }
    else
    {
        parsegeti(p->opr.op[0]);
        ex(opr(READ,1,p->opr.op[1]));
    }
}
void parsegetc(nodeType *p)
{
    if(p==NULL)
    {
        printf("getc cannot receive NULL\n");
        exit(1);
    }
    else if(p->type==typeId)
    {
        ex(opr(READC,1,p));
    }
    else
    {
        parsegetc(p->opr.op[0]);
        ex(opr(READC,1,p->opr.op[1]));
    }
}
void parseputi(nodeType *p)
{
    if(p==NULL)
    {
        printf("puti cannot receive NULL\n");
        exit(1);
    }
    else if(p->type==typeId)
    {
        ex(opr(PI,1,p));
    }
    else
    {
        parseputi(p->opr.op[0]);
        ex(opr(PI,1,p->opr.op[1]));
    }
}
void parseputin(nodeType *p)
{
    if(p==NULL)
    {
        printf("puti_ cannot receive NULL\n");
        exit(1);
    }
    else if(p->type==typeId)
    {
        ex(opr(PIN,1,p));
    }
    else
    {
        parseputin(p->opr.op[0]);
        ex(opr(PIN,1,p->opr.op[1]));
    }
}
void parseputifm(nodeType *p1,nodeType *p2)
{
    if(p2==NULL)
    {
        printf("puti cannot receive NULL\n");
        exit(1);
    }
    else if(p2->type==typeId)
    {
        ex(opr(PI,2,p1,p2));
    }
    else
    {
        parseputifm(p1,p2->opr.op[0]);
        ex(opr(PI,2,p1,p2->opr.op[1]));
    }
}
void parseputc(nodeType *p)
{
    if(p==NULL)
    {
        printf("putc cannot receive NULL\n");
        exit(1);
    }
    else if(p->type==typeId)
    {
        ex(opr(PC,1,p));
    }
    else
    {
        parseputc(p->opr.op[0]);
        ex(opr(PC,1,p->opr.op[1]));
    }
}
void parseputcn(nodeType *p)
{
    if(p==NULL)
    {
        printf("putc_ cannot receive NULL\n");
        exit(1);
    }
    else if(p->type==typeId)
    {
        ex(opr(PCN,1,p));
    }
    else
    {
        parseputcn(p->opr.op[0]);
        ex(opr(PCN,1,p->opr.op[1]));
    }
}
void parseputcfm(nodeType *p1,nodeType *p2)
{
    if(p2==NULL)
    {
        printf("putc cannot receive NULL\n");
        exit(1);
    }
    else if(p2->type==typeId)
    {
        ex(opr(PC,2,p1,p2));
    }
    else
    {
        parseputcfm(p1,p2->opr.op[0]);
        ex(opr(PC,2,p1,p2->opr.op[1]));
    }
}
int addPara(nodeType *p)
{
    int num=0;
    if(p->type==typeOpr)
    {
        num+=addPara(p->opr.op[0]);
        if(p->opr.op[1]->type==typeId)
            addVar2Point(p->opr.op[1]->id.id,&head);
        else
        {
            printf("the argument list must be the variables only.\n");
            exit(1);
        }
        ++num;
    }
    else if(p->type==typeId)
    {
        addVar2Point(p->id.id,&head);
        ++num;
    }
    else
    {
        printf("the argument list must be the variables only.\n");
        exit(1);
    }
    return num;

}
int ex(nodeType *p) {
    int oldx,oldy,lbl1, lbl2,lbl3;
    int oldoutest;
    int oldoff = 0;
    struct dic *tmp;
    struct dic *oldhead = NULL;
    struct fun *ftmp;
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
        ++fpoffset;//sad
        break;
    case typeId:
        // printf("---typeId:%s---\n",p->id.id);
        if(p->id.global)
        {
            tmp = findVar(p->id.id,NULL);
        }
        else
            tmp = findVar(p->id.id,head);
        if(tmp == NULL)
        {
            printf("cannot be refer since it does not exist.\n");
            exit(1);
        }
        if(!isg)        
            printf("\tpush\tfp[%d]\n", tmp->pos);
        else
            printf("\tpush\tsb[%d]\n", tmp->pos);
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
        case GETI:
            parsegeti(p->opr.op[0]);
            break;
        case GETC:
            parsegetc(p->opr.op[0]);
            break;
        case PUTIN:
            parseputin(p->opr.op[0]);
            break;
        case PUTI:
            if(p->opr.nops==1)
                parseputi(p->opr.op[0]);
            else
                parseputifm(p->opr.op[0],p->opr.op[1]);
            break;
        case PUTCN:
            parseputc(p->opr.op[0]);
            break;
        case PUTC:
            if(p->opr.nops==1)
                parseputc(p->opr.op[0]);
            else
                parseputcfm(p->opr.op[0],p->opr.op[1]);
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
        case RETURN:
            ex(p->opr.op[0]);
            printf("\tret\n");
            break;
    	case READ:
            flag = 0;
            if(p->opr.op[0]->id.global)
                tmp = findVar(p->opr.op[0]->id.id,NULL);
            else
                tmp = findVar(p->opr.op[0]->id.id,head);
            if(tmp == NULL)
            {
                if(p->opr.op[0]->id.global&&outest==0)
                {
                    printf("cannot find that global variable.\n");
                    exit(1);
                }
                else
                {
                    tmp = addVar2Point(p->opr.op[0]->id.id,&head);
                }
            }
            tmp->ischar = 0;
            if(!flag)
            {
                printf("\tgeti\n");
                if(!isg)
                    printf("\tpop\tfp[%d]\n",tmp->pos);
                else
                    printf("\tpop\tsb[%d]\n",tmp->pos);
            }
            else
            {
                printf("\tpush 0\n");
                ++fpoffset;
                ++tmp->pos;
                //ex(p->opr.op[0]);
                printf("\tgeti\n");
                if(p->opr.op[0]->id.global)
                    tmp = findVar(p->opr.op[0]->id.id,NULL);
                else
                    tmp = findVar(p->opr.op[0]->id.id,head);
                if(!isg)
                    printf("\tpop\tfp[%d]\n",tmp->pos);
                else
                    printf("\tpop\tsb[%d]\n",tmp->pos);
            }
    	    break;
        case READC:
            flag = 0;
            if(p->opr.op[0]->id.global)
                tmp = findVar(p->opr.op[0]->id.id,NULL);
            else
                tmp = findVar(p->opr.op[0]->id.id,head);
            if(tmp == NULL)
            {
                if(p->opr.op[0]->id.global&&outest==0)
                {
                    printf("cannot find that global variable.\n");
                    exit(1);
                }
                else
                {
                    tmp = addVar2Point(p->opr.op[0]->id.id,&head);
                }
            }
            tmp->ischar = 1;
            if(!flag)
            {
                printf("\tgetc\n");
                if(!isg)
                    printf("\tpop\tfp[%d]\n",tmp->pos);
                else
                    printf("\tpop\tsb[%d]\n",tmp->pos);
            }
            else
            {
                printf("\tpush 0\n");
                ++fpoffset;
                ++tmp->pos;
                //ex(p->opr.op[0]);
                printf("\tgetc\n");
                if(p->opr.op[0]->id.global)
                    tmp = findVar(p->opr.op[0]->id.id,NULL);
                else
                    tmp = findVar(p->opr.op[0]->id.id,head);
                if(!isg)
                    printf("\tpop\tfp[%d]\n",tmp->pos);
                else
                    printf("\tpop\tsb[%d]\n",tmp->pos);
            }
            break;   
        case PRINT:     
            ex(p->opr.op[0]);
            --fpoffset;
            if (ischar) printf("\tputc\n");
            else printf("\tputi\n");
            ischar = 1;
            break;
        case PI:
            if(p->opr.nops==1)
                ex(p->opr.op[0]);
            else
                ex(p->opr.op[1]);
            --fpoffset;
            if(p->opr.nops==1)
                printf("\tputi\n");
            else
                printf("\tputi %s\n",p->opr.op[0]->id.id);
            break;
        case PIN:
            ex(p->opr.op[0]);
            --fpoffset;
            printf("\tputi_\n");
            break;
        case PC:
            if(p->opr.nops==1)
                ex(p->opr.op[0]);
            else
                ex(p->opr.op[1]);
            --fpoffset;
            if(p->opr.nops==1)
                printf("\tputc\n");
            else
                printf("\tputc %s\n",p->opr.op[0]->id.id);
            break;
        case PCN:
            ex(p->opr.op[0]);
            --fpoffset;
            printf("\tputc_\n");
            break;
        case '=':       
            ex(p->opr.op[1]);
            if(p->opr.op[0]->id.global)
                tmp = findVar(p->opr.op[0]->id.id,NULL);
            else
                tmp = findVar(p->opr.op[0]->id.id,head);
            if(tmp == NULL)
            {
                if(p->opr.op[0]->id.global&&outest==0)
                {
                    printf("cannot find that global variable.\n");
                    exit(1);
                }
                else
                {
                    if(outest)
                        tmp = addVar2Point(p->opr.op[0]->id.id,&global);
                    else
                        tmp = addVar2Point(p->opr.op[0]->id.id,&head);
                }
            }
            if(!flag)
            {
                --fpoffset;
                 //var type changed. 
                tmp->ischar = ischar; // Disable and exit if type change is not allowed.
                if(!isg)
                    printf("\tpop\tfp[%d]\n",tmp->pos);
                else
                    printf("\tpop\tsb[%d]\n",tmp->pos);
            }
            else
            {
                ex(p->opr.op[1]);
                --fpoffset;
                tmp->ischar = ischar;
                if(p->opr.op[0]->id.global)
                    tmp = findVar(p->opr.op[0]->id.id,NULL);
                else
                    tmp = findVar(p->opr.op[0]->id.id,head);
                if(!isg)
                    printf("\tpop\tfp[%d]\n",tmp->pos);
                else
                    printf("\tpop\tsb[%d]\n",tmp->pos);
            }
            break;
        case FUNCALL:
            ex(p->opr.op[1]);
            ftmp = findFun(p->opr.op[0]->id.id);
            printf("\tcall L%03d,%d\n",ftmp->no,ftmp->para_num);
            break;

        case FUNDCLR:
            oldoutest = outest;
            outest = 0;
            oldoff = fpoffset;
            fpoffset = 0;
            lbl1 = lbl++;
            oldhead = head;
            head = NULL;
            printf("\tjmp L%03d\n",lbl1);
            ftmp = findFun(p->opr.op[0]->id.id);
            if(ftmp!=NULL)
            {
                printf("Has been declared;\n");
                exit(1);
            }
            ftmp = addFun(p->opr.op[0]->id.id);
            ftmp->no = lbl++;
            printf("L%03d:\n",ftmp->no);
            if(p->opr.op[1]==NULL)
            {

            }
            else
            {
                nodeType *ptmp = p->opr.op[1];
                para = 1;
                while(ptmp->type==typeOpr)
                {
                    addVar2Point(ptmp->opr.op[1]->id.id,&head);
                    ptmp = ptmp->opr.op[0];
                    para++;
                }
                addVar2Point(ptmp->id.id,&head);

                para = addPara(p->opr.op[1]);
            }
            int itmp = 0;
            for(tmp = head;tmp!=NULL;tmp = tmp->next)
            {
                tmp->pos -= (3+itmp);
                itmp++;
            }
            ftmp->para_num = para;
            ex(p->opr.op[2]);
            printf("L%03d:\n",lbl1);
            outest = oldoutest;
            head = oldhead;
            fpoffset = oldoff;
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
                case ',':   para++; break;
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

