# GPL Phase-2

### Due Date
Thursday, September ~19~ 24, 2024, 11:59 p.m.

### Title
Lexical Analyzer and Parser Generator Tools: Flex and Bison

### Starter Files

The **ONLY** files you will edit are **record.l** and **record.y**. You **MUST** not change any other files.

- **record.l**: input for the scanner generator, flex.
- **record.y**: input for the parser generator, bison.
- **error.h/error.cpp**: error reporting class; used all semester to make sure all errors are uniform.
- **parser.h**: wrapper for the bison generated y.tab.h include file.
- **parser.cpp**: main program that reads command line arguments, starts the parser, and prints information about the execution.
- **Makefile**: make utility script (talk to instructor if you don't know how to use make).
- **p2_tester**: in this directory, you will find a binary executable comparing the out from your parser to the expected output.
- **tests/**: directory where you will find all the input test files and expected output saved in files.

If you have completed Lab 2 (implementing **record.l**), copy over the file to your _P2_ directory.

### How to Setup
1. Create _P2_ directory under _GPL_ directory.
2. Copy over all the files (and tests/) to _P2_.

~3. Run `$make` to ensure you can compile the program.~

~4. If you see `parser` generated, you can work on P2.~

Steps 3 and 4 won't work with the new starter `record.l` file as I added more code for the students.
If you attempt to compile with the latest code in this directory, you will get the following error:
```
record.l: In function ‘void get_double_value()’:
record.l:62:10: error: ‘YYSTYPE’ {aka ‘union YYSTYPE’} has no member named ‘union_double’
   62 |   yylval.union_double = atof(yytext);
      |          ^~~~~~~~~~~~
make: *** [Makefile:26: lex.yy.o] Error 1
```

This is because we did not declare union_double in `record.y`. This will go away if you complete step 1 (see **Program Requirements** below).

### Description
The goal of this phase is to help students become familiar with lexical analyzer and parser generator tools, Flex and Bison. You are provided with a working parser that reads a collection of record declarations written in a _language_ for specifying records.  After each record is read, the program prints the record to standard output.

Each record is named (like the variable **int value**; it is named **value**).  Each element of the record is also named. Given the following input,

```
record sally
{
  age = 42;
}

record george
{
  name = "george";
  age = 31;
  phone = 5551212;
  height = 5.7;
}
```

Your task is to update the provided parser that will parse the input and output the following:

```
record sally
{

  age = 42 (int)

} 1 field was declared

record george
{

  name = 'george' (string)
  age = 31 (int)
  phone = 5551212 (int)
  height = 5.7 (double)

} 4 fields were declared.

2 records were declared.
```

### Program Requirements

1. Add a double declaration to the union. Check how an integer union is declared in `record.y`.
2. All tokens (e.g., T_ASSIGN, T_INT_CONSTANT) passed from the lexer to the parser must be declared. For example, `%token T_ASSIGN "="`.
3. Count the fields in each record  [tests: t001.in/t001.out, t002.in/t002.out] 
4. Add fields of type double constant (e.g., 3.1, .42, 42.)  [tests: t003.in/t003.out, t004.in/t004.out]
5. Add fields of type string constant (e.g., "hello world")  [tests: t005.in/t005.out, t006.in/t006.out]
6. Add fields of type date (e.g.,  FEB 22, 1745, AUG 11, 1892) [tests: t008.in/t008.out]
7. Fix the rules for recognizing the identifiers (the names) to contain underscores [tests: t009.in/t009.out]
8. Fix the rules for recognizing the identifiers to contain digits if the first character is not a digit [tests: t010.in/t010.out t007.in/t007.out]

### Prerequisite

1. GPL binary that is suitable for your machine's architecture.
2. C/C++ compiler (GCC, G++, Clang, Clang++)
3. Bison
4. Flex
5. OpenGL

### Hints
1. Count the fields in each record. Look at `record.y` and figure out how the number of records is counted.  The counting happens in the declaration_list rule, and the count is printed in the program rule. Counting fields works the same way.  You will count in the rule that is analogous to the declaration_list rule and print the total in the rule that is analogous to the program rule.  The only changes are in `record.y`.
  - If you have completed steps 1 and 2 in the **Program Requirements**. Try the following:
  - Create your test file, e.g., `empty.in`, with one or more records with no fields.
    ```
    record record1{
    }
    ```
    ```
    record record2{
    }
    ```
  - Compile your parser and run your test file to see how the number of records changes.
  - Remember, grammar is parsed using a recursion.
3. Add fields of type double constant (e.g. 3.1, .42, 42.). You need to change both `record.y` and `record.l`. Look for everything that has to do with declaring an integer field (`T_INT_CONSTANT`, `union_int`, `get_int_value()`, and the field rule for an integer).  Replicate everything in both `record.y` and `record.l` so it handles doubles.  Most of the replication is simple substitution (DOUBLE for INT and double for int), but the regular expression for doubles and the get_double_value() function must be modified to handle doubles instead of integers. You have to add a new grammar rule (bold below) to the non-terminal symbol field.  Rules are separated with a pipeline character, `|`.
```
field:
  T_ID T_ASSIGN T_INT_CONSTANT T_SEMIC
  {
    cout << "  " << *$1 << " = " << $3 << " (int)\n";
  }
  |
  T_ID T_ASSIGN T_DOUBLE_CONSTANT T_SEMIC
  {
    cout ...
  }
  ;
```
3. Add fields of type string constant (e.g "hello world"). Similar to adding a double.  Also requires a new grammar rule. The tricky part is that you must remember to remove the `"` from the string the scanner matches. The really tricky part is figuring out how to pass test `t006`.  Think carefully about your regular expression for a string. The string constant cannot contain a `"` or even `\"`.
4. Add fields of type date (e.g.  FEB 22, 1745, AUG 11, 1892). Do not create a single token for an entire date.  Break it into several tokens: ```T_ID T_ASSIGN T_MONTH T_INT_CONSTANT T_COMMA T_INT_CONSTANT T_SEMIC```. While it would be possible to do some error checking (e.g., FEB 3000, 42) using the flex rule, complete error checking (e.g., FEB 30) can be done in the parser. Create a single token that matches only JAN or FEB or MAR or ... or DEC. Put the matching string (yytext) into the union (yylval). In other words, don't create a T_JAN, T_FEB, ...   Create a T_MONTH and put the month string in the union. Then, follow the same pattern used to add double constants and string constants.
5. Fix the rules for recognizing the identifiers (the names) to contain underscores. When you get the above steps working, this should be easy.  All you have to do is modify the regular expression for the identifier.
6. Fix the rules for recognizing the identifiers so they can contain digits as long as the first character is not a digit. Just like the last step, all you have to do is modify the regular expression for identifiers.
7. In `record.y`, you shouldn't change productions for `program:` and `declaration_list`.

### How to Test Your Program

1. Go to _P2_ directory, where you should have all your files for phase 2.
2. Compile your parser (`$make`). You should see a `parser` binary executable file generated (You can ignore warnings).
3. Run your parser with the test file, e.g., `$./parser tests/t001.in`
4. Compare the output with the expected output, e.g., `tests/t001.out`
5. To test all your programs to see whether you passed, run `p2_tester`. E.g., `$p2_tester/p2_tester`. Make sure you have your compiled file, `parser`, you are in the _P2_ directory, and Python3 installed.
   
### How to Submit

1. Add all files to the GitHub repository.
2. Commit and push to your repository.
3. Make sure you can see your files online.
