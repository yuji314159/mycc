#include <stdbool.h>

typedef enum {
    TK_RESERVED,
    TK_RETURN,
    TK_IF,
    TK_ELSE,
    TK_WHILE,
    TK_FOR,
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
    ND_RETURN,
    ND_IF,
    ND_WHILE,
    ND_FOR,
} NodeType;

typedef struct Node Node;

struct Node {
    NodeType type;
    Node *lhs;
    Node *rhs;
    Node *init;
    Node *cond;
    Node *inc;
    Node *then;
    Node *els;
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

bool consume(char *op);
bool consume_return();
bool consume_if();
bool consume_else();
bool consume_while();
bool consume_for();
Token *consume_ident();
void expect(char *op);
int expect_number();
bool at_eof();
Token *new_token(TokenType type, Token *cur, char *str, int len);
Token *tokenize(char *p);
LVar *find_lvar(Token *tok);
Node *new_node(NodeType type, Node *lhs, Node *rhs);
Node *new_node_num(int val);

void program();

void gen(Node *node);
