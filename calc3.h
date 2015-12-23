typedef enum { typeCon, typeCha, typeString, typeId, typeDim, typeOpr } nodeEnum;

/* constants */
typedef struct {
    int value;                  /* value of constant */
} conNodeType;

typedef struct {
    int value;
} chaNodeType;

// added on 20th: STRING
typedef struct {
    char* value;
} strgNodeType;

typedef struct constTag
{
    nodeEnum type;

    union {
        conNodeType con;        /* constants */
        chaNodeType cha;       /* charactors */
        strgNodeType strg;
    };
} nodeConst;

/* identifiers */
typedef struct {
    char* id;                      /* subscript to sym array */
    int global;
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
        strgNodeType strg;
        idNodeType id;          /* identifiers */
        oprNodeType opr;        /* operators */
    };
} nodeType;
