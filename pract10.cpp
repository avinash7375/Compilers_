#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 100 // Maximum number of productions and input length

typedef struct {
    char lhs;            // Left-hand side of the production
    char rhs[MAX];      // Right-hand side of the production
} Production;

typedef struct {
    char action[MAX][MAX]; // Action table
    int gotoTable[MAX][MAX]; // Goto table
} ParsingTable;

// Grammar definitions
Production productions[MAX];
int productionCount = 0;

// Function to initialize parsing table
void initializeParsingTable(ParsingTable *table) {
    for (int i = 0; i < MAX; i++) {
        for (int j = 0; j < MAX; j++) {
            table->action[i][j] = ' '; // Initialize with spaces
            table->gotoTable[i][j] = -1; // Initialize with -1
        }
    }
}

// Function to define the LR parsing table (hardcoded for this example)
void defineParsingTable(ParsingTable *table) {
    // Hardcoded parsing table for a simple grammar
    // This example assumes a specific grammar
    // S -> A
    // A -> aA | b

    // Action table
    table->action[0][0] = 's'; // Shift for 'a'
    table->action[0][1] = 's'; // Shift for 'b'
    table->action[1][2] = 'r'; // Reduce A -> aA (or A -> b)
    table->action[2][2] = 'r'; // Reduce A -> aA (or A -> b)

    // Goto table
    table->gotoTable[0][3] = 1; // Go to state 1 on 'A'
}

// Function to perform LR parsing
void parseInput(const char *input, ParsingTable *table) {
    int stateStack[MAX]; // Stack to hold states
    char symbolStack[MAX]; // Stack to hold symbols
    int topState = -1;
    int topSymbol = -1;

    int i = 0; // Input index
    while (1) {
        char currentSymbol = input[i];
        int state = (topState == -1) ? 0 : stateStack[topState];

        // Check the action table
        char action = table->action[state][currentSymbol - 'a']; // Assuming 'a' = 0, 'b' = 1

        if (action == 's') {
            // Shift
            stateStack[++topState] = state; // Push current state
            symbolStack[++topSymbol] = currentSymbol; // Push current symbol
            i++; // Move to the next symbol
        } else if (action == 'r') {
            // Reduce (example: A -> aA)
            // Pop symbols from the stack
            if (symbolStack[topSymbol] == 'a' || symbolStack[topSymbol] == 'b') {
                topSymbol--;
                // Go to state based on GOTO table
                int nextState = table->gotoTable[state][3]; // Assuming A = 3
                stateStack[topState] = nextState; // Update the top state
            } else {
                printf("Error: Unable to reduce.\n");
                return;
            }
        } else {
            printf("Error: Invalid action.\n");
            return;
        }

        // Check for acceptance
        if (topState == 0 && i >= strlen(input)) {
            printf("Input string is accepted.\n");
            return;
        }
    }
}

int main() {
    ParsingTable table;
    initializeParsingTable(&table);
    defineParsingTable(&table);

    char input[MAX_INPUT];
    printf("Enter the input string (e.g., ab): ");
    scanf("%s", input);

    parseInput(input, &table);

    return 0;
}
