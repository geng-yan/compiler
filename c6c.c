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
static int in = 0; // a register temporarily holding the array index
static struct dic *head=NULL;
static struct arrayDic *arrayHead = NULL;
struct fun *fhead = NULL;
struct dic *global=NULL;
int outx=-1,outy=-1;
int flag = 0;
// enum type{INT,ARRAY}; // not sure whether this is used
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
    struct arrayDic *ref;
    struct dic *next;
};

struct arrayDic{
    char* name;
    struct width *dims;
    int type;
    // struct arrayDic *next;
};

struct width{
    int bound;
    struct width *next;
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
// <<<<<<< HEAD
//         head = (struct dic*)malloc(sizeof(struct dic));
//         head->name = strdup(var);
//         head->pos = fpoffset-1;
//         head->ischar = 0;
//         head->next = NULL;
//         head->t = INT;
//         tail = head;
//         return head;
// =======
        fhead = (struct fun*)malloc(sizeof(struct fun));
        fhead->name = strdup(name);
        fhead->para_num = 0;
        fhead->no = 0;
        fhead->next = NULL;
        tail = fhead;
        return fhead;
// >>>>>>> clara
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
// need to modify
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
        tmp->ref = NULL;
        tmp->next = NULL;
        // tmp->t = INT;
        tail->next = tmp;
        tail = tmp;
        return tmp;
    }
}

struct dic* findVar(char* var, struct dic* thead)
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
    else if(p->type==typeId || p->type==typeCon)
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
    else if(p->type==typeId || p->type==typeCon)
    {
        ex(opr(PIN,1,p));
    }
    else
    {
        parseputin(p->opr.op[0]);
        ex(opr(PIN,1,p->opr.op[1]));
    }
}
void parseputifm(nodeType *p1, nodeType *p2)
{
    if(p2==NULL)
    {
        printf("puti cannot receive NULL\n");
        exit(1);
    }
    else if(p2->type==typeId || p2->type==typeCon)
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
    else if(p->type==typeId || p->type==typeCha)
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
    else if(p->type==typeId || p->type==typeCha)
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
    else if(p2->type==typeId || p2->type==typeCon)
    {
        ex(opr(PC,2,p1,p2));
    }
    else
    {
        parseputcfm(p1,p2->opr.op[0]);
        ex(opr(PC,2,p1,p2->opr.op[1]));
    }
}
void parseputs(nodeType *p)
{
    if(p==NULL)
    {
        printf("puts cannot receive NULL\n");
        exit(1);
    }
    else if(p->type==typeId || p->type==typeString)
    {
        ex(opr(PS,1,p));
    }
    else
    {
        parseputs(p->opr.op[0]);
        ex(opr(PS,1,p->opr.op[1]));
    }
}
void parseputsn(nodeType *p)
{
    if(p==NULL)
    {
        printf("puts_ cannot receive NULL\n");
        exit(1);
    }
    else if(p->type==typeId | p->type==typeString)
    {
        ex(opr(PSN,1,p));
    }
    else
    {
        parseputsn(p->opr.op[0]);
        ex(opr(PSN,1,p->opr.op[1]));
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

struct arrayDic* newArray(char* name, int init, int type, struct width *dims){
    struct arrayDic *tmp = (struct arrayDic*)malloc(sizeof(struct arrayDic));
    tmp->name = strdup(name);
    tmp->dims = dims;
    tmp->type = type;
    int i = 0;

    fpoffset--;
    while (i<calcTotal(tmp->dims)){
        printf("\tpush\t%d\n", init);
        i++;
        ++fpoffset;
    }
    return tmp;
}

struct width* getDims(nodeType *p){
    struct width *head = (struct width*)malloc(sizeof(struct width));
    struct width *trailing;
    head->bound = p->con.value;
    switch(p->type){
        case typeCon:
            head->next = NULL;
            break;
        case typeOpr:
            trailing = getDims(p->opr.op[0]);
            head->next = trailing;
            break;
    }
    return head;
}

int calcOff(struct width *dims, struct width *actual){
    struct width *currDim = dims;
    struct width *currAct = actual;
    int off = 0;
    while (currDim->next != NULL){
        if (currAct == NULL){
            printf("Array Dimension Error\n");
            exit(-1);
        }
    }
}

int calcTotal(struct width *head){
    int prod = 1;
    struct width *curr = head;
    while (curr != NULL){
        prod *= curr->bound;
        curr = curr->next;
    }
    return prod;
}

int ex(nodeType *p) {
    int oldx,oldy,lbl1, lbl2,lbl3;
    int oldoutest;
    int oldoff = 0;
    struct dic *tmp;
    struct arrayDic *arrayTmp;

    struct dic *oldhead = NULL;
    struct fun *ftmp;
    if (!p) return 0;
    switch(p->type) {
    case typeCon:
        // printf("---typeCon---\n");       
        printf("\tpush\t%d\n", p->con.value);
        ischar = 0;
        ++fpoffset; 
        //break;
        return p->con.value;
    case typeCha:
        // printf("---typeCha---\n");
        printf("\tpush\t%d\n", p->cha.value);
        ischar = 1;
        ++fpoffset;//sad
        break;
    case typeString:
        printf("\tpush\t%s\n", p->strg.value);
        ++fpoffset;
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
            parseputcn(p->opr.op[0]);
            break;
        case PUTC:
            if(p->opr.nops==1)
                parseputc(p->opr.op[0]);
            else
                parseputcfm(p->opr.op[0],p->opr.op[1]);
            break;
        case PUTSN:
            parseputsn(p->opr.op[0]);
            break;
        case PUTS:
            if(p->opr.nops==1)
                parseputs(p->opr.op[0]);
            // TODO: formatted
            // else
            //     parseputcfm(p->opr.op[0],p->opr.op[1]);
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
        case ARRAY: // array declaration; single dimension
            flag = 0;
            ++fpoffset;
            if(p->opr.op[0]->id.global)
                tmp = findVar(p->opr.op[0]->id.id,NULL);
            else
                tmp = findVar(p->opr.op[0]->id.id,head);
            if (tmp != NULL){ // already declared
                printf("ERROR: duplicated array declaration: %s\n", p->opr.op[0]->id.id);
                // execution should stop here so the value of fp is no longer relevant
                exit(-1);
            }
            if(outest)
                tmp = addVar2Point(p->opr.op[0]->id.id, &global);
            else
                tmp = addVar2Point(p->opr.op[0]->id.id, &head);
            // int dim = p->opr.op[1]->con.value;
            if(p->opr.nops == 2){ // without initialization
                // default init value is 0; "type" is 0: the type is not determined yet
                tmp->ref = newArray(p->opr.op[0]->id.id, 0, 0, getDims(p->opr.op[1]));
            }
            else{ // with initialization
                switch(p->opr.op[3]->con.value){
                    case 1: // integer array
                        tmp->ref = newArray(p->opr.op[0]->id.id, p->opr.op[2]->con.value, 1, getDims(p->opr.op[1]));
                        break;
                    case 2: // char array, i.e. "real string"
                        tmp->ref = newArray(p->opr.op[0]->id.id, p->opr.op[2]->cha.value, 2, getDims(p->opr.op[1]));
                        break;
                }
            }        
            //fpoffset += (dim-1); // reserve space for array
            fpoffset += calcTotal(getDims(p->opr.op[1])) - 1;
            break;
        case ACCESS: // VARIABLE '[' expr ']'
            flag = 0;
            if(p->opr.op[0]->id.global)
                tmp = findVar(p->opr.op[0]->id.id,NULL);
            else
                tmp = findVar(p->opr.op[0]->id.id,head);
            if (tmp == NULL){
                printf("ERROR: array element referenced before declaration.\n");
                exit(1);
            }
            ex(p->opr.op[1]);
            printf("\tpush\t%d\n", tmp->pos);
            printf("\tadd\n");
            printf("\tpop\tin\n");
            printf("\tpush\tfp[in]\n");
            break;
            //printf("\tpush\tfp[%d]\n", tmp->pos + p->opr.op[1]->con.value);
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
                    if(!outest)
                        tmp = addVar2Point(p->opr.op[0]->id.id,&head);
                    else
                        tmp = addVar2Point(p->opr.op[0]->id.id,&global);
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
                    if(!outest)
                        tmp = addVar2Point(p->opr.op[0]->id.id,&head);
                    else
                        tmp = addVar2Point(p->opr.op[0]->id.id,&global);
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
            //ischar = 1;
            break;
        case PI:
            if(p->opr.nops==1) // simple
                ex(p->opr.op[0]);
            else // formatted
                ex(p->opr.op[1]);
            --fpoffset;
            if(p->opr.nops==1)
                printf("\tputi\n");
            else
                printf("\tputi %s\n",p->opr.op[0]->strg.value);
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
                printf("\tputc %s\n",p->opr.op[0]->strg.value);
            break;
        case PCN:
            ex(p->opr.op[0]);
            --fpoffset;
            printf("\tputc_\n");
            break;
        // added
        case PS:
            if(p->opr.nops==1)
                ex(p->opr.op[0]);
            else
                ex(p->opr.op[1]);
            --fpoffset;
            if(p->opr.nops==1)
                printf("\tputs\n");
            // TODO: formatted
            // else
            //     printf("\tputc %s\n",p->opr.op[0]->id.id);
            break;
        case PSN:
            ex(p->opr.op[0]);
            --fpoffset;
            printf("\tputs_\n");
            break;
        case '=':       
            ex(p->opr.op[1]); // RHS value
            nodeType *vartmp;
            int isArray;
            if (p->opr.op[0]->type == typeId){
                vartmp = p->opr.op[0];  // simple variable
                isArray = 0;
            }
            else{
                vartmp = p->opr.op[0]->opr.op[0]; // array element
                isArray = 1;
            }
            // common
            if(vartmp->id.global)
                tmp = findVar(vartmp->id.id,NULL);
            else
                tmp = findVar(vartmp->id.id,head);
            if(tmp == NULL) // variable not found
            {  
                if (isArray == 1){
                    printf("ERROR: array element referenced before declaration.\n");
                    exit(1);
                }
                if(vartmp->id.global && outest==0)
                {
                    printf("cannot find that global variable.\n");
                    exit(1);
                }
                else
                {
                    if(outest)
                        tmp = addVar2Point(vartmp->id.id, &global);
                    else
                        tmp = addVar2Point(vartmp->id.id, &head);
                }
            }
            if(!flag){ // the variable is not newly created
                --fpoffset;
                 //var type changed. 
                tmp->ischar = ischar; // Disable and exit if type change is not allowed.
            }
            else{
                ex(p->opr.op[1]);
                --fpoffset;
                tmp->ischar = ischar;
                if(vartmp->id.global)
                    tmp = findVar(vartmp->id.id, NULL);
                else
                    tmp = findVar(vartmp->id.id, head);
            }
            if(isArray == 0){
                if(!isg)
                    printf("\tpop\tfp[%d]\n",tmp->pos);
                else
                    printf("\tpop\tsb[%d]\n",tmp->pos);
            }
            else{
                if (tmp->ref == NULL){
                    printf("TYPE ERROR: %s is not an array\n", tmp->name);
                    exit(-1);
                }
                ex(p->opr.op[0]->opr.op[1]);
                printf("\tpush\t%d\n", tmp->pos);
                printf("\tadd\n");
                printf("\tpop\tin\n");
                if(!isg)
                    printf("\tpop\tfp[in]\n");
                else
                    printf("\tpop\tsb[in]\n");
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
            ex(p->opr.op[0]); // deleted: int aischar = ischar;
            ex(p->opr.op[1]); int charopr = ischar; ischar = 0;
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

