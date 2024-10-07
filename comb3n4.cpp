#include <stdio.h>
#include <ctype.h>
#include <string.h>

// Define token types
typedef enum {
    IDENTIFIER,
    NUMBER,
    OPERATOR,
    PUNCTUATION,
    UNKNOWN
} TokenType;

// Define the structure of a token
typedef struct {
    TokenType type;
    char value[100];
} Token;

// Function to tokenize the input string
int tokenize(const char *input, Token tokens[], int maxTokens) {
    int i = 0, tokenIndex = 0;

    while (input[i] != '\0' && tokenIndex < maxTokens) {
        if (isalpha(input[i])) {
            // Process identifier
            int j = 0;
            while (isalnum(input[i]) || input[i] == '_') {
                tokens[tokenIndex].value[j++] = input[i++];
            }
            tokens[tokenIndex].value[j] = '\0';
            tokens[tokenIndex].type = IDENTIFIER;
            tokenIndex++;
        } else if (isdigit(input[i])) {
            // Process number
            int j = 0;
            while (isdigit(input[i])) {
                tokens[tokenIndex].value[j++] = input[i++];
            }
            tokens[tokenIndex].value[j] = '\0';
            tokens[tokenIndex].type = NUMBER;
            tokenIndex++;
        } else if (ispunct(input[i])) {
            // Process operator or punctuation
            tokens[tokenIndex].value[0] = input[i];
            tokens[tokenIndex].value[1] = '\0';
            if (input[i] == '=' || input[i] == '+' || input[i] == '-' ||
                input[i] == '*' || input[i] == '/' || input[i] == ';') {
                tokens[tokenIndex].type = OPERATOR;
            } else {
                tokens[tokenIndex].type = PUNCTUATION;
            }
            tokenIndex++;
            i++;
        } else {
            // Process unknown token
            tokens[tokenIndex].value[0] = input[i];
            tokens[tokenIndex].value[1] = '\0';
            tokens[tokenIndex].type = UNKNOWN;
            tokenIndex++;
            i++;
        }
    }

    return tokenIndex; // Return the total number of tokens generated
}

// Function to print tokens
void printTokens(Token tokens[], int tokenCount) {
    for (int i = 0; i < tokenCount; i++) {
        printf("Token Type: ");
        switch (tokens[i].type) {
            case IDENTIFIER: printf("IDENTIFIER"); break;
            case NUMBER: printf("NUMBER"); break;
            case OPERATOR: printf("OPERATOR"); break;
            case PUNCTUATION: printf("PUNCTUATION"); break;
            case UNKNOWN: printf("UNKNOWN"); break;
        }
        printf(", Value: %s\n", tokens[i].value);
    }
}

// Parser function to check if the token sequence matches "IDENTIFIER = NUMBER ;"
int parse(Token tokens[], int tokenCount) {
    if (tokenCount != 4) {
        return 0; // Expecting exactly 4 tokens
    }
    // First token should be IDENTIFIER
    if (tokens[0].type != IDENTIFIER) {
        return 0;
    }
    // Second token should be "=" operator
    if (tokens[1].type != OPERATOR || strcmp(tokens[1].value, "=") != 0) {
        return 0;
    }
    // Third token should be NUMBER
    if (tokens[2].type != NUMBER) {
        return 0;
    }
    // Fourth token should be ";" operator
    if (tokens[3].type != OPERATOR || strcmp(tokens[3].value, ";") != 0) {
        return 0;
    }
    return 1; // Valid sequence
}

int main() {
    char input[100];
    Token tokens[100];  // Array to store tokens
    int tokenCount;

    // Read input from the user
    printf("Enter a string to tokenize and parse: ");
    fgets(input, sizeof(input), stdin);

    // Remove the newline character at the end (if any)
    input[strcspn(input, "\n")] = '\0';

    // Tokenize the input string
    tokenCount = tokenize(input, tokens, 100);

    // Print the resulting tokens
    printTokens(tokens, tokenCount);

    // Parse the tokens to check if the string is valid according to the grammar
    if (parse(tokens, tokenCount)) {
        printf("The string is valid (parses successfully).\n");
    } else {
        printf("The string is invalid (fails to parse).\n");
    }

    return 0;
}
