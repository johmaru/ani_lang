#include "ani_lang.h"
#include "error_message.h"

void gen_lval(Node *node) {
    if (node->kind != ND_LVAR) {
        error_message->state = NotLvarIsVariableError;
        error("%s", error_message_to_string(error_message));
    }

    printf("  mov rax, rbp\n");
    printf("  sub rax, %d\n", node->offset);
    printf("  push rax\n");
}

static int labelseq = 0;
void gen(Node *node){
fprintf(stderr, "生成中のノード: kind=%d\n", node->kind);
     if (node->kind == ND_IF) {
        gen(node->cond);
        printf("  pop rax\n");
        printf("  cmp rax, 0\n");

        int seq = labelseq++;
        printf("  je .Lelse%d\n", seq);
        gen(node->then);
        printf("  jmp .Lend%d\n", seq);
        printf(".Lelse%d:\n", seq);
        if (node->els) {
            gen(node->els);
        }
        printf(".Lend%d:\n", seq);
        return;
    }

    if (node->kind == ND_WHILE) {
        int seq = labelseq++;
        printf(".Lbegin%d:\n", seq);
        gen(node->cond);
        printf("  pop rax\n");
        printf("  cmp rax, 0\n");
        printf("  je .Lend%d\n", seq);
        gen(node->then);
        printf("  jmp .Lbegin%d\n", seq);
        printf(".Lend%d:\n", seq);
        return;
    }

    if (node->kind == ND_FOR) {
    int seq = labelseq++;
    
    if (node->init) {
        gen(node->init);
        printf("  pop rax\n");
    }
    
    printf(".Lbegin%d:\n", seq);
    
    if (node->cond) {
        gen(node->cond);
        printf("  pop rax\n");
        printf("  cmp rax, 0\n");
        printf("  je .Lend%d\n", seq);
    }
    
    if (node->body) {
        gen(node->body);
    }
    
    if (node->inc) {
        gen(node->inc);
        printf("  pop rax\n");
    }
    
    printf("  jmp .Lbegin%d\n", seq);
    printf(".Lend%d:\n", seq);
    return;
}

    if (node->kind == ND_BLOCK){
        for (int i = 0; i < node->stmts->len; i++) {
            gen(vec_get(node->stmts, i));
            if (i < node->stmts->len - 1) {
                printf("  pop rax\n"); 
            }
        }
        return;
    }

    if (node->kind == ND_RETURN) {
        gen(node->lhs);
        printf("  pop rax\n");
        printf("  mov rsp, rbp\n");
        printf("  pop rbp\n");
        printf("  ret\n");
        return;
    }

    if (node->kind == ND_INC) {
        gen_lval(node->lhs);
        printf("  pop rax\n");
        printf("  mov rdi, [rax]\n");
        printf("  add rdi, 1\n");
        printf("  mov [rax], rdi\n");
        printf("  push rdi\n");
        return;
    }


    switch (node->kind)
    {
    case ND_NUM:
        printf("  push %d\n", node->val);
        return;
    case ND_LVAR:
        gen_lval(node);
        printf("  pop rax\n");
        printf("  mov rax, [rax]\n");
        printf("  push rax\n");
        return;
     case ND_ASSIGN:
        gen_lval(node->lhs);
        gen(node->rhs);

        printf("  pop rdi\n");
        printf("  pop rax\n");
        printf("  mov [rax], rdi\n");
        printf("  push rdi\n");
        return;   
    }

    gen(node->lhs);
    gen(node->rhs);

    printf("  pop rdi\n");
    printf("  pop rax\n");

    switch (node->kind) {
        case ND_ADD:
            printf("  add rax, rdi\n");
            break;
        case ND_SUB:
            printf("  sub rax, rdi\n");
            break;
        case ND_MUL:
            printf("  imul rax, rdi\n");
            break;
        case ND_DIV:
            printf("  cqo\n");
            printf("  idiv rdi\n");
            break;
        case ND_EQ:
            printf("  cmp rax, rdi\n");
            printf("  sete al\n");
            printf("  movzb rax, al\n");
            break;
        case ND_NE:
            printf("  cmp rax, rdi\n");
            printf("  setne al\n");
            printf("  movzb rax, al\n");
            break;
        case ND_LT:
            printf("  cmp rax, rdi\n");
            printf("  setl al\n");
            printf("  movzb rax, al\n");
            break;
        case ND_LE:
            printf("  cmp rax, rdi\n");
            printf("  setle al\n");
            printf("  movzb rax, al\n");
            break;
    }

    printf("  push rax\n");
}