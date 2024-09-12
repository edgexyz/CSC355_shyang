%{
#include <iostream>
#include <stdio.h>
#include <string.h>

/* Token types */
typedef enum {
    T_KEYWORD,
    T_IDENTIFIER,
    T_NUMBER,
    T_OPERATOR,
    T_DELIMITER,
    T_CHAR_LIT,
    T_STRING_LIT,
    UNKNOWN
} TokenType;

/* Token structure */
typedef struct {
    TokenType type;
    char value[256];
} Token;

/* Function to print tokens */
void printToken(Token token) {
    const char *typeString;
    switch (token.type) {
        case T_KEYWORD: typeString = "T_KEYWORD"; break;
        case T_IDENTIFIER: typeString = "T_IDENTIFIER"; break;
        case T_NUMBER: typeString = "T_NUMBER"; break;
        case T_OPERATOR: typeString = "T_OPERATOR"; break;
        case T_DELIMITER: typeString = "T_DELIMITER"; break;
        case T_CHAR_LIT: typeString = "T_CHAR_LIT"; break;
        case T_STRING_LIT: typeString = "T_STRING_LIT"; break;
        default: typeString = "UNKNOWN"; break;
    }
    printf("<%s, '%s'>\n", typeString, token.value);
}

void getToken(TokenType t_type) {
    Token token;
    token.type = t_type;
    strncpy(token.value, yytext, sizeof(token.value) - 1);
    // The null terminator ('\0') is used in C to mark the end of a string.
    token.value[sizeof(token.value) - 1] = '\0';
    printToken(token);
}

void error() {
    char value[256];
    strncpy(value, yytext, sizeof(value) - 1);
    std::cout << "Input: " << value << " is " << "UNKNOWN" << std::endl;
}
%}

/* Definitions */
KEYWORDS    "int"|"float"|"char"|"string"|"if"|"else"|"while"|"return"|"printf" 
DIGIT       [0-9]
LETTER 
ID      
NUMBER 

/* Rules */
%%
{KEYWORDS} getToken(T_KEYWORD);
%%

int main(int argc, char **argv) {
    if (argc > 1) {
        FILE *file = fopen(argv[1], "r");
        if (!file) {
            perror("Failed to open file");
            return 1;
        }
        yyin = file;
    }
    yylex();
    return 0;
}

int yywrap() {
    return 1;
}

