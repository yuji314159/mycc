typedef enum {
    TK_RESERVED,
    TK_IDENT,
    TK_NUM,
    TK_EOF,
} TokenType;

typedef struct Token Token;

struct Token {
    TokenType type;
    Token *next;
    int val;
    char *str;
    int len;
};

typedef enum {
    ND_ADD,
    ND_SUB,
    ND_MUL,
    ND_DIV,
    ND_EQ,
    ND_NE,
    ND_LT,
    // ND_GT,
    ND_LE,
    // ND_GE,
    ND_NUM,
    ND_ASSIGN,
    ND_LVAR,
} NodeType;

typedef struct Node Node;

struct Node {
    NodeType type;
    Node *lhs;
    Node *rhs;
    int val;
    int offset;
};

typedef struct LVar LVar;

struct LVar {
    LVar *next;
    char *name;
    int len;
    int offset;
};

extern Token *token;
extern char *user_input;
extern Node *code[100];
extern LVar *locals;

void error(char *fmt, ...);
void error_at(char *loc, char *fmt, ...);

Token *tokenize(char *p);
void program();

void gen(Node *node);
