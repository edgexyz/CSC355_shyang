#include <iostream>
#include <string>
#include <cassert>
using namespace std;

// DFA States
enum State {
    A, // Start state
    B,
    C,
    SINK
};

/*  Simulates DFA state transition on the input symbol.
 *  
 *  @param current state
 *  @param input symbol
 *  @return transited state or error.
 */
State transition(State currentState, char symbol) {
    switch (currentState) {
        case A:
            if (symbol == 'a') return B;
            else if (symbol == 'b') return C;
            else return SINK;
        case B:
            if (symbol == 'b') return C;
            else if (symbol == 'a') return SINK;
        case C:
            if (symbol == 'b') return C;
            else if (symbol == 'a') return SINK;
        case SINK:
            if (symbol == 'b') return SINK;
            else if (symbol == 'a') return SINK;
        default:
            return SINK;
    }
}

/*  Checks if the current state is an acceptign state.
 *  
 *  @param current state
 *  @return true if the current state is an accepting state. false, otherwise.
 */
bool isAcceptingState(State currentState) {
    return currentState == A || currentState == B || currentState == C;
}

/*  Convert enum State type to string.
 *  
 *  @param current state
 *  @return state in string
 */
string tostring(State currentState) {

    string state;

    if (currentState == A) {
        state = "A";
    }
    else if (currentState == B) {
        state = "B";
    }
    else if (currentState == C) {
        state = "C";
    }
    else if (currentState == SINK) {
        state = "SINK";
    }
    else {
        assert(false);
    }

    return state;
}

int main() {
    string input;
    cout << "Enter a string (consisting of 'a' and 'b'): ";
    cin >> input;

    State currentState = A; // Start at A

    string path = "A";

    // Process each symbol in the input string
    for (int i = 0; i < input.length(); i++) {
        char symbol = input[i];
        currentState = transition(currentState, symbol);
        
        string stateStr = tostring(currentState);
        path = path + "->" + stateStr;
    }
    
    if (isAcceptingState(currentState)) {
        cout << "Input accepted!" << endl;
    }
    else {
        cout << "Input rejected!" << endl;
    }
    cout << "Path: " << path << endl;

    return 0;
}
