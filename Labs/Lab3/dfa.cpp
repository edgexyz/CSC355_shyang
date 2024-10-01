/*
 * This program simulates DFA.
 *
 * Author: Shu Yang
 */


#include <iostream>
#include <string>
#include <cassert>
using namespace std;

// DFA States
enum State {
    // COMPLETE ME
    A,
    B,
    C,
    D,
    ERROR // Error state for undefined transitions
};

/*  Checks if the current state is an acceptign state.
 *  
 *  @param current state
 *  @return true if the current state is an accepting state. false, otherwise.
 */
bool isAcceptingState(State currentState) {
    // COMPLETE ME
    return currentState == B || currentState == C || currentState == D;
}

/*  Convert enum State type to string.
 *  
 *  @param current state
 *  @return state in string
 */
string toString(State currentState) {

    string state;

    // COMPLETE ME
    if(currentState == A) {
        state = "A";
    }
    else if(currentState == B) {
        state = "B";
    }
    else if(currentState == C) {
        state = "C";
    }
    else if(currentState == D) {
        state = "D";
    }
    else {
        state = "ERROR";
    }

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
    switch(currentState) {
    case A:
        if(symbol == 'a')
            return B;
        else
            break;

    case B:
        if(symbol == 'a')
            return C;
        else if(symbol == 'b')
            return D;
        else
            break;

    case D:
        if(symbol == 'a' || symbol == 'b')
            return C;
        else
            break;
    
    default:
        break;
    }

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
