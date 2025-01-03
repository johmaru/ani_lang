#ifndef ERROR_MESSAGE_H
#define ERROR_MESSAGE_H

typedef enum {
    NORMAL,
} ErrorMessageType;

typedef enum {
    NumArgumentError,
    NotSemiColonError,
    NotMachError,
    NotNumError,
    FailTokenizeError,
    NotLvarIsVariableError,
} ErrorState;

typedef struct ErrorMessage {
    ErrorMessageType type;
    ErrorState state;
} ErrorMessage;

extern ErrorMessage *error_message;
char* error_message_to_string(ErrorMessage *error_message);

#endif