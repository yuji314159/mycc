#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include "mycc.h"

void error(char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    fprintf(stderr, "\n");
    exit(1);
}

void error_at(char *loc, char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);

    int pos = loc - user_input;
    fprintf(stderr, "%s\n", user_input);
    fprintf(stderr, "%*s", pos, "");
    fprintf(stderr, "^ ");
    vfprintf(stderr, fmt, ap);
    fprintf(stderr, "\n");
    exit(1);
}

int len_locals() {
    int len = 0;
    for (LVar *var = locals; var; var = var->next) {
        len += 1;
    }
    return len;
}

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "引数の数が正しくありません\n");
        return 1;
    }

    user_input = argv[1];
    token = tokenize(argv[1]);
    // fprintf(stderr, "# tokenize OK\n");
    program();
    // fprintf(stderr, "# program OK\n");

    printf(".intel_syntax noprefix\n");
    printf(".global main\n");
    printf("main:\n");

    printf("    push rbp\n");
    printf("    mov rbp, rsp\n");
    printf("    sub rsp, %d\n", 8 * len_locals());

    for (int i = 0; code[i]; ++i) {
        gen(code[i]);
        printf("    pop rax\n");
    }

    printf("    mov rsp, rbp\n");
    printf("    pop rbp\n");
    printf("    ret\n");

    return 0;
}
