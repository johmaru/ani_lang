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

void gen_block(Node *node) {
    for (int i = 0; i < node->stmts->len; i++) {
        Node *stmt = vec_get(node->stmts, i);
        gen(stmt);
        if (stmt->kind != ND_RETURN && i < node->stmts->len - 1) {
            printf("  pop rax\n");
        }
    }
}

static int labelseq = 0;
void gen(Node *node){
    if (node->kind == ND_BLOCK){
        gen_block(node);
        return;
        }

     if (node->kind == ND_IF) {
        gen(node->cond);
        printf("  pop rax\n");
        printf("  cmp rax, 0\n");

        int seq = labelseq++;
        printf("  je .Lelse%d\n", seq);
        if (node->then->kind == ND_BLOCK){
            gen_block(node->then);
        } else {
            gen(node->then);
            printf("  pop rax\n");
        }
        printf("  jmp .Lend%d\n", seq);
        printf(".Lelse%d:\n", seq);
        if (node->els) {
            gen(node->els);
        }
        printf(".Lend%d:\n", seq);
        printf("  push rax\n");
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
    }
    
    printf(".Lbegin%d:\n", seq);
    
    if (node->cond) {
        gen(node->cond);
        printf("  pop rax\n");
        printf("  cmp rax, 0\n");
        printf("  je .Lend%d\n", seq);
    }
        
  if (node->body->kind == ND_BLOCK) {
        fprintf(stderr, "forブロック処理開始\n");
        for (int i = 0; i < node->body->stmts->len; i++) {
            Node *stmt = vec_get(node->body->stmts, i);
            gen(stmt);
           
            if (stmt->kind != ND_RETURN && i < node->body->stmts->len - 1) {
                printf("  pop rax\n");
            }
        }
    } else {
        gen(node->body);
        printf("  pop rax\n");
    }

   if (node->inc) {
        gen(node->inc);
        printf("  pop rax\n");  
    }
    
    printf("  jmp .Lbegin%d\n", seq);
    printf(".Lend%d:\n", seq);
    printf("  push rax\n");
    return;}

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