#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

// Define token types
typedef enum {
    IDENTIFIER,
    NUMBER,
    OPERATOR,
    PUNCTUATION,
    UNKNOWN
} TokenType;

// Define structure of a token
typedef struct {
    TokenType type;
    char value[100];
} Token;

// Define structure for a syntax tree node
typedef struct SyntaxTreeNode {
    char value[100];
    struct SyntaxTreeNode* left;
    struct SyntaxTreeNode* right;
} SyntaxTreeNode;

// Function to create a new syntax tree node
SyntaxTreeNode* createNode(const char* value) {
    SyntaxTreeNode* node = (SyntaxTreeNode*)malloc(sizeof(SyntaxTreeNode));
    strcpy(node->value, value);
    node->left = NULL;
    node->right = NULL;
    return node;
}

// Function to print the syntax tree (preorder traversal)
void printSyntaxTree(SyntaxTreeNode* root, int level) {
    if (root == NULL) return;

    // Print current node with indentation based on the level in the tree
    for (int i = 0; i < level; i++) {
        printf("  ");
    }
    printf("%s\n", root->value);

    // Recur for left and right children
    printSyntaxTree(root->left, level + 1);
    printSyntaxTree(root->right, level + 1);
}

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

// Function to parse tokens and generate the syntax tree
SyntaxTreeNode* parseAndGenerateSyntaxTree(Token tokens[], int tokenCount) {
    if (tokenCount != 4) {
        printf("Parsing failed: Expecting exactly 4 tokens\n");
        return NULL; // Invalid number of tokens
    }
    
    // Ensure the first token is an identifier
    if (tokens[0].type != IDENTIFIER) {
        printf("Parsing failed: First token should be an identifier\n");
        return NULL;
    }
    
    // Ensure the second token is an '=' operator
    if (tokens[1].type != OPERATOR || strcmp(tokens[1].value, "=") != 0) {
        printf("Parsing failed: Second token should be '='\n");
        return NULL;
    }
    
    // Ensure the third token is a number
    if (tokens[2].type != NUMBER) {
        printf("Parsing failed: Third token should be a number\n");
        return NULL;
    }
    
    // Ensure the fourth token is a semicolon
    if (tokens[3].type != OPERATOR || strcmp(tokens[3].value, ";") != 0) {
        printf("Parsing failed: Fourth token should be ';'\n");
        return NULL;
    }
    
    // Create the syntax tree
    SyntaxTreeNode* root = createNode("=");
    root->left = createNode(tokens[0].value);  // Identifier on the left
    root->right = createNode(tokens[2].value); // Number on the right
    
    return root; // Return the root of the syntax tree
}

int main() {
    char input[100];
    Token tokens[100];  // Array to store tokens
    int tokenCount;
    SyntaxTreeNode* syntaxTree;

    // Read input from the user
    printf("Enter a string to tokenize and generate syntax tree: ");
    fgets(input, sizeof(input), stdin);

    // Remove the newline character at the end (if any)
    input[strcspn(input, "\n")] = '\0';

    // Tokenize the input string
    tokenCount = tokenize(input, tokens, 100);

    // Print the resulting tokens
    printTokens(tokens, tokenCount);

    // Parse tokens and generate the syntax tree
    syntaxTree = parseAndGenerateSyntaxTree(tokens, tokenCount);

    // If parsing was successful, print the syntax tree
    if (syntaxTree != NULL) {
        printf("Syntax tree:\n");
        printSyntaxTree(syntaxTree, 0);
    } else {
        printf("Failed to generate syntax tree due to parsing errors.\n");
    }

    return 0;
}
