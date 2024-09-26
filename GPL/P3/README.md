# GPL Phase-3

### Due Date
Friday, October 4, 2024, 11:59 p.m.

### Title
Implement GPL Scanner and Parser

### Starter Files

The **ONLY** files you will edit are **gpl.l** and **gpl.y**. You **MUST** not change any other files.

- **gpl.l**: Lexer file.
- **gpl.y**: Parser file.
- **gpl.cpp**: main() program for gpl.  The C pre-processor is used to customize it for the different phases
- **Makefile**: Makefile to compile the project (this Makefile works for p3-p7)
- **tokens**: Lists all the tokens in gpl.
- **grammar**: Contains all the rules (also called productions) for gpl.
- **parser.h**: Substitutes for y.tab.h. Always include **parser.h** instead of y.tab.h (Must update each time you add a new type to the flex/bison union.  Do not forget about this file.)
- **error.h** and **error.cpp**: An error reporting class that ensures your errors match my errors letter for letter (Never change this file).
- **gpl_assert.h** and **gpl_assert.cpp**: A standard assert implementation that uses functions so they can be traced by the debugger (Do not change this file).
- **tests/**: directory where you will find all the input test files and expected output saved in files.

### How to Setup
1. Create _P3_ directory under _GPL_ directory.
2. Copy over all the files (and tests/) to _P3_.
3. Make an initial commit to your repository.

### Description

Write a lexical analyzer (gpl.l) and parser (gpl.y) for gpl.

The lexical analyzer should handle all the keywords in gpl, all the special symbols (e.g. `;` and `.`), the operators, and the types (`integers`, `doubles`, `strings`).  A complete list of tokens is in `P3/tokens`.

The parser should handle all the grammar (a complete list of grammars can be found in `P3/grammar`).  It should parse any syntactically legal gpl program but not do anything (your solution will be the parser w/o any actions in { }). For example, the following GPL program (`tests/t002.gpl`) 
```
double x;

on leftarrow
{

    x += 1;
}
```
will output the following (`tests/t002.out`):
```
gpl.cpp::main()
  input file(tests/t002.gpl)
  random seed(42)
  read_keypresses_from_standard_input(false)
  dump_pixels(false)
  symbol_table(false)
  graphics(false)

gpl.cpp::main() Calling yyparse()


gpl.cpp::main() after call to yyparse().

No errors found (parser probably worked correctly).

Graphics is turned off by the Makefile.  Program exiting.

gpl.cpp::main() done.
```

#### Program Requirements

- You **must** define rules for **all** tokens (listed in `P3/tokens`) in `gpl.l`.
    - All tokens (e.g., `%token T_INT "int"`, `%token <union_int> T_FORWARD "forward"`, etc.) are already declared in `gpl.y` for you.
- Add rules in `gpl.l` to tokenize the input. You will use the appropriate emit function (e.g., `emit()`, `emit_id()`, etc.) to pass the token to the parser. For example, `int    return emit(T_INT);`. You need to complete the following emit functions in `gpl.l`:
    ```
    int emit_str_constant(int token)    // This function is to handle the string value wrapped with ""
    int emit_id(int token)
    int emit_int(int token)
    int emit_double(int token)
    ```
    - HINT 1: You need to emit the tokens using `int emit(int token)` function for the static tokens (again, see the list of tokens in `P3/tokens`).
    - HINT 2: you cannot simply use `int emit(int token)` function for non-static tokens (e.g., `T_ID`, `T_INT_CONSTANT`, etc.).
    - HINT 3: The emit functions you need to complete do two things: (1) pass the semantic value to the parser, and (2) pass the token to the parser.
- Each time we emit a token, we increment the `line_count` if the token is mapped with the last string in the line (i.e., a string with `\n` character). Check `int emit(int token)` and `void count_lines(char *str)` functions. Counting the line number is useful when debugging. There are three tokens, i.e., `T_EXIT`, `T_PRINT`, and `T_FORWARD`, we need to return the `line_count` value to the parser. You can do this by emitting the token using `int emit_with_line_number(int token)` function. Simply using `int emit(int token)` function for this phase will not cause a problem, i.e., you will still pass all the tests. However, you will face a problem in the future, so handle this in this phase.
- You need to complete the grammar rule section in `gpl.y`. To start, copy all the rules in `P3/grammar` file to `gpl.y`. Some of the rules are **incomplete**. Your job is to complete them.
    - HINT 1: Please review the test files to find which syntaxes you need to handle.
    - HINT 2: The following rules are incomplete:
        - `declaration`
        - `simple_type`
        - `forward_declaration`
        - `keystroke`
        - `statement`
        - `math_operator`
- The grammar may not make sense in this phase as the rules have no action (body), which means they are simply receiving the tokens and matching with the correct grammar rule.

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
