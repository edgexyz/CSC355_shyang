# GPL Phase-5

### Due Date
Friday, November 8, 2024, 11:59 p.m.

### Title
Expressions

### IMPORTANT

According to the author of the GPL project, the students find this phase most challenging, which I **strongly** agree.
The students who finish this phase on time usually finish the entire project.
Please start **early** as soon as you complete _P4_.

### Starter Files

You **MUST** not change **gpl_type.h** and **gpl_type.cpp** files.

- **constant.h**: constant header file (DO NOT CHANGE).
- **constant.cpp**: constant class implementation file (DO NOT CHANGE).
- **expression.h**: expression header file.
- **expression.cpp**: expression class implementation cpp file.
- **variable.h**: variable header file.
- **variable.cpp**: variable class implementation cpp file.
- **expression_grammar**: a file holding all the rules used to parse expressions.

### How to Setup
1. Create _p5_ directory under _GPL_ directory.
2. Copy over all the files (and tests/) to _p4_.
3. Copy all the files from _P4_ (except tests/ in _p4_) to _p5_.
   - Before copying over, make sure you do not have auxiliary (e.g., .o) files and generated executable `gpl` binary file.
   - You can remove them by running `$make clean` in _p4_.
4. In the **parser.h**, add the following above `#include "y.tab.h"`
   ```
   class Expression;
   class Variable;
   ```
5. Make an initial commit to your repository.

### Description
- In this phase, you implement the `Expression` class to add expression trees in the parse tree.
- Specifically, you are implementing a class that describes a node in an expression tree.
- Since a pointer to the root of an expression tree (a node in the tree) is a pointer to an entire expression tree, an expression tree can be stored with a single pointer to the root.
- After you have implemented the expression class, add actions to your gpl.y that build an expression tree when an expression is parsed.  Specifically, you need to implement actions for all rules that have the following non-terminals as their left-hand-side (it will help you design the expression class if you understand the code you will write for these actions):
   ```
   expression
   primary_expression
   optional_initializer
   math_operator
   variable    // except for the rules that have a T_PERIOD on the right-hand-side, they will be implemented in p6.
   ```
- Once your parser is building expression trees, add actions to initialize the variables of the following types.
   - integer
   - double, and
   - string
- This is done in the rules that have optional_initializer as their left-hand-side.
- When you are done, you will be able to initialize int/double/string variables to any legal expression:
   ```
   int i = 42;
   int j = i * 42;
   double x = i * j * 1.42 / (55 + 2 * i);  // integers are automatically cast to doubles
   string = "hello" + " " + "world";
   string = "x = " + x;                     // doubles and integers are automatically cast to string
   ```
- For assignment `p4`, you changed the rule for declaring arrays:

   `simple_type T_ID T_LBRACKET expression T_RBRACKET`

  was changed to:

   `simple_type T_ID T_LBRACKET T_INT_CONSTANT T_RBRACKET`
- For p5, you need to change it back (replace `T_INT_CONSTANT` with `expression`) and update your action for creating an array so that the expression is evaluated to get the size of the array.

#### Program Requirements
In addition to the requirements from p4,  your interpreter must be able to initialize variables (integers, doubles, strings) using a legal expression containing integers, doubles, and/or strings.  You must handle an expression according to the size of an array.

In p4, all variables (integer, double, string) were initialized to the constants `42`, `3.14159`, `"Hello world"`.  In this assignment, if an optional initializer is not specified, use the values `0`, `0.0`, `""`.  Since gpl arrays can never have initial values, they should always be initialized to these default values.

When an error is discovered in an expression that prevents you from creating an expression node, create a constant integer Expression with value = 0.  This allows the parse to continue and potentially find additional errors before stopping.  For example:
```
// The following example does not contain the necessary type-checking expression T_DIVIDE expression
{

    if either $1 or $3 is of type STRING, there is an error in the expression
        // Issue the correct error message to create a placeholder expression because we can't create the correct expression.
        $$ = new Expression(0);  // In my code this creates a constant int expression node w/value = 0
    else
       // create the correct expression   
       $$ = new Expression($1, T_DIVIDE, $3);

}
```
Constant expressions (e.g., `42`, `1.234`, `"hello"`) are **always** leaf nodes (_both the left and right children are NULL_).  The leaf nodes **must** hold a pointer to a class Constant object (see constant.h). **Constant expressions should never change.**

