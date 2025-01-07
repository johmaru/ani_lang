#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    void **data;
    int capacity;
    int len;
} Vector;

Vector *new_vector();
void vec_push(Vector *vec, void *elem);
void *vec_get(Vector *vec, int i);
void vec_free(Vector *vec);

typedef enum {
    TK_RESERVED,
    TK_IDENT,
    TK_NUM,
    TK_EOF,
    TK_RETURN,
    TK_IF,
    TK_ELSE,
    TK_WHILE,
    TK_FOR,
    TK_LBRANCE,
    TK_RBRANCE,
    TK_INC,
    TK_DEC,
} TokenKind;

typedef enum {
    ND_ADD,
    ND_SUB,
    ND_MUL,
    ND_DIV,
    ND_NUM,
    ND_EQ,
    ND_NE,
    ND_LT,
    ND_LE,
    ND_ASSIGN,
    ND_LVAR,
    ND_RETURN,
    ND_IF,
    ND_ELSE,
    ND_WHILE,
    ND_FOR,
    ND_BLOCK,
    ND_INC,
    ND_DEC,
    ND_CALL,
} NodeKind;

typedef struct Node Node;

struct Node {
    NodeKind kind;
    Node *lhs;
    Node *rhs;
    int val;
    int offset;
    Node *cond;
    Node *then;
    Node *els;
    Node *init;
    Node *inc;
    Node *dec;
    Node *body;
    Node *next;
    Vector *stmts;
    char *funcname;
    Vector *args;
};

typedef struct Token Token;

struct Token {
    TokenKind kind;
    Token *next;
    int val;
    char *str;
    int len;
};

extern Token *token;

extern char *user_input;

extern Node *code[100];

Node *stmt();
Node *assign();
Node *expr();
Node *equality();
Node *relational();
Node *add();
Node *mul();
Node *unary();
Node *primary();
bool consume(char *op);
void expect(char *op);
int expect_number();
Token *tokenize(char *p);
void gen(Node *node);
void error(char *fmt, ...);
void error_at(char *loc, char *fmt, ...);
void program();
bool at_eof();
Token *consume_ident();
char *my_strndup(const char *s, size_t n);
bool consume_token(TokenKind kind);
bool peek(char *op);
extern int foo();
extern int hello();
extern int test(int a, int b, int c, int d, int e, int f, int g, int h, int i, int j);

