#include "ani_lang.h"
#include "error_message.h"

Token *token;
char *user_input;

int main(int argc, char **argv) {
    error_message = calloc(1, sizeof(ErrorMessage));
    error_message->type = NORMAL;

    if (argc != 2) {
    error_message->state = NumArgumentError;
    fprintf(stderr, "%s\n", error_message_to_string(error_message));
    return 1;
  }

    user_input = argv[1];
    token = tokenize(user_input);
    program();

    printf(".intel_syntax noprefix\n");
    printf(".global main\n");
    printf("main:\n");

    printf("  push rbp\n");
    printf("  mov rbp, rsp\n");
    printf("  sub rsp, 208\n");

    for (int i = 0; code[i]; i++) {
        if (!code[i]) break;
        gen(code[i]);
        printf("  pop rax\n");
    }

    printf("  mov rsp, rbp\n");
    printf("  pop rbp\n");
    printf("  ret\n");
    return 0;

}