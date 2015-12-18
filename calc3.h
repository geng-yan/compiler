typedef enum { typeCon, typeCha, typeId, typeOpr } nodeEnum;

/* constants */
typedef struct {
    int value;                  /* value of constant */
} conNodeType;

typedef struct {
    int value;
} chaNodeType;

/* identifiers */
typedef struct {
    char* id;                      /* subscript to sym array */
} idNodeType;

/* operators */
typedef struct {
    int oper;                   /* operator */
    int nops;                   /* number of operands */
    struct nodeTypeTag *op[1];  /* operands (expandable) */
} oprNodeType;

typedef struct nodeTypeTag {
    nodeEnum type;              /* type of node */

    /* union must be last entry in nodeType */
    /* because operNodeType may dynamically increase */
    union {
        conNodeType con;        /* constants */
        chaNodeType cha;       /* charactors */
        idNodeType id;          /* identifiers */
        oprNodeType opr;        /* operators */
    };
} nodeType;
