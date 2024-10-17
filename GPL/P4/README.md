# GPL Phase-4

### Due Date
Friday, October 25, 2024, 11:59 p.m.

### Title
Implement Symbol Table for GPL Interpreter

### Starter Files

You **MUST** not change **gpl_type.h** and **gpl_type.cpp** files.

- **gpl_type.h**: Contains enumerated types for types, operators, and error status.
- **gpl_type.cpp**: Provides routines for converting the enumerated types in gpl_types.h into strings.  This is helpful for debugging and for printing.
- **symbol.h**: A header file for the symbol.
- **symbol_table.h**: A header file for the symbol table.

### How to Setup
1. Create _P4_ directory under _GPL_ directory.
2. Copy over all the files (and tests/) to _P4_.
3. Copy all the files from _P3_ (except tests/ in _P3_) to _P4_.
   - Before copying over, make sure you do not have auxiliary (e.g., .o) files and generated executable `gpl` binary file.
   - You can remove them by running `$make clean` in _P3_.
4. In the **parser.h**, add `#include "gpl_type.h"` above the `#include "y.tab.h"` line.
5. Make an initial commit to your repository.

### Description

- In this phase, you will implement a symbol table for the `gpl` interpreter (class **Symbol** and class **Symbol_table**).
- The header files for both classes are provided. Your task is to implement a function body for each function declaration.
- Add actions (code inside the `{}` after a production) to your **gpl.y** to insert `integer`, `double`, `string`, and `array` declarations into the symbol table.
- If a variable is previously declared or if the size of the array is invalid, issue an error using the `Error::error()` reporting function (see **error.h**).
- To test your symbols, initialize all the integers to `42`, all the doubles to `3.14159`, and all the strings to `"Hello world"`.
   - In the next phase, you will implement expressions, which will be used to initialize variables.

#### Program Requirements
- Class **Symbol_table** must be implemented as a **singleton**.  You **MUST** use the STL `unordered_map` to store symbols in the symbol table (the key is a C++ `string`, and the value is a `Symbol *` (you must use a pointer to a symbol)). This declaration is already done for you. See line 29 in **symbol_table.h** file.
- You must implement class Symbol to hold the name,  value (stored as a void pointer), and type of each variable.
   - You must use the enumerated type [GPL_type](https://github.com/hlim1/GPL/blob/main/Project/P4/gpl_type.h) to store type.
- The Symbol must be able to hold arrays and variables of type `INT`, `DOUBLE`, and `STRING`.
   - You **MUST** use a member `void pointer (void *)` to store a pointer to the value.
   - Use the new operator to allocate the memory for the variable, for example, in the Symbol constructor for an integer
     ```
     m_data_void_ptr = (void *) new int(initial_value);
     m_type = INT;
     ```
- The Symbol_table must have a print function that, in turn, call's the Symbol's print function.
   - Variables must be printed in alphabetical order — the order of the identifier string, not the type name.
   - Since the unordered_map does not sort the entries, you **must** sort the symbols alphabetically in the symbol table's print function.
   - Prints all the elements of the symbol table in the following format (order is the default STL map's order)
     ```
     <type>  <name>  =  <value>
     ```
     For example,
     ```
     int a = 42
     double b = 3.14159
     int c = 42
     string d = "Hello world"
     int nums[0] = 42
     int nums[1] = 42
     int nums[2] = 42
     ```
  - Note:  add " " when printing string constants.  The actual string should NOT contain quotes (your scanner should strip the quotes off of string constants)
- class Symbol stores array and non-array variables.  For this phase, it must hold an `integer`, a `double`, a `string`, an `array of integers`, an `array of doubles`, or an `array of strings`.
   - Since each symbol holds only one variable or one array, you must implement the storage of the variable/array using a single void pointer.  This mechanism is error prone; asserts and error checking can save a tremendous amount of debugging time.
- For this assignment, the following rule in grammar:
   ```
   variable_declaration:
   simple_type T_ID T_LBRACKET expression T_RBRACKET
   ```
   Must be changed to:
   ```
   variable_declaration:
   simple_type T_ID T_LBRACKET T_INT_CONSTANT T_RBRACKET
   ```
   - In the next phase, you will implement expressions and have to change this rule back (comment out the current rule so it will be easier to restore in the next assignment).

### Hint
- Take a look at what is a **singleton** and how it's used in C++. [Singleton in C++](https://medium.com/@antwang/singleton-in-c-651c37ca1d5b).
- Take a look at what is a C++ void pointer is. [C++ Void Pointer](https://www.geeksforgeeks.org/cpp-void-pointer/).
- The current grammar (the one in p3) initializes ints, doubles, and strings using an expression.  It also uses an expression for the size of arrays.  Building the code for the expressions is hard and is the bulk of the next assignment.  For this assignment give default values to variables (see above) so that you can test your class Symbol's set and print functions.
- Write and test your Symbol and Symbol_table classes before you call them from actions embedded in your gpl.y.  In other words, write a stand alone program to test your Symbol and Symbol_table classes.  It is easier to test code when bison is not involved.
- When searching an STL unordered_map to for a specific key, you cannot use the [] operator.  You must use the find() function.  If you use the [] operator and the element is NOT in the map you will end up inserting it into the map.  find() returns an iterator which is pointing to map.end() if the target was not in the map, and the match if it was in the map.
- I included a lot of error checking asserts in my program.  It made it much easier to debug.  I suggest that you use asserts liberally.  When programming, every time you think a variable has a specific value, write an assert statement.  Standard asserts are implemented using pre-processor directives.  That means it is hard to use the debugger (gdb) to break on assert statements.  In the p3 source code I included an assert that uses a function call so you can break on the asserts.  Include gpl_assert.h to use this flavor of asserts.
- When printing the Symbol_table, you must first sort the Symbols alphabetically.  The easiest way to sort them is to first put them all into a vector<Symbol *> and then call the STL sort function.  The STL sort function requires a function that compares elements to be sorted.  I created the function bool compare_symbols(Symbol *a, Symbol *b) that returns true if a's id is less than b's id.

### How to Test Your Program
1. Compile your gpl using make: `$make`
2. Make sure you have successfully compiled and have `gpl` binary executable file.
3. Change the mode of `gt` file: `$chmod 700 gt`. `gt` file is a bash script that will run the test using your `gpl` program.
4. Run `gt` script: `$./gt`
    - `gt` does not run tests you have already passed. To run **all** the tests: `./gt -all`
    - To run the specific test, e.g., t004: `./gt 4` (you do not need the zeros in front of the test number)
5. If you fail a test, you can use `vimdiff` to see the difference.
   - You first need to create a file with **your** output. E.g., `$./gpl tests/t001.gpl > my001.out`.
   - Compare your file with the expected output file. E.g., `$vimdiff tests/t001.out my001.out`.
   - The highlighted parts are where the difference exists.
   - If you want to know more about `vimdiff`, check out [this](https://www.freecodecamp.org/news/compare-two-files-in-linux-using-vim/) post from freeCodeCamp.

### How to Submit
1. Add all files to the GitHub repository.
2. Commit and push to your repository.
3. Make sure you can see your files online.
