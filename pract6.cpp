#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STATES 100
#define MAX_TRANSITIONS 128

// Define structure for NFA state
typedef struct {
    int transitions[MAX_TRANSITIONS][MAX_STATES]; // State transitions
    int epsilon[MAX_STATES]; // Epsilon transitions
    int epsilon_count;
    int transition_count[MAX_TRANSITIONS];
} NFAState;

// Define structure for DFA state
typedef struct {
    int nfa_states[MAX_STATES]; // Set of NFA states represented by this DFA state
    int nfa_state_count;
} DFAState;

int state_counter = 0;
NFAState nfa[MAX_STATES];
int dfa[MAX_STATES][MAX_TRANSITIONS];
int visited[MAX_STATES];

// Function to initialize an NFA state
void initNFAState(NFAState *state) {
    for (int i = 0; i < MAX_TRANSITIONS; i++) {
        state->transition_count[i] = 0;
    }
    state->epsilon_count = 0;
}

// Function to add a transition to an NFA state
void addTransition(NFAState *state, int input, int to_state) {
    state->transitions[input][state->transition_count[input]++] = to_state;
}

// Function to add an epsilon transition to an NFA state
void addEpsilonTransition(NFAState *state, int to_state) {
    state->epsilon[state->epsilon_count++] = to_state;
}

// Function to perform epsilon closure on a set of states
void epsilonClosure(int nfa_states[], int *nfa_state_count) {
    int stack[MAX_STATES];
    int top = -1;
    int visited[MAX_STATES] = {0};
    
    // Push all initial states into the stack
    for (int i = 0; i < *nfa_state_count; i++) {
        stack[++top] = nfa_states[i];
    }
    
    while (top != -1) {
        int current = stack[top--];
        visited[current] = 1;
        
        // Add epsilon transitions
        for (int i = 0; i < nfa[current].epsilon_count; i++) {
            int next = nfa[current].epsilon[i];
            if (!visited[next]) {
                stack[++top] = next;
                nfa_states[(*nfa_state_count)++] = next;
            }
        }
    }
}

// Check if two DFA states (sets of NFA states) are equivalent
int areSameStates(DFAState *s1, DFAState *s2) {
    if (s1->nfa_state_count != s2->nfa_state_count)
        return 0;
    for (int i = 0; i < s1->nfa_state_count; i++) {
        if (s1->nfa_states[i] != s2->nfa_states[i])
            return 0;
    }
    return 1;
}

// Function to check if a DFA state already exists
int stateExists(DFAState dfa_states[], int count, DFAState *state) {
    for (int i = 0; i < count; i++) {
        if (areSameStates(&dfa_states[i], state))
            return i;
    }
    return -1;
}

// Function to convert NFA to DFA using subset construction
void convertNFAToDFA(int start_state, int num_inputs) {
    DFAState dfa_states[MAX_STATES]; // DFA states
    int dfa_state_count = 0;

    // DFA transition table
    memset(dfa, -1, sizeof(dfa));

    // Initially, epsilon closure of the NFA start state is the DFA start state
    DFAState dfa_queue[MAX_STATES];
    int queue_front = 0, queue_back = 0;

    // Create the start state for DFA
    dfa_queue[queue_back].nfa_states[0] = start_state;
    dfa_queue[queue_back].nfa_state_count = 1;
    epsilonClosure(dfa_queue[queue_back].nfa_states, &dfa_queue[queue_back].nfa_state_count);
    dfa_states[dfa_state_count++] = dfa_queue[queue_back]; // DFA start state corresponds to this set of NFA start states
    queue_back++;

    // Process each DFA state
    while (queue_front < queue_back) {
        DFAState current_dfa_state = dfa_queue[queue_front++]; // Get the next DFA state to process

        // Process transitions for each input symbol
        for (int input = 0; input < num_inputs; input++) {
            DFAState new_dfa_state;
            new_dfa_state.nfa_state_count = 0;

            // Compute the set of NFA states reachable on this input
            for (int i = 0; i < current_dfa_state.nfa_state_count; i++) {
                int nfa_state = current_dfa_state.nfa_states[i];
                for (int j = 0; j < nfa[nfa_state].transition_count[input]; j++) {
                    new_dfa_state.nfa_states[new_dfa_state.nfa_state_count++] = nfa[nfa_state].transitions[input][j];
                }
            }

            // Compute epsilon closure of the new DFA state
            epsilonClosure(new_dfa_state.nfa_states, &new_dfa_state.nfa_state_count);

            // Check if the new DFA state already exists
            int existing_state = stateExists(dfa_states, dfa_state_count, &new_dfa_state);
            if (existing_state == -1) {
                // New DFA state, add it
                dfa_states[dfa_state_count++] = new_dfa_state;
                dfa_queue[queue_back++] = new_dfa_state;
                existing_state = dfa_state_count - 1;
            }

            // Update the DFA transition table
            dfa[queue_front - 1][input] = existing_state;
        }
    }

    // Print the DFA
    printf("\nDFA State Transitions:\n");
    for (int i = 0; i < dfa_state_count; i++) {
        printf("DFA State %d:\n", i);
        for (int input = 0; input < num_inputs; input++) {
            if (dfa[i][input] != -1) {
                printf("  On input %d -> DFA State %d\n", input, dfa[i][input]);
            }
        }
    }
}

int main() {
    char regex[100];
    int num_inputs = 2; // Assuming binary input (0 or 1 for simplicity)

    // Define NFA (in this case, manually constructing one for demo)
    // In practice, you'd first convert a regex to NFA
    initNFAState(&nfa[0]);
    addTransition(&nfa[0], 0, 1);
    addEpsilonTransition(&nfa[0], 2);

    initNFAState(&nfa[1]);
    addTransition(&nfa[1], 1, 3);

    initNFAState(&nfa[2]);
    addTransition(&nfa[2], 1, 3);

    initNFAState(&nfa[3]); // Accept state

    // Convert the NFA to DFA
    convertNFAToDFA(0, num_inputs);

    return 0;
}
