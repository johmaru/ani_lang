#include "ani_lang.h"
#include "error_message.h"


typedef struct LVar LVar;

struct LVar {
    LVar *next;
    char *name;
    int len;
    int offset;
};

LVar *locals;

LVar *find_lvar(Token *tok) {
  for (LVar *var = locals; var; var = var->next)
    if (var->len == tok->len && !memcmp(tok->str, var->name, var->len))
      return var;
  return NULL;
}



Node *new_node(NodeKind kind,Node *lhs, Node *rhs) {
    Node *node = calloc(1, sizeof(Node));
    node->kind = kind;
    node->lhs = lhs;
    node->rhs = rhs;
    return node;
}

Node *new_node_if(Node *cond, Node *then, Node *els) {
    Node *node = calloc(1, sizeof(Node));
    node->kind = ND_IF;
    node->cond = cond;
    node->then = then;
    node->els = els;
    return node;
}

Node *new_node_num(int val) {
    Node *node = calloc(1, sizeof(Node));
    node->kind = ND_NUM;
    node->val = val;
    return node;
}

Node *stmt() {
    Node *node;
    if (consume_token(TK_IF)) {
        expect("(");
        Node *cond = expr();
        expect(")");

        Node *then = stmt();

        Node *els = consume_token(TK_ELSE) ? stmt() : NULL;
        return new_node_if(cond, then, els);
    }

     if (consume_token(TK_RETURN)) {
       node = calloc(1, sizeof(Node));
         node->kind = ND_RETURN;
            node->lhs = expr();
    } else {
        node = expr();
    }

    if (!consume(";")) {
        error_message->state = NotSemiColonError;
        error_at(token->str, "%s", error_message_to_string(error_message));
    }
    return node;
    }

void program() {
    locals = calloc(1, sizeof(LVar));
    locals->offset = 0;
    int i = 0;
    while (!at_eof()) {
        code[i++] = stmt();
    }
    code[i] = NULL;
}    

Node *expr() {
    return assign();
}

Node *code[100];

Node *assign() {
    Node *node = equality();
    if (consume("=")) {
        node = new_node(ND_ASSIGN, node, assign());
    }
    return node;
    }

Node *equality() {
    Node *node = relational();

    for (;;) {
        if (consume("==")) {
            node = new_node(ND_EQ, node, relational());
        } else if (consume("!=")) {
            node = new_node(ND_NE, node, relational());
        } else {
            return node;
        }
    }
}

Node *relational() {
    Node *node = add();

    for (;;) {
        if (consume("<")) {
            node = new_node(ND_LT, node, add());
        } else if (consume("<=")) {
            node = new_node(ND_LE, node, add());
        } else if (consume(">")) {
            node = new_node(ND_LT, add(), node);
        } else if (consume(">=")) {
            node = new_node(ND_LE, add(), node);
        } else {
            return node;
        }
    }
}

Node *add() {
    Node *node = mul();

    for (;;) {
        if (consume("+")) {
            node = new_node(ND_ADD, node, mul());
        } else if (consume("-")) {
            node = new_node(ND_SUB, node, mul());
        } else {
            return node;
        }
    }
}

Node *mul() {
    Node *node = unary();

    for (;;) {
        if (consume("*")) {
            node = new_node(ND_MUL, node, unary());
        } else if (consume("/")) {
            node = new_node(ND_DIV, node, unary());
        } else {
            return node;
        }
    }
}

Node *unary() {
    if (consume("+")) 
        return primary();
    if (consume("-"))
        return new_node(ND_SUB, new_node_num(0), primary());
    return primary();
}

