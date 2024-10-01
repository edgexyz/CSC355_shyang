/*
 * This program simulates DFA.
 *
 * Author: ?
 */


#include <iostream>
#include <string>
#include <cassert>
using namespace std;

// DFA States
enum State {
    // COMPLETE ME
    ERROR // Error state for undefined transitions
};

/*  Checks if the current state is an acceptign state.
 *  
 *  @param current state
 *  @return true if the current state is an accepting state. false, otherwise.
 */
bool isAcceptingState(State currentState) {
    // COMPLETE ME
    return false;
}

/*  Convert enum State type to string.
 *  
 *  @param current state
 *  @return state in string
 */
string toString(State currentState) {

    string state;

    // COMPLETE ME

    return state;
}

/*  Simulates DFA state transition on the input symbol.
 *  
 *  @param current state
 *  @param input symbol
 *  @return transited state or error.
 */
State transition(State currentState, char symbol) {
    // COMPLETE ME

    return ERROR;
}

int main() {
    string inputs[8] = {
        "a",        // accept.
        "aa",       // accept.
        "ab",       // accept.
        "aba",      // accept.
        "abb",      // accept.
        "b",        // reject.
        "abaa",     // reject.
        "abbb"      // reject.
    };

    for (int i = 0; i < sizeof(inputs)/sizeof(inputs[0]); i++) {

        State currentState = A; // Start at A

        string path = "A";
        char curSymbol;
        State prevState;

        string input = inputs[i];
        cout << "Input string: " << input << endl;
        // Process each symbol in the input string
        for (int j = 0; j < input.length(); j++) {
            char symbol = input[j];
            prevState = currentState;
            currentState = transition(currentState, symbol);

            string stateStr = toString(currentState);
            path = path + "->" + stateStr;
            
            // If we encounter an undefined transition (ERROR state)
            if (currentState == ERROR) {
                cout << "Error: Undefined transition for symbol '" << symbol << "' from state '";
                cout <<  toString(prevState) << "'. Thus, the input rejected." << endl;
                cout << "Path: " << path << endl;
                break; // Exit with an error
            }
        }
        
        if (currentState != ERROR) {
            if (isAcceptingState(currentState)) {
                cout << "Input accepted!" << endl;
            }
            else {
                cout << "Input rejected!" << endl;
            }
            cout << "Path: " << path << endl;
        }
    }
    
    return 0;
}
