#include <stdio.h>
#include <string.h>
#include <ctype.h>

// Define token types
typedef enum {
    IDENTIFIER,
    NUMBER,
    OPERATOR,
    PUNCTUATION,
    UNKNOWN
} TokenType;

// Token structure
typedef struct {
    TokenType type;
    char value[100];
} Token;

// Function to tokenize the input string
int tokenize(const char* lexeme, Token tokens[], int maxTokens) {
    int i = 0, tokenIndex = 0;
    while (lexeme[i] != '\0' && tokenIndex < maxTokens) {
        if (isalpha(lexeme[i])) {
            // Identifier
            int j = 0;
            while (isalnum(lexeme[i]) || lexeme[i] == '_') {
                tokens[tokenIndex].value[j++] = lexeme[i++];
            }
            tokens[tokenIndex].value[j] = '\0';
            tokens[tokenIndex].type = IDENTIFIER;
            tokenIndex++;
        } else if (isdigit(lexeme[i])) {
            // Number
            int j = 0;
            while (isdigit(lexeme[i])) {
                tokens[tokenIndex].value[j++] = lexeme[i++];
            }
            tokens[tokenIndex].value[j] = '\0';
            tokens[tokenIndex].type = NUMBER;
            tokenIndex++;
        } else if (ispunct(lexeme[i])) {
            // Operator or punctuation
            tokens[tokenIndex].value[0] = lexeme[i];
            tokens[tokenIndex].value[1] = '\0';
            if (lexeme[i] == '=' || lexeme[i] == ';') {
                tokens[tokenIndex].type = OPERATOR;
            } else {
                tokens[tokenIndex].type = UNKNOWN;
            }
            tokenIndex++;
            i++;
        } else {
            i++;
        }
    }
    return tokenIndex; // Return the number of tokens
}

// Function to parse the tokens based on the grammar
int parse(Token tokens[], int tokenCount) {
    if (tokenCount != 4) {
        return 0; // We expect exactly 4 tokens: IDENTIFIER, OPERATOR (=), NUMBER, OPERATOR (;)
    }
    // Check the first token: it should be an IDENTIFIER
    if (tokens[0].type != IDENTIFIER) {
        return 0;
    }
    // Check the second token: it should be an OPERATOR (equal sign '=')
    if (tokens[1].type != OPERATOR || strcmp(tokens[1].value, "=") != 0) {
        return 0;
    }
    // Check the third token: it should be a NUMBER
    if (tokens[2].type != NUMBER) {
        return 0;
    }
    // Check the fourth token: it should be an OPERATOR (semicolon ';')
    if (tokens[3].type != OPERATOR || strcmp(tokens[3].value, ";") != 0) {
        return 0;
    }
    return 1; // Parsing successful
}

int main() {
    char lexeme[100];
    Token tokens[100];
    
    printf("Enter a lexeme: ");
    fgets(lexeme, sizeof(lexeme), stdin);
    
    // Remove the newline character if present
    lexeme[strcspn(lexeme, "\n")] = '\0';

    // Tokenize the input
    int tokenCount = tokenize(lexeme, tokens, 100);
    
    // Parse the tokens and check if the string parses correctly
    if (parse(tokens, tokenCount)) {
        printf("The string is valid (parses successfully).\n");
    } else {
        printf("The string is invalid (fails to parse).\n");
    }

    return 0;
}
