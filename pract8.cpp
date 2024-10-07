#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 100
#define MAX_PRODUCTIONS 50
#define MAX_SYMBOLS 50

// Structure to hold grammar productions
typedef struct {
    char lhs;
    char rhs[MAX_PRODUCTIONS][MAX]; // Right-hand side productions
    int rhs_count;
} Production;

Production productions[MAX_PRODUCTIONS];
char first[MAX_SYMBOLS][MAX];  // First sets
char follow[MAX_SYMBOLS][MAX];  // Follow sets
int firstCount[MAX_SYMBOLS];     // Counts of First sets
int followCount[MAX_SYMBOLS];    // Counts of Follow sets
int productionCount = 0;

// Function to find First set of a given non-terminal
void findFirst(char symbol) {
    // If the symbol is terminal
    if (!(symbol >= 'A' && symbol <= 'Z')) {
        first[symbol][firstCount[symbol]++] = symbol; // Add terminal to First set
        return;
    }

    for (int i = 0; i < productionCount; i++) {
        if (productions[i].lhs == symbol) {
            for (int j = 0; j < productions[i].rhs_count; j++) {
                char *production = productions[i].rhs[j];
                if (production[0] == 'ε') { // Epsilon production
                    first[symbol][firstCount[symbol]++] = 'ε';
                } else {
                    for (int k = 0; production[k] != '\0'; k++) {
                        // If the current symbol is non-terminal
                        if (production[k] >= 'A' && production[k] <= 'Z') {
                            findFirst(production[k]);
                            // Add First set of the non-terminal to the First set
                            for (int l = 0; l < firstCount[production[k]]; l++) {
                                if (first[production[k]][l] != 'ε') {
                                    first[symbol][firstCount[symbol]++] = first[production[k]][l];
                                }
                            }
                            // If the First set contains epsilon, continue to next symbol
                            if (strchr(first[production[k]], 'ε') == NULL) {
                                break;
                            }
                        } else { // If it's a terminal
                            first[symbol][firstCount[symbol]++] = production[k];
                            break;
                        }
                    }
                }
            }
        }
    }
}

// Function to find Follow set of a given non-terminal
void findFollow(char symbol) {
    // If it's the start symbol, add '$' to Follow set
    if (productions[0].lhs == symbol) {
        follow[symbol][followCount[symbol]++] = '$';
    }

    for (int i = 0; i < productionCount; i++) {
        for (int j = 0; j < productions[i].rhs_count; j++) {
            char *production = productions[i].rhs[j];

            for (int k = 0; production[k] != '\0'; k++) {
                // If the current symbol is found in the production
                if (production[k] == symbol) {
                    // Look at the next symbol
                    if (production[k + 1] != '\0') {
                        // If next symbol is a terminal
                        if (!(production[k + 1] >= 'A' && production[k + 1] <= 'Z')) {
                            follow[symbol][followCount[symbol]++] = production[k + 1];
                        } else { // If next symbol is a non-terminal
                            for (int l = 0; l < firstCount[production[k + 1]]; l++) {
                                if (first[production[k + 1]][l] != 'ε') {
                                    follow[symbol][followCount[symbol]++] = first[production[k + 1]][l];
                                }
                            }
                            // If First of next symbol contains epsilon, add Follow of current symbol
                            if (strchr(first[production[k + 1]], 'ε') != NULL) {
                                findFollow(production[k + 1]);
                                for (int m = 0; m < followCount[production[k + 1]]; m++) {
                                    follow[symbol][followCount[symbol]++] = follow[production[k + 1]][m];
                                }
                            }
                        }
                    } else { // If it's the last symbol, add Follow of current production's lhs
                        if (productions[i].lhs != symbol) {
                            findFollow(productions[i].lhs);
                            for (int m = 0; m < followCount[productions[i].lhs]; m++) {
                                follow[symbol][followCount[symbol]++] = follow[productions[i].lhs][m];
                            }
                        }
                    }
                }
            }
        }
    }
}

// Function to display First and Follow sets
void display() {
    printf("\nFirst Sets:\n");
    for (int i = 0; i < MAX_SYMBOLS; i++) {
        if (firstCount[i] > 0) {
            printf("First(%c) = { ", i);
            for (int j = 0; j < firstCount[i]; j++) {
                printf("%c ", first[i][j]);
            }
            printf("}\n");
        }
    }

    printf("\nFollow Sets:\n");
    for (int i = 0; i < MAX_SYMBOLS; i++) {
        if (followCount[i] > 0) {
            printf("Follow(%c) = { ", i);
            for (int j = 0; j < followCount[i]; j++) {
                printf("%c ", follow[i][j]);
            }
            printf("}\n");
        }
    }
}

int main() {
    char startSymbol;

    // Input the grammar
    printf("Enter the number of productions: ");
    scanf("%d", &productionCount);
    
    for (int i = 0; i < productionCount; i++) {
        printf("Enter production (e.g., A->aB): ");
        char input[MAX];
        scanf("%s", input);
        
        productions[i].lhs = input[0];
        productions[i].rhs_count = 0;
        
        // Split the RHS productions by '|'
        char *token = strtok(input + 3, "|");
        while (token != NULL) {
            strcpy(productions[i].rhs[productions[i].rhs_count], token);
            productions[i].rhs_count++;
            token = strtok(NULL, "|");
        }
    }

    // Compute First sets
    for (int i = 0; i < productionCount; i++) {
        findFirst(productions[i].lhs);
    }

    // Compute Follow sets
    for (int i = 0; i < productionCount; i++) {
        findFollow(productions[i].lhs);
    }

    // Display the First and Follow sets
    display();

    return 0;
}