You **should not** store any int/double/string values in an expression node. Class Constant must be used to store constant values. The expression node has a 
`m_constant` is a pointer variable that points to the constant object.

### Hint
There are two primary functions of class Expression. (1) you need to construct the tree, and (2) you need to evaluate the tree (evaluate the expression the tree represents)

Expression construction is straightforward.  You create a new node in the expression tree for each expression and primary_expression rule in the grammar.  Since the parser will match the expression in a bottom-up fashion, you just build the tree as you parse the expression.  Consider the example of the addition rule:
```
expression:
 expression T_PLUS expression
 {
        // check that the type of $1 and $3 are compatible 
         $$ = new Expression($1, PLUS, $3);
 }
```
All you have to do is create constructors for Expression that can handle all the different configurations of an expression tree and call them in the appropriate actions. The constructor shown in the above example is for binary expressions.  It takes an operator (I used an enumerated type `Operator_type` from gpl_type.h) and pointers to two other expressions. Expression_grammar file shows all the rules used to parse expressions.

There are four different flavours of an Expression node.  It is easiest if you implement one class Expression and use member variables to track which flavour of Expression it is.

- **constant (int/double/string)**: Leaf node that holds a pointer to a class Constant object.  m_lhs and m_rhs pointers are NULL.  m_variable pointer is NULL. m_type is INT/DOUBLE/STRING.  m_oper is ignored. E.g., `42`, `1.2`, `"hello"`.
- **binary expression**: Non-leaf node that holds an operator (in m_oper) and pointers to two expressions.  m_lhs points to e1, m_rhs to e2. m_variable pointer is NULL.  m_type is derived from e1, e1, and m_oper. E.g., `e1 + e2`.
- **unary expression**: Non-leaf node that holds and operator (in m_oper) and a pointer to a single expression.  m_lhs points to e, m_rhs is NULL.  m_variable pointer is NULL.  m_type is derived from e and m_oper. E.g., `-e`.
- **variable**: Leaf node that holds a pointer to a Variable object (in m_variable).  m_lhs and m_rhs pointers are NULL.  m_oper is ignored.  m_type is retrieved from m_variable  (m_type = m_variable->get_type()). E.g., `i`.

There are 6 constructors in the `Expression` class.

`lhs`: the left-hand side of the operator.
`rhs`: the right-hand side of the operator.

`e`, `e1`, and `e2`: are pointers to the Expression objects.

**Expression evaluation** functions recursively evaluate each node in the tree.  The algorithm performs a _depth-first traversal_ of the tree.  When a node's evaluation function is called, if the node has children, it is called to evaluate the left child, then the right child, and then applies its operator.  If the node holds a constant (integer, double, string), it returns the value (which it gets from the Constant object: e.g., m_constant.get_int_value()).  If it holds a variable, it evaluates the variable (e.g., my_variable->get_int_value() ) and returns the value. Therefore, this function evaluates the expression and returns the computed value. For example,

```
i                                                                          42

Expression object e                                                        Expression object e
                  |                                                                          |
Variable object   v   // variable object points to symbol objects i        Constant object   c   // constant object holds value 42
```

### Expression Types
Expressions have types, just like Symbols.  An Expression's type is the type of the result of evaluating the expression.  You can dynamically calculate the type of an Expression, but it is much easier, more reliable, and easier to debug if you determine the type at parse time when you are constructing the Expression object (in the Expression's constructor).

Sometimes, it is easy to determine the type of an Expression.  For example, an Expression object with an integer constant is of type `INT`.

On the other hand, determining the type of a binary expression is more subtle.  Consider the following expression: `a + b`

If `a` and `b` are integers, the type of this expression is `INT`.  If `a` is a double and `b` is an integer, the type of this expression is `DOUBLE`.  If either `a` or `b` is a string, the type of this expression is `STRING`.

In the Expression constructors, you have to handle the dozens of combinations to correctly determine the type.  Consider this expression: `a < b`

Relational operators always evaluate to `0` or `1`.  Thus, the type of this expression is INT regardless of the types `a` and `b`.