Node *primary() {

    if (consume("(")) {
        Node *node = expr();
        expect(")");
        return node;
    }

    Token *tok = consume_ident();
    if (tok) {
        Node *node = calloc(1, sizeof(Node));
        node->kind = ND_LVAR;

        LVar *lvar = find_lvar(tok);
        if (lvar) {
            node->offset = lvar->offset;
        } else {
            lvar = calloc(1, sizeof(LVar));
            lvar->next = locals;
            lvar->name = tok->str;
            lvar->len = tok->len;
            lvar->offset = locals->offset + 8;
            node->offset = lvar->offset;
            locals = lvar;
        }
        return node;
    }

    return new_node_num(expect_number());
}

Token *consume_ident() {
    if (token->kind == TK_IDENT) {
        Token *tok = token;
        token = token->next;
        return tok;
    }
    return NULL;
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
    va_end(ap);
    exit(1);
}

void error(char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    fprintf(stderr, "\n");
    exit(1);
}

bool consume_token(TokenKind kind) {
    if (token->kind != kind)
        return false;
    token = token->next;
    return true;
}

bool consume(char *op) {
    if (token->kind != TK_RESERVED || 
        strlen(op) != token->len ||
        memcmp(token->str, op, token->len)) {
        return false;
    }
    token = token->next;
    return true;
}

void expect(char *op) {
    if (token->kind != TK_RESERVED || 
        strlen(op) != token->len || 
        memcmp(token->str, op, token->len)) {
        error_message->state = NotMachError;    
        error_at(token->str, "'%s'%s", op, error_message_to_string(error_message));
    }
    token = token->next;
}

int expect_number() {
    if (token->kind != TK_NUM) {
        error_message->state = NotNumError;
        error_at(token->str, "%s", error_message_to_string(error_message));
    }
    int val = token->val;
    token = token->next;
    return val;
}

bool at_eof() {
    return token->kind == TK_EOF;
}

Token *new_token(TokenKind kind, Token *cur, char *str, int len) {
    Token *tok = calloc(1, sizeof(Token));
    tok->kind = kind;
    tok->str = str;
    tok->len = len;
    cur->next = tok;
    fprintf(stderr, "新しいトークン: ");
    fprintf(stderr, "種類=%d, ", kind);
    fprintf(stderr, "文字列=\"%.*s\", ", len, str);
    fprintf(stderr, "長さ=%d\n", len);
    return tok;
}

bool startswith(char *p, char *q) {
    return memcmp(p, q, strlen(q)) == 0;
}

bool is_keyword(char *p,const char *keyword) {
    int byte_len = strlen(keyword);
    if(strncmp(p, keyword, byte_len) == 0) {
        unsigned char next = p[byte_len];
        return !(isalnum(next) || next == 0x80);
    }
    return false;
}

Token *tokenize(char *p) {
    Token head;
    head.next = NULL;
    Token *cur = &head;

    while (*p)
    {
       if(isspace(*p)) {
           p++;
           continue;
       }

       if (isdigit(*p)) {
            char *q = p;
            int val = strtol(p, &p, 10);
            cur = new_token(TK_NUM, cur, q, p - q);
            cur->val = val;
            continue;
        }


        if (is_keyword(p, "もし")) {
            int len = strlen("もし");
            cur = new_token(TK_IF, cur, p, len);
            p += len;
            continue;
        }    

         if (startswith(p, "==") || startswith(p, "!=") || startswith(p, "<=") || startswith(p, ">=")) {
            cur = new_token(TK_RESERVED, cur, p, 2);
            p += 2;
            continue;
         }

         if (is_keyword(p, "かえす")) {
            int len = strlen("かえす");
            cur = new_token(TK_RETURN, cur, p, len);
            p += len;
            continue;
         }

         if ('a' <= *p && *p <= 'z') {
             cur = new_token(TK_IDENT, cur, p++, 1);
             continue;
            }

            if (strchr("=+-*/()<>;", *p)) {
            cur = new_token(TK_RESERVED, cur, p++, 1);
            continue;
            }
            error_message->state = FailTokenizeError;
            error("%s", error_message_to_string(error_message));
    }

    new_token(TK_EOF, cur, p,0);
    return head.next;
    
}