#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "mycc.h"

// グローバル変数嫌い
Token *token;
char *user_input;
Node *code[100];
LVar *locals;

bool consume(char *op) {
    if (token->type != TK_RESERVED || strlen(op) != token->len || memcmp(token->str, op, token->len)) {
        return false;
    }
    token = token->next;
    return true;
}

bool consume_return() {
    if (token->type != TK_RETURN) {
        return false;
    }
    token = token->next;
    return true;
}

bool consume_if() {
    if (token->type != TK_IF) {
        return false;
    }
    token = token->next;
    return true;
}

bool consume_else() {
    if (token->type != TK_ELSE) {
        return false;
    }
    token = token->next;
    return true;
}

bool consume_while() {
    if (token->type != TK_WHILE) {
        return false;
    }
    token = token->next;
    return true;
}

bool consume_for() {
    if (token->type != TK_FOR) {
        return false;
    }
    token = token->next;
    return true;
}

Token *consume_ident() {
    if (token->type != TK_IDENT) {
        return NULL;
    }
    Token *token_ = token;
    token = token->next;
    return token_;
}

void expect(char *op) {
    if (token->type != TK_RESERVED || strlen(op) != token->len || memcmp(token->str, op, token->len)) {
        error_at(token->str, "\"%s\" ではありません", op);
    }
    token = token->next;
}

int expect_number() {
    if (token->type != TK_NUM) {
        error_at(token->str, "数ではありません");
    }
    int val = token->val;
    token = token->next;
    return val;
}

bool at_eof() {
    return token->type == TK_EOF;
}

Token *new_token(TokenType type, Token *cur, char *str, int len) {
    Token *tok = calloc(1, sizeof(Token));
    tok->type = type;
    tok->str = str;
    tok->len = len;
    cur->next = tok;
    return tok;
}

int is_alnum(char c) {
  return 'a' <= c && c <= 'z' || 'A' <= c && c <= 'Z' || isdigit(c) || c == '_';
}

Token *tokenize(char *p) {
    Token head;
    head.next = NULL;
    Token *cur = &head;

    while (*p) {
        if (isspace(*p)) {
            ++p;
        } else if (strncmp(p, "==", 2) == 0 || strncmp(p, "!=", 2) == 0 || strncmp(p, "<=", 2) == 0 || strncmp(p, ">=", 2) == 0) {
            cur = new_token(TK_RESERVED, cur, p, 2);
            p += 2;
        } else if (*p == '+' || *p == '-' || *p == '*' || *p == '/' ||
                   *p == '(' || *p == ')' || *p == '<' || *p == '>' ||
                   *p == '=' || *p == ';') {
            cur = new_token(TK_RESERVED, cur, p, 1);
            p += 1;
        } else if (strncmp(p, "return", 6) == 0 && !is_alnum(p[6])) {
            cur = new_token(TK_RETURN, cur, p, 6);
            p += 6;
        } else if (strncmp(p, "if", 2) == 0 && !is_alnum(p[2])) {
            cur = new_token(TK_IF, cur, p, 2);
            p += 2;
        } else if (strncmp(p, "else", 4) == 0 && !is_alnum(p[4])) {
            cur = new_token(TK_ELSE, cur, p, 4);
            p += 4;
        } else if (strncmp(p, "while", 5) == 0 && !is_alnum(p[5])) {
            cur = new_token(TK_WHILE, cur, p, 5);
            p += 5;
        } else if (strncmp(p, "for", 3) == 0 && !is_alnum(p[3])) {
            cur = new_token(TK_FOR, cur, p, 3);
            p += 3;
        } else if ('a' <= *p && *p <= 'z') {
            int i = 1;
            for (; is_alnum(p[i]); ++i) {}
            cur = new_token(TK_IDENT, cur, p, i);
            p += i;
        } else if (isdigit(*p)) {
            cur = new_token(TK_NUM, cur, p, 0);
            cur->val = strtol(p, &p, 10);
        } else {
            error_at(token->str, "トークナイズできません");
        }
    }

    new_token(TK_EOF, cur, p, 0);
    return head.next;
}

LVar *find_lvar(Token *tok) {
    for (LVar *var = locals; var; var = var->next) {
        if (var->len == tok->len && memcmp(tok->str, var->name, var->len) == 0) {
            return var;
        }
    }
    return NULL;
}

Node *new_node(NodeType type, Node *lhs, Node *rhs) {
    Node *node = calloc(1, sizeof(Node));
    node->type = type;
    node->lhs = lhs;
    node->rhs = rhs;
    return node;
}

Node *new_node_num(int val) {
    Node *node = calloc(1, sizeof(Node));
    node->type = ND_NUM;
    node->val = val;
    return node;
}