You **must** consider the many different operators and combinations of operand types when determining the type of an Expression.  **This can take a lot of time.**

Using an Expression's type:  Consider the following example:

```
int i = 42 + "hello world";
```

This input will match the following rule:

```
simple_type T_ID optional_initializer
```

The `int` will match simple_type, the `i` will match `T_ID`, and the `42 + "hello world"` will match expression (optional_initializer derives expression).  Since integers should automatically be converted to strings when in a string expression, the expression `42 + "hello world"` is a valid `STRING` expression (it has the value `"42hello world"`).  In other words, this legal expression is of type `STRING`. In gpl, assigning a string to an integer is a semantic error; thus, the above input should generate an error.

The check for the correct type can be made as follows (assume that optional_initializer returns a pointer to an expression) and your expression class has a get_type() function:

```
simple_type T_ID optional_initializer
{
    if ($1 == INT) // the type of my simple_type token is Gpl_type
    {
        int initial_value = 0;  // 0 is the default value for integers
        // if an initializer was specified
        if ($3 != NULL)
        {
            if ($3->get_type() != INT)
               error -- the initializer is not of the correct type
            else initial_value = $3->eval_int();
        }
        // now a new INT symbol can be created using initial_value and *$2.
     }
    // do other cases here (e.g. $1 == DOUBLE)
}
```

### Expression Evaluation

Evaluating an integer expression is different than evaluating a double expression, which is different from evaluating a string expression.

The evaluation mechanism is much easier if you have three separate type-dependent evaluation functions:

```
int Expression::eval_int()

double Expression::eval_double()

string Expression::eval_string()
```

It is possible to combine them into one function, but your code will probably be a tangled mess.

Since all relational operators (`<`,  `>`, `<=`,  `>=`, etc.) are of type `INT`, the code to evaluate them should **ONLY LIVE** in eval_int().  You must think carefully about which operators are implemented in which eval function.

gpl allows an integer to cast to a double or a string.  A double can cast to a string.  Casting should be handled in the Expression eval functions.  Consider this example where an integer expression is being cast to a string:

```
string Expression::eval_string()
{
    if (m_type == INT)
    {
         int value = eval_int();
         // convert value into a string an return it
    }
    ...
}
```

The other cast operators should be implemented in a similar fashion. `eval_double` and `eval_string` are given to you (already implemented). The only evaluate function you need to implement is `eval_int`.

### Class Variable
class Variable provides an encapsulation of all the different flavors of variables.  Without it, class Expression would have to handle each of these flavors and be more complicated.  A bonus is that in p7, we can use class Variables in assignment statements.  Some students push the variable's functionality into the class symbol, which significantly complicates the symbol and leads to other problems.  You mustn't eliminate class Variable and push its functionality into class Expression and/or class Symbol.

Consider the following flavors of gpl variables:

```
a                       
nums[a + b]
my_rectangle.x               // implemented in p6
rectangles[i + 2].y          // implemented in p6
```

class Variable encapsulates all of these flavors and provides a clean interface for class Expression to use: 

```
m_variable->get_type()

-and-

m_variable->get_int_value()
-or-
m_variable->get_double_value()
-or-
m_variable->get_string_value()
```

The Expression code does not even have to know what flavor of Variable it uses (Expression does not have to know about the array index expression or the member field string).

For p5, the class variable should store a pointer as a symbol and a pointer as an expression.  The Symbol pointer is used for all variables.  The Expression pointer is used only for array variables.  For example:

```
i                // the Symbol pointer will point to i's Symbol.  The Expression pointer will be NULL.
nums[i * j]      // the Symbol pointer will pointer to nums' Symbol.  The Expression pointer will point to the Expression holding "i * j".
```
Starting in p6, class Variable will also need to hold a string for a possible member field.  For example:  my_rectangle.x  the "x" is the member field.

You will need to add a pointers to your expression and variable classes to the union in gpl.y.  This means that you will have to provide forward class declarations in parser.h before y.tab.h is included, which you should have done during the p5 setup.

```
class Expression;
class Variable;
```

~The Expression class holds a pointer to the Variable class.  The Variable class holds a pointer to the Expression class.  If you include variable.h in expression.h and expression.h in variable.h, you will get yourself into an infinite include loop (which the C-preprocessor ignores if you use the #ifndef #define #endif construct).  The solution is to provide forward class declarations instead of including the entire .h files.  For example:~

~expression.h~
    ~class Variable;~
~expression.cpp~
    ~#include "variable.h"~
~variable.h~
    ~class Expression;~
~variable.cpp~
    ~#include "expression.h"~
    
~This only works when you reference a pointer to the object in the .h file.  If Expression contained a Variable (instead of a pointer to a Variable) this would not work.  When possible, you should use "class object;" in a .h file instead of including the entire .h file.  It will reduce compilation dependencies and shorten compilation time.~

~You can convert an integer or double to a string using the C++11 to_string function to_string.~

~You will need to convert integers and doubles to a string in Expression::eval_string().~

For the p5 assignment, you will not be adding an action for the following rules:

```
expression -> expression T_NEAR expression

expression -> expression T_touches expression
```

### Avoiding Common Mistakes

- Do not seed the random number generator.  It is seeded in gpl.cpp via a command line argument.
- The gpl program's int, double, and string values live in two places:  class Constant, class Symbol.  If you store any gpl int/double/string values anyplace else you are heading down a path that will prevent you from finishing the assignment.
- Never change an int/double/string value stored in class Constant (i.e. do not change class Constant).  You have a fundamental misunderstanding of how the expression tree works if you want to change a Constant.
- If you have started every assignment you have ever done right before the deadline and completed it on time, come to my office, and I will plead with you to start p5 right away.  Every student who fails the class starts p5 late.

### WARNINGS
1.  The bulk of this assignment is the creation of the data structure to hold an expression tree.   In many gpl statements, expressions are evaluated at run-time (when the game is running), and thus we need a data structure to hold an expression so that we can evaluate it at run-time.  In this phase of the assignments you will use expression only in one place: variable initialization.  Since variables are initialize only once (at parse time) it would be possible to avoid building an expression tree and simply calculate the value of the expression.  HOWEVER, since we will need the expression tree in later phases YOU MUST NOT SIMPLY EVALUATE THE EXPRESSION WHEN YOU PARSE IT, YOU MUST IMPLEMENT AND BUILD THE EXPRESSION TREE IN P5 and evaluate it to initialize variables.  If you do not implement expression correctly you will not be able to complete future phases.
2.  A small but important part of this assignment is to develop a class that can represent a variable.  I call this class Variable.  Some students eliminate class Variable by pushing the functionality into class Symbol or class Expression.  DO NOT ELIMINATE THE VARIABLE CLASS, IT BECOMES MORE IMPORTANT LATER IN THE PROJECT.
3.  There are a lot of pieces to this assignment.  It takes more time than many students anticipate.  Students who do not finish p5 within three days of the deadline usually do not pass the class.  Don't count on using most of your late days on p5, it usually does not work out.
4.  DO NOT cache any values in any Expression nodes.  For example, consider this code:
   ```
   int i = 30;
   int j = 12;
   int k = i + j;
   ```
   It is tempting to think that 30 + 12 is 42 and the 42 should be saved in the "i + j" expression node.  While this will work for this example and this     phase, it won't work for future phases.  DO NOT CACHE ANY VALUES IN THE EXPRESSION (the constants, 30 and 12 in this example, are stored in a class Constant object and not in the expression).

   When constructing an expression, DO NOT APPLY ANY OF THE OPERATORS, THIS LEADS TO CACHING THE RESULTS.  The operators should only be applied at expression evaluation.  If you evaluate the operators when creating an expression and cache the values, you will not find out you did it wrong until P7.  You will not have enough time to rewrite p5.

5. For this phase the types of expressions are INT, DOUBLE, and STRING.  In future phases there will be a couple more type.  Do not assume the type is always an INT, DOUBLE, or STRING.  Write you code to handle other types (which are all errors in this phase).  You code should follow this pattern (you can use a switch, but make sure there is a default for the error):
```
 if (cur_type == INT)
       handle INT
    else if (cur_type == DOUBLE)
       handle DOUBLE
    else if (cur_type == STRING)   // don't just use "else" assuming the type is a STRING
       handle STRING
    else
       error: illegal type
```

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
