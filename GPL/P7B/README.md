# GPL Phase-7

### Due Date
Friday, December 6, 2024, 11:59 p.m.

### Title
Statements, Event Handlers, and Animation Blocks

### Starter Classes (Each Class has .h and .cpp files)

- **Statement**: Base class for all statements, has pure virtual function void execute()
- **If_statement**: Implements an if statement.  It contains an Expression and Statement_blocks for then and else (the else block is optional).
- **Print_statement**: Implements a print statement.  Contains: an Expression.  The Expression is evaluated to a string, and that string is printed.
- **Exit_statement**: Implements an exit statement.  It contains an expression that is evaluated to calculate the exit status.  I.e., the value of the expression is sent to the system function exit().
- **Assignment_statement**: Implements an assignment statement.  Contains: a Variable (the LHS), an expression (the RHS), and an operator type (=, +=,. -=)
- **For_statement**: Implements a for loop statement.  Contains three Statement_blocks (one for initializer, one for incrementor, one for body of the loop) and an Expression.
- **Statement**: Pure virtual base class for actual statement classes.
- **Event_manager**: class Event_manager stores all the event handlers.
- **gpl_y.txt**: Grammar rules with actions that you can use for phase 7. You might (and likely) have to modify the grammar beyond the ones listed here. Again, it is important that you understand the program itself, including the input and output of gpl, to finalize the project.

### Other Files
- **Makefile**: For this phase, you need to use updated Makefile, which is included in this directory.
- **gpl.cpp**: Updated `gpl.cpp` to define P7B instead of P8.

### How to Setup
1. Create _p7a_ directory under _GPL_ directory.
2. Copy over all the files (and tests/) to _p7a_.
3. Make sure to use the new Makefile provided.
4. Copy all the files from _p6_ (except tests/ in _p6_) to _p7a_.
   - Before copying over, make sure you do not have auxiliary (e.g., .o) files and generated executable `gpl` binary file.
   - You can remove them by running `$make clean` in _p6_.
5. In the **parser.h**, add the following above `#include "y.tab.h"`
   ```
   #include "window.h"
   ```
6. Make an initial commit to your repository.

### Overview

This last phase has two parts, phase 7a and phase 7b. You need to complete phase 7a before starting phase 7b. In other words, you should pass all the test cases for phase 7a (test directory: `tests_a`) before running the tests for phase 7b (test directory: `test_b`). Read **How to Test Your Program** section below to learn how to run tests for this phase.

### Description for P7 Part A
Add the ability to parse and execute statements and event handler blocks ("on blocks") to the parser you created in phase 6. To get credit on this assignment, your program **must** open a window, draw objects (as specified in `p6`), and handle events correctly. In other words, it should work just like my posted gpl executables with everything except animation blocks and initialization blocks.  See the gpl manual.

#### Step 0:
Add the following lines of code in the appropriate files, e.g., `gpl.y`:
```
#include "if_statement.h"
#include "for_statement.h"
#include "print_statement.h"
#include "exit_statement.h"
#include "assign_statement.h"
```

#### Step 1
Implement the appropriate grammar actions to handle the statements. For example, the following grammar rule is for handling `print` statement.
```
print_statement:
  T_PRINT T_LPAREN expression T_RPAREN
```

Every statement must belong to one and only one statement block. Since statement blocks can be nested, keeping a **global** stack of statement blocks  (`stack<Statement_block *> statement_block_stack;`) is easiest.

Here is a summary of parsing statements:
1. When you see an `{`, create an empty statement block and push it on the global statement block stack (that is what the non-terminal `statement_block_creator` is for).
2. When you parse a statement, insert the statement onto the statement block at the top of the global statement block stack. For example: `statement_block_stack.top()->insert(new Print_statement(expr, $1));`
3. When you parse an `}`, pop (`statement_block_stack.pop();`) a statement block off of the global statement block stack (this is what the non-terminal `end_of_statement_block` is for).  A pointer to this statement block will always be used by either (1) an `if` statement, (2) a `for` statement, (3) an `event handler`, or (4) an `animation block`.

For this step, you need to define **all** the actions for statement rules. The following example (`print_statement` grammar rule) can be directly copied and pasted into your code.

**print_statement**
   ```
   print_statement:
     T_PRINT T_LPAREN expression T_RPAREN
     {
         Expression *expr = $3;
         if (expr->get_type() != INT
           && expr->get_type() != DOUBLE
           && expr->get_type() != STRING
          )
         {
           Error::error(Error::INVALID_TYPE_FOR_PRINT_STMT_EXPRESSION);
           // for error handling
           expr = new Expression(0);
         }
         statement_block_stack.top()->insert(new Print_statement(expr, $1)); // $1 has line_number
     }
   ```

- Check out the `gpl_y.txt` file for the provided code.

#### Step 2
The event manager is called from `window.cpp`. ~Thus, you must use the same function names for Event_manager that I use in `window.cpp`.~

All the event manager does is keep a vector of pointers to Statement_block (an STL vector works well) for each type of event (space, left-arrow, right-arrow, down-arrow, up-arrow, etc.  See **gpl manual** for the complete list).  

When an event block is parsed (an `on block`) you insert the new `statement_block` into the `event_manager` (this is done in the action for the non-terminal `on_block`).
```
on_block:
   T_ON keystroke statement_block
```

Your task is to implement the action for the rule. It's a simple insertion using the `Event_manager::register_handler()` function. Check out the `event_manager.cpp` file to understand the structure, e.g., parameters, of the function to understand the behavior of the function.

### Description for P7 Part B

(1) Add animation blocks to the parser you created in program 7.  Since your program should already be able to parse a statement block, all you have to do is:
   (a) write the action for animation blocks in gpl.y
   (b) implement the game_object parameter for animation blocks

(2) Implement the touches and near operators.  This will require small additions to your expression class.

(3) Implement initialization and termination blocks (this is trivial).

(4) Handle some animation block-related errors.

#### Step 0

Change the directory, `p7a`, and name it `p7b`. This is needed to handle the path issue when compiling.


#### Step 1

**Animation block**

Recall part A that when parsing a statement block, you created a new `Statement_block` and pushed it onto the statement block stack when you parsed the `{`, and when you parsed the `}`, you popped it off the stack.  This way, each statement you parsed (while inside the block) could be inserted into the `Statement_block` at the top of the stack.

When parsing the forward statement, you create an empty Animation_block and put it into the symbol table.

When you parse the `{` of an animation block, instead of creating a new `Statement_block`, you need to get the current animation block's `Animation_block` out of the symbol table and push it on top of the stack.  At the end of an animation block, you pop the `Animation_block` off the stack.

Since `Animation_block` inherits `Statement_block`, there is no problem pushing it onto the stack and using it when you are inserting statements.  In other words, you can code as if you were using a `Statement_block *` instead of an `Animation_block *`.

The easiest way to do this is to add an action in the middle of the rule for animation_block:
```
animation_block:
	T_ANIMATION T_ID T_LPAREN check_animation_parameter {put action here} T_RPAREN T_LBRACE statement_list T_RBRACE end_of_statement_block
```

In this action, you will look up the animation block in the symbol table (its name is $2).  Then, you push it on the statement block stack.  Thus, you will be filling in the animation block that was created in the forward statement instead of creating a new one.

#### Step 2

**Getting parameters to work**

Building a mechanism that works like parameter passing in C++ would be difficult, so I came up with a hack.  If you can come up with a cleaner way to implement this, I would like to hear about it.

When an animation block is created by a forward statement, the formal parameter variable is created and placed into the symbol table as if it were a global variable (except it is not drawn or animated).

When an animation block is executed, the actual parameter must replace the formal parameter.

Consider the following gpl code:

```
forward animation go(rectangle cur_rect);

rectangle my_rect(animation_block = go);

animation go(rectangle cur_rect)
{
cur_rect.x += 10;

}
```

The formal parameter is `cur_rect`.  The actual parameter is `my_rect`.  `cur_rect` is created and placed in the symbol table when the forward statement is parsed.  When the statements in the animation block are parsed, they reference `cur_rect`.  Since `cur_rect` is in the symbol table, you can parse code that contains `cur_rect` just like the code in on blocks.

When the animation block go is executed for the rectangle `my_rect`, `cur_rect` needs to be "replaced" with `my_rect`.  Ok, here comes the hack:  Before executing the statement block, modify `cur_rect`'s symbol so that it points to my_rect's Game_object.  It sounds like a bad hack, but it is very easy to implement.

In order to implement this:
   (1) Make sure you are creating the formal argument when you parse the forward statement and passing it to Animation_block's constructor.
   (2) Implement Animation_block::execute(Game_object *argument).  This function should do the following:
      (a) modify the symbol for the formal parameter so it points to argument
      (b) call Statement_block::execute()
      (c) undo the modification you did in step (a)

#### Step 3

**Near and touches**

`near` and `touches` are binary operators that take two Game_objects and return an INT (`0` or `1`).

Game_object implements the actual functionality, all you have to do is call near() or touches() when you evaluate a binary expression where the operator is near or touches.

You will have to add actions in gpl.y to handle these operators.  Since the operands are always variables, there are minimal changes (i.e. being able to detect the GAME_OBJECT type variable) to the construction of your Expression.  You will have to change Expression::eval_int() to be able to handle these operators.

#### Step 4

**Initialization blocks**

The window manager (window.cpp) makes the following call right before program execution starts:
```
Event_manager::instance()->execute_handlers(Window::INITIALIZE);
```
This means that all you have to do to implement initialization blocks is to insert the block into the Event_manager when you parse it (in the action for initialization_block).  Just like event blocks, there can be many initialization blocks.

You should not need to change `Event_manager::insert()` because `INITIALIZATION` is a keystroke just like `SPACE`.

#### Step 5

**Termination blocks**

The main program (gpl.cpp) makes the following call right before program exits:
```
Event_manager::instance()->execute_handlers(Window::TERMINATE);
```
This means that all you have to do to implement termination blocks is to insert the block into the Event_manager when you parse it (in the action for termination_block).  Just like event blocks, there can be many termination blocks.

You should not need to change `Event_manager::insert()` because TERMINATE is a keystroke just like `SPACE`.

You must modify your Exit_statement::execute() so it tells the Event_manager to execute the TERMINATE handlers
```
Event_manager::instance()->execute_handlers(Window::TERMINATE);
```

### Program Requirements

### How to Test Your Program
1. Compile your gpl using make: `$make`
2. Make sure you have successfully compiled and have `gpl` binary executable file.
3. Change the mode of `gt` file: `$chmod 700 gt`. `gt` file is a bash script that will run the test using your `gpl` program.
4. Run `gt` script: `$./gt`
    - In this phase, you need to change lines 10 and 14 in `gt` file to run the correct tests for each part of p7.
    - To test phase 7a, change the code at line 10 to the following `test_dir=tests_a` and change the code at line 14 to `result_dir=results_a`
    - To test phase 7b, change the code at line 10 to the following `test_dir=tests_b` and change the code at line 14 to `result_dir=results_b`
    - You should pass all the phase 7a tests before starting phase 7b.
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
