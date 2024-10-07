#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define structure for a state
typedef struct State {
    int state_num;
    struct State* on_a; // Transition for 'a'
    struct State* on_b; // Transition for 'b'
    struct State* epsilon1; // Epsilon transition 1
    struct State* epsilon2; // Epsilon transition 2
} State;

// Define structure for an NFA
typedef struct NFA {
    State* start_state;
    State* accept_state;
} NFA;

int state_counter = 0; // Global state counter

// Function to create a new state
State* createState() {
    State* state = (State*)malloc(sizeof(State));
    state->state_num = state_counter++;
    state->on_a = NULL;
    state->on_b = NULL;
    state->epsilon1 = NULL;
    state->epsilon2 = NULL;
    return state;
}

// Function to create an NFA for a single character
NFA* createCharNFA(char c) {
    NFA* nfa = (NFA*)malloc(sizeof(NFA));
    State* start = createState();
    State* accept = createState();
    
    if (c == 'a') {
        start->on_a = accept; // Transition on 'a'
    } else if (c == 'b') {
        start->on_b = accept; // Transition on 'b'
    }
    
    nfa->start_state = start;
    nfa->accept_state = accept;
    return nfa;
}

// Function to combine two NFAs using concatenation
NFA* concatenateNFA(NFA* nfa1, NFA* nfa2) {
    NFA* nfa = (NFA*)malloc(sizeof(NFA));
    nfa1->accept_state->epsilon1 = nfa2->start_state; // Epsilon transition from nfa1's accept to nfa2's start
    nfa->start_state = nfa1->start_state;
    nfa->accept_state = nfa2->accept_state;
    return nfa;
}

// Function to combine two NFAs using union (a | b)
NFA* unionNFA(NFA* nfa1, NFA* nfa2) {
    NFA* nfa = (NFA*)malloc(sizeof(NFA));
    State* start = createState(); // New start state
    State* accept = createState(); // New accept state
    
    // Epsilon transitions from new start state to the start states of nfa1 and nfa2
    start->epsilon1 = nfa1->start_state;
    start->epsilon2 = nfa2->start_state;
    
    // Epsilon transitions from the accept states of nfa1 and nfa2 to the new accept state
    nfa1->accept_state->epsilon1 = accept;
    nfa2->accept_state->epsilon1 = accept;
    
    nfa->start_state = start;
    nfa->accept_state = accept;
    return nfa;
}

// Function to apply Kleene star to an NFA (a*)
NFA* kleeneStarNFA(NFA* nfa) {
    NFA* new_nfa = (NFA*)malloc(sizeof(NFA));
    State* start = createState(); // New start state
    State* accept = createState(); // New accept state
    
    // Epsilon transitions
    start->epsilon1 = nfa->start_state; // From new start to old start
    start->epsilon2 = accept;           // From new start to new accept (for zero repetitions)
    nfa->accept_state->epsilon1 = nfa->start_state; // From old accept to old start (loop)
    nfa->accept_state->epsilon2 = accept;           // From old accept to new accept
    
    new_nfa->start_state = start;
    new_nfa->accept_state = accept;
    return new_nfa;
}

// Function to print the NFA transitions
void printNFA(State* state, int visited[]) {
    if (visited[state->state_num]) return;
    visited[state->state_num] = 1;
    
    if (state->on_a) {
        printf("State %d --a--> State %d\n", state->state_num, state->on_a->state_num);
        printNFA(state->on_a, visited);
    }
    
    if (state->on_b) {
        printf("State %d --b--> State %d\n", state->state_num, state->on_b->state_num);
        printNFA(state->on_b, visited);
    }
    
    if (state->epsilon1) {
        printf("State %d --ε--> State %d\n", state->state_num, state->epsilon1->state_num);
        printNFA(state->epsilon1, visited);
    }
    
    if (state->epsilon2) {
        printf("State %d --ε--> State %d\n", state->state_num, state->epsilon2->state_num);
        printNFA(state->epsilon2, visited);
    }
}

// Function to construct NFA from regular expression
NFA* constructNFA(char* regex) {
    NFA* stack[100]; // Stack for NFAs
    int top = -1;
    
    for (int i = 0; i < strlen(regex); i++) {
        if (regex[i] == 'a' || regex[i] == 'b') {
            // Create NFA for character
            stack[++top] = createCharNFA(regex[i]);
        } else if (regex[i] == '.') {
            // Concatenation
            NFA* nfa2 = stack[top--];
            NFA* nfa1 = stack[top--];
            stack[++top] = concatenateNFA(nfa1, nfa2);
        } else if (regex[i] == '|') {
            // Union
            NFA* nfa2 = stack[top--];
            NFA* nfa1 = stack[top--];
            stack[++top] = unionNFA(nfa1, nfa2);
        } else if (regex[i] == '*') {
            // Kleene star
            NFA* nfa = stack[top--];
            stack[++top] = kleeneStarNFA(nfa);
        }
    }
    
    return stack[top]; // Final NFA
}

int main() {
    char regex[100];
    
    // Input regular expression in postfix notation
    printf("Enter the regular expression in postfix notation (e.g., ab.a.): ");
    scanf("%s", regex);
    
    // Construct the NFA
    NFA* nfa = constructNFA(regex);
    
    // Print the NFA
    int visited[100] = {0};
    printf("NFA transitions:\n");
    printNFA(nfa->start_state, visited);
    
    return 0;
}
