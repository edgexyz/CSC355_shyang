/*
 * Predictive Parser for Arithmetic Expressions
 * --------------------------------------------
 * Description:
 * This program implements a predictive parser for simple arithmetic expressions
 * using a predefined grammar and parsing table. It processes input expressions 
 * based on a top-down parsing approach and determines if the input is syntactically valid.
 *
 * Grammar:
 * The grammar for this parser is as follows:
 *     E  → TE'
 *     E' → +TE' | ε
 *     T  → FT'
 *     T' → *FT' | ε
 *     F  → (E) | id
 *
 * Parsing Table:
 * Each cell in the table corresponds to a production rule or an error.
 * Empty cells ("") represent epsilon productions, and "error" denotes invalid transitions.
 *
 * Author: [Your Name]
 * Date: [Current Date]
 *
 * Compilation:
 * This program requires a C++11 or later compiler. To compile:
 *     g++ -std=c++11 top-down.cpp -o top-down
 *
 * Usage:
 * Run the compiled program and it will parse multiple hardcoded test expressions.
 * Modify the `testInputs` vector in `main()` to add or change test cases.
 *
 * Note:
 * In the parsing table and code, non-terminals with a prime (e.g., T') are represented
 * in lowercase (e.g., "T'" is written as "t") for simplicity.
 */

#include <iostream>
#include <stack>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

unordered_map<string, int> terminalMap = {
    {"i", 0}, {"+", 1}, {"*", 2}, {"(", 3}, {")", 4}, {"$", 5}
};

unordered_map<string, int> nonTerminalMap = {
    {"E", 0}, {"e", 1}, {"T", 2}, {"t", 3}, {"F", 4}
};

// Parsing table
vector<vector<string>> TABLE = {
    {"Te", "error", "error", "Te", "error", "error"},     // E
    {"error", "+Te", "error", "error", "", ""},           // E'
    {"Ft", "error", "error", "Ft", "error", "error"},     // T
    {"error", "", "*Ft", "error", "", ""},                // T'
    {"i", "error", "error", "(E)", "error", "error"}      // F
};

stack<string> parseStack;


/**
 * Pushes a symbol onto the parse stack.
 * @param symbol The grammar symbol to push onto the stack.
 */
void push(const string &symbol) {
    parseStack.push(symbol);
}

/**
 * Pops the top element from the parse stack.
 */
void pop() {
    if (!parseStack.empty()) {
        parseStack.pop();
    }
}

/**
 * Retrieves the top element of the parse stack.
 * @return The top element of the stack as a string.
 */
string top() {
    return parseStack.empty() ? "" : parseStack.top();
}

/**
 * Checks if a given symbol is a terminal.
 * @param symbol The symbol to check.
 * @return True if the symbol is a terminal, false otherwise.
 */
bool isTerminal(const string &symbol) {
    return symbol == "*" || symbol == "+" || symbol == "i" || symbol == "(" || symbol == ")" || symbol == "$";
}

/**
 * Looks up the production for a given non-terminal and terminal in the parsing table.
 * @param X The non-terminal symbol on top of the stack.
 * @param a The current input symbol.
 * @return The production corresponding to the parsing table entry for (X, a).
 */
string table(const string &X, const string &a) {
    int aIdx = terminalMap[a];
    int XIdx = nonTerminalMap[X];
    return TABLE[XIdx][aIdx];
}

/**
 * Parses the input tokens based on the parsing table and grammar rules.
 * @param input A vector of tokens representing the input, ending with the "$" symbol.
 * @return True if the input is successfully parsed according to the grammar, false otherwise.
 */
bool interpret(const vector<string>& input) {
    
    // COMPLETE ME

}

int main() {
    // Define multiple test inputs
    vector<vector<string>> testInputs = {
        // Passing cases
        {"i", "+", "i", "*", "i", "$"},           // Simple expression with addition and multiplication
        {"i", "*", "i", "+", "i", "$"},           // Different order of operations
        {"(", "i", "+", "i", ")", "*", "i", "$"}, // Expression with parentheses
        {"i", "+", "(", "i", "*", "i", ")", "$"}, // Parentheses around multiplication
        {"(", "(", "i", "+", "i", ")", "*", "i", ")", "$"}, // Nested parentheses
        {"i", "$"},                               // Single identifier
        {"(", "i", ")", "$"},                     // Single identifier in parentheses

        // Error cases
        {"i", "*", "+", "i", "$"},                // Operator without operand in between
        {"i", "+", "*", "i", "$"},                // Consecutive operators without operand
        {"(", "i", "+", "i", "$"},                // Missing closing parenthesis
        {"i", "+", "(", "i", "*", "$"},           // Missing closing parenthesis and identifier
        {"i", "*", "(", "+", "i", ")", "$"},      // Operator inside parentheses without preceding identifier
        {"+", "i", "$"},                          // Starting with an operator
        {"(", ")", "$"},                          // Empty parentheses
        {"i", "+", "$"},                          // Ends abruptly with an operator
    };

    // Iterate over each test input and parse it
    for (size_t i = 0; i < testInputs.size(); ++i) {
        cout << "Test Input " << (i + 1) << ": ";
        for (const auto& token : testInputs[i]) {
            cout << token << " ";
        }
        cout << endl;

        // Test the current input with interpret function
        if (interpret(testInputs[i])) {
            cout << "Parsing successful!" << endl;
        } else {
            cout << "Parsing error!" << endl;
        }

        cout << "------------------------" << endl;
    }

    return 0;
}
