#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 10 // Maximum number of operators
#define MAX_INPUT 100 // Maximum length of input expression

// Enum for operator associativity
typedef enum { LEFT, RIGHT } Associativity;

// Structure to define an operator
typedef struct {
    char symbol;          // Operator symbol
    int precedence;      // Precedence level
    Associativity assoc; // Associativity (LEFT or RIGHT)
} Operator;

// Operator precedence table
char precedenceTable[MAX][MAX];
Operator operators[MAX];
int opCount = 0;

// Function to initialize precedence table
void initializePrecedenceTable() {
    for (int i = 0; i < MAX; i++) {
        for (int j = 0; j < MAX; j++) {
            precedenceTable[i][j] = ' '; // Initialize with spaces
        }
    }
}

// Function to add operators and their precedence
void addOperator(char symbol, int precedence, Associativity assoc) {
    operators[opCount].symbol = symbol;
    operators[opCount].precedence = precedence;
    operators[opCount].assoc = assoc;
    opCount++;
}

// Function to construct the precedence table
void constructPrecedenceTable() {
    for (int i = 0; i < opCount; i++) {
        for (int j = 0; j < opCount; j++) {
            // Check precedence and associativity
            if (operators[i].precedence > operators[j].precedence) {
                precedenceTable[i][j] = '>'; // Left operator has higher precedence
            } else if (operators[i].precedence < operators[j].precedence) {
                precedenceTable[i][j] = '<'; // Right operator has lower precedence
            } else {
                // Same precedence
                if (operators[i].assoc == LEFT) {
                    precedenceTable[i][j] = '='; // Same precedence and left associative
                } else {
                    precedenceTable[i][j] = '<'; // Right associative
                }
            }
        }
    }
}

// Function to display the precedence table
void displayPrecedenceTable() {
    printf("\nOperator Precedence Table:\n");
    printf("   ");
    for (int i = 0; i < opCount; i++) {
        printf("  %c ", operators[i].symbol);
    }
    printf("\n");

    for (int i = 0; i < opCount; i++) {
        printf(" %c ", operators[i].symbol);
        for (int j = 0; j < opCount; j++) {
            printf(" %c ", precedenceTable[i][j]);
        }
        printf("\n");
    }
}

int main() {
    initializePrecedenceTable();

    // Input operators and their precedence
    addOperator('+', 1, LEFT);
    addOperator('-', 1, LEFT);
    addOperator('*', 2, LEFT);
    addOperator('/', 2, LEFT);
    addOperator('^', 3, RIGHT); // Exponentiation operator

    // Construct precedence table
    constructPrecedenceTable();

    // Display precedence table
    displayPrecedenceTable();

    return 0;
}
