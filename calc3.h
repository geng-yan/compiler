typedef enum { typeCon, typeCha, typeString, typeId, typeArray, typeDim, typeOpr } nodeEnum;

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
    int offset;
} idNodeType;

/* node type for recording the dimension of array */
// typedef struct {
//     vector<int> dims;
// } dimNodeType;

typedef struct {
    char* id;
    int base;
    int ndim;
    // std::vector<int> dims;
} arrayNodeType;

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
        arrayNodeType array;
        oprNodeType opr;        /* operators */
    };
} nodeType;
