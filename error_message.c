#include "error_message.h"
#include <stddef.h>

ErrorMessage *error_message = NULL;

char* error_message_to_string(ErrorMessage *error_message) {
    switch (error_message->type) {
        case NORMAL:
            switch (error_message->state)
            {
            case NumArgumentError:
                return "引数の数が違うわ...";
                break;

            case NotSemiColonError:
                return "セミコロンが無いわ...";
                break;    

            case NotMachError:
                return "と合わないわ";
                break;   

            case NotNumError:
                return "数ではないわ";
                break;

            case FailTokenizeError:
                return "トークナイズできないわ";
                break;   

            case NotLvarIsVariableError:
                return "左の値が変数の形じゃないみたい";    
                break;          
            
            default:
               return "エラーがおきたみたい";
                break;
            }
            break;

        default:
            return "エラーがおきたみたい、でも...原因がわからない...";
            break;
    }
}