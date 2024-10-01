/*
 * This program simulates DFA.
 *
 * Transition Diagram:
 * -> ((A)) -a-> ((B))---
 *      |          ^    |     
 *      |          |    |
 *      b-->((C))--b    |
 *            ^         |
 *            |         |
 *            ----------b
 *
 * Author: Terrence J. Lim
 */

#include <iostream>
#include <string>
#include <cassert>
#include <stdlib.h>
using namespace std;

// DFA States
enum State {
    A,    // Start state
    B,
    C
};

string path = "A";
char curSymbol;
State prevState;

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
string toString(State currentState) {

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
    else {
        assert(false);
    }

    return state;
}

/* Prints error message and aborts the entire program.
 */
void fail() {

    cout << "Error: Undefined transition for symbol '" << curSymbol << "' from state '";
    cout <<  toString(prevState) << "'. Thus, the input rejected." << endl;
    cout << "Path: " << path << endl;
    
    abort();    
}

/*  Simulates DFA state transition on the input symbol.
 *  
 *  @param current state
 *  @param input symbol
 *  @return transited state or error.
 */
State transition(State currentState, char symbol) {
    switch (currentState) {
        case A:
            if (symbol == 'a') 
                return B;
            else if (symbol == 'b') 
                return C;
        case B:
            if (symbol == 'b') 
                return C;
            else 
                fail();
        case C:
            if (symbol == 'b') 
                return C;
            else 
                fail();
        default:
            fail();
    }
}

int main() {
    string input;
    cout << "Enter a string (consisting of 'a' and 'b'): ";
    cin >> input;

    State currentState = A; // Start at A

    // Process each symbol in the input string
    for (int i = 0; i < input.length(); i++) {
        char symbol = input[i];
        curSymbol = symbol;
        prevState = currentState;
        currentState = transition(currentState, symbol);

        string stateStr = toString(currentState);
        path = path + "->" + stateStr;
    }
    
    // In this example, we do not need to check if the final state is an accepting or not
    // as all states are accepting states. This is added for the demonstration.
    if (isAcceptingState(currentState)) {
        cout << "Input accepted!" << endl;
    }
    else {
        cout << "Input rejected!" << endl;
    }

    cout << "Path: " << path << endl;

    return 0;
}
