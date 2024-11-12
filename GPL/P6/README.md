# GPL Phase-6

### Due Date
Friday, November 22, 2024, 11:59 p.m.

### Title
Game Objects & Forward Declarations

### Starter Classes (Each Class has .h and .cpp files)

You **MUST** not change **gpl_type.h** and **gpl_type.cpp** files.

- **Window**: creates `gpl window`, only called from `gpl.cpp`.  You don't do anything with it in p6.
- **Game_object**: Base class for all game objects.  You won't create a Game_object (you create objects that inherit it), but your program will deal with Game_object pointers (Game_object *).
- **Rectangle**: rectangle game object.
- **Circle**: circle game object.
- **Textbox**: textbox game object.
- **Triangle**: triangle game object.
- **Pixmap**": pixmap game object (uses default_pixmap.h).
- **Statement_block**: Skeleton class to act as the base class for class Animation_block (see below).  Will eventually hold statements for animation blocks and event blocks (i.e., will hold if statement, for statements, etc.).  In p7, you will have to change this class.
- **Animation_block**: Skeleton class so you can set the animation_block parameter in p6.  In p6, you will use pointers to `Animation_block`.  You will need to add one or more functions to `Animation_block` for p6 for error checking.  In p7, you will also have to modify it.
- **Event_manager**: Skeleton class so that `gpl.cpp` will compile.  You don't have to do anything with it in p6.  In p7 you have to implement this class.

### How to Setup
1. Create _p6_ directory under _GPL_ directory.
2. Copy over all the files (and tests/) to _p6_.
3. Copy all the files from _p5_ (except tests/ in _p5_) to _p6_.
   - Before copying over, make sure you do not have auxiliary (e.g., .o) files and generated executable `gpl` binary file.
   - You can remove them by running `$make clean` in _p5_.
4. In the **parser.h**, add the following above `#include "y.tab.h"`
   ```
    #include "event_manager.h"
    #include "game_object.h"
   ```
5. Make an initial commit to your repository.

### Description

In this phase, you will add game objects and forward declarations to the parser you created in phase 5.

### Program Requirements

After completing this phase 6, your gpl parser **must** handle the following:

|All the grammar that phase 5 can handle|                                                           |
----------------------------------------|------------------------------------------------------------
|Forward declarations|                                  forward animation bounce(circle cur_circle);|
|game object declarations|rectangle my_rectangle(); circle my_circle(x = 100, y = 100, radius = 30);|
|expressions with animation blocks|                      circle my_circle(animation_block = bounce);|
|expressions with member variables|                                     int i = my_rectangle.x + 20;|

When you print game objects that are in the symbol table, you should print them using the print function Game_object::print().
Since you are using my code, the output should automatically look like:

```
Rectangle
{
  animation = 0,
  blue = double(0),
  green = double(0),
  h = int(10),
  red = double(1),
  rotation = double(0),
  visible = int(1),
  w = int(10),
  x = int(5),
  y = int(6)
}
```

The game objects and the window object use the **OpenGL** and **GLUT** libraries.
You will have to have these installed on your machine before you can use these files.
Additionally, to pass all the tests, you need the window to open and closed with the 'q' key pressed.
To learn how to install the libraries, check out `CSC355_Student/GPL/README.md` file.

NOTE: A significant part of this assignment is understanding how all the parts of GPL code work.
By this time, you should have a good understanding of the code, but please do understand what each part does if not, before starting this phase.

### Steps to Follow

#### Step 0

Include the header files:

```
#include "event_manager.h"
#include "statement_block.h"
#include "animation_block.h"
#include "game_object.h"
#include "triangle.h"
#include "pixmap.h"
#include "circle.h"
#include "rectangle.h"
#include "textbox.h"
#include "window.h"
```

#### Step 1

Update the `Symbol` class so it can handle game objects.  Class `Game_object` is the base class for all game objects. Thus, you want to be able to put a `Game_object *` in the `Symbol` class. Add the following function declarations in the `symbol.h` file:
```
// Constructor for game_objects
Symbol(std::string name, Gpl_type type);

// Returns game_object value
Game_object* get_game_object_value(int index) const;
```

Add the following functions to the `symbol.cpp` and complete the functions:

Constructor for a single game object declaration.

```
Symbol::Symbol(string name, Gpl_type type)
{
    assert(type == CIRCLE
           || type == RECTANGLE
           || type == TRIANGLE
           || type == TEXTBOX
           || type == PIXMAP
           || type == ANIMATION_BLOCK
          );
  
    m_name = name;
    m_type = type;
    m_size = UNDEFINED_SIZE;
  
    switch(type)
    {
      // COMPLETE ME
      default: assert(0);
    }
}
```

The getter function for retrieving the game object value from the array of game objects.

```
Game_object *Symbol::get_game_object_value(int index /* = UNDEFINED_INDEX */) const
  {
    validate_type_and_index(GAME_OBJECT, index);
  
    if (is_array())
    {
      // since this is an array of actual object, can't consider it an array of Game_object
      // must consider each type: Rectangle, Triangle, etc...

      switch(m_type)
      {
        case CIRCLE_ARRAY:
        {
          Circle *object_array = (Circle *) m_data_void_ptr;
          return object_array + index;
          break;
        }

        // COMPLETE ME

        default: 
        {
          assert(false && "given type is not handled by switch");
          return NULL;
        }
      }
    }
    else
      // m_data_void_ptr is a void pointer, that really points to a Game_object.
      return (Game_object *) m_data_void_ptr;
  }
```

You need to update your `Symbol::print()` so that it can print game objects. Use `Game_object::print()` (see `game_object.h`)

GPL allows an array of game objects, e.g., `rectandle rect[5];` (an array of five rectangle objects). Therefore, you need to update the `Symbol` class constructor for array declaration. Since arrays of game objects cannot be **initialized**, you don't have to worry about initializing every index positions of array of game objects.

#### Step 2

Add code to `gpl.y` so that you can create game objects that don't have any parameters (`rectangle my_rectangle();`)

There are two parts to creating game objects: 
1. create the object, and
2. set the parameter values specified.

This step, step 2, implements Part 1. After Step 2, you will be able to parse programs that contain game objects without any parameters. In Step 4, you will implement Part 2 and be able to handle object parameters.

It is easiest to set the parameter values if the object is created before the parameter values are parsed.  Consider the following rule:

```
object_declaration:
    object_type T_ID {here} T_LPAREN parameter_list_or_empty T_RPAREN
```

Bison allows you to specify an action anywhere on the right-hand side of a production.  In the above rule, the action (marked by `{here}`) is inserted in the middle of the right-hand side.  If you create the game object at the location marked by `{here}`, the object will have already been created when the parameter list is parsed.  Consider the following action fragment, i.e., actions for the first production rule for `object_declaration`:

```
object_type T_ID 
  {
    // create a new object and it's symbol
    // (Symbol() creates the new object);
    Symbol *symbol = new Symbol(*$2, $1);
  
    if (!symbol_table->insert(symbol))
    {
      Error::error(Error::PREVIOUSLY_DECLARED_VARIABLE, *$2);
    }

    // assign to global variable so the parameters can be inserted into
    // this object when each parameter is parsed
    cur_object_under_construction = symbol->get_game_object_value();
    cur_object_under_construction_name = symbol->get_name();
  }
  T_LPAREN parameter_list_or_empty T_RPAREN 
  {
    cur_object_under_construction = NULL;
    delete $2; // Scanner allocates memory for each T_ID string
  }
```

In the above action code for `object_declaration` rule, `$1` the object type (RECTANGLE, TRIANGLE, etc.) that is matched in the object_declaration rule (e.g., $$ = TRIANGLE), and `$2` is the name of object. `cur_object_under_construction` is a global variable that stores the object that is currently being created.  Add the following lines to your `gpl.y` definition section:

```
// Global variable to make the construction of object much less complex
// Only one object can ever be under construction at one time
Game_object *cur_object_under_construction = 0;
string cur_object_under_construction_name;
```

It can be used in Step 4 when the parameters are parsed.  Consider the following declaration:

```
rectangle rect(x = 42, y = 42);
```

The rectangle is constructed before the parameters are parsed.  A pointer to the object is placed in the global variable `cur_object_under_construction`.  When the parameter (for example: `x = 42`) is, the `x` field of `cur_object_under_construction` can be set to `42`.

There are two productions for creating game objects.  The first creates a single object, the second creates an array of objects.

```
object_declaration:
    object_type T_ID {here} T_LPAREN parameter_list_or_empty T_RPAREN
    |
    object_type T_ID T_LBRACKET expression T_RBRACKET
```

object_type will match one of the types of game objects (`T_RECTANGLE`, `T_TRIANGLE`, etc).   object_type should pass the object's type up the tree (e.g. `$$ = TRIANGLE`) just like simple_type.

The action for creating an array of game objects will be very similar to the code for creating an array of simple objects (int, double, string).  You get the type of the game object from $1, you evaluate the expression ($4->eval_int() ) to get the size of the array, and then you pass the type and the array size to the Symbol's constructor.  I use the same Symbol constructor that I use for simple objects; I added `CIRCLE`, `RECTANGLE`, `TRIANGLE`, `TEXTBOX`, `PIXMAP` to the switch statement inside this constructor so it can create arrays of these game objects. You must update your `Symbol(std::string name, Gpl_type type, int size);` constructor definition to handle creating objects with `game_object` types.

#### Step 3

Make sure you are compiling gpl.cpp with `-DGRAPHICS` in your Makefile; this should happen automatically if the directory is named "p6".  When you run your program with `t001.gpl` you should see a window with a game object in the lower left and the game objects should print in the symbol table (see `t001.gpl`, `t001.out`, and `t001.jpg`).

Starting in p6, in `gpl.cpp` reserved variables (such as `window_w` and `window_h`) are read from the symbol table.  For this to work, your `Symbol_table` and `Symbol` classes must provide the correct functions.  Consider the following code for reading the `window_x` in `gpl.cpp`:

```
    Symbol *symbol;
    int window_x;

    // look for window_x in the symbol table.  If found use it for the default window x
    symbol = symbol_table->lookup("window_x");
    if (symbol != NULL)
    {
      // if this variable does not have the type INT
      if (symbol->get_type() != INT)
        Error::error(Error::INVALID_TYPE_FOR_RESERVED_VARIABLE,
                     "window_x",
                     gpl_type_to_string(symbol->get_type()),
                     "int"
                    );
      // else this variable has the correct type, override the default
      else window_x = symbol->get_int_value();
    }
```

In order for this to work, your class Symbol_table must provide the lookup function `Symbol *Symbol_table::lookup(string target)` (return `NULL` if not found, return the `symbol *` if found) and your class Symbol must provide `Gpl_type Symbol::get_type()`, `int Symbol::get_int_value()`, `double Symbol::get_double_value()`, and `String Symbol::get_string_value()` functions.

I think it is important that you get test `t001` working before you go on.

#### Step 4

Add code to `gpl.y` to parse game object initialization parameters (e.g.,  `rectangle my_rectangle(x = 100, y = 100);`).  Don't handle animation_blocks yet.  Once step 4 is complete, your rectangle should be at a different position in the window, and the printout of the rectangle should show this `x` and `y` values (see `t002.gpl`, `t002.out`, and `t002.jpg`)

This is conceptually the hardest part of this phase.  However, it is not hard to implement once you get how it works.

Consider the following rule:

```
object_declaration:
	object_type T_ID {here} T_LPAREN parameter_list_or_empty T_RPAREN
```

It is easiest if the object is created before parameter_list_or_empty is matched by the parser.  If you create the object and put it in the symbol table in the `{here}` action (as is shown in step 2), then all you have to do is set a global variable in the `{here}` action to point to the game object you just created.  Once you do this, the parameter rules can use the global variable to insert parameters into the object. 

The following rule will match all the parameters (one at a time):

```
parameter:
	T_ID T_ASSIGN expression
```

The heart of the action for this rule is:

```
// Remember that cur_object_under_construction is a pointer to the actual game object.
// set_member_variable is a function that is a part of Game_object class.
Status status = cur_object_under_construction->set_member_variable(<member name>, <value>);
```

The global variable `cur_object_under_construction` was set in the `{here}` action.  The value is retrieved from calling eval on the expression (`$3`).   You will need code to handle all the possible different types for the values (`INT`, `DOUBLE`, `STRING`) [in step 8 you will add ANIMATION_BLOCK to this list]. You can start by adding the following code to the `paramter` rule.

```
parameter:
	T_ID T_ASSIGN expression
	{
		string parameter = *$1;
		delete $1; // Scanner allocates memory for each T_ID string
		
		Expression *value_expression = $3;
		Gpl_type value_expression_type = value_expression->get_type();
		
		// get type of the parameter T_ID of cur_object_under_construction
		Status status;
		Gpl_type parameter_type;
		status = cur_object_under_construction->get_member_variable_type(parameter, parameter_type);

		// COMPLETE ME
	}
	;	 
```

`Game_object` class contains functions for getting and setting member variables.  These functions are overloaded to handle all the possible types of member variables.

In addition to having a global variable for the current object under construction, you will also need a global variable for the name of the current object under construction for the following error messages:

```
Error::UNKNOWN_CONSTRUCTOR_PARAMETER
Error::INCORRECT_CONSTRUCTOR_PARAMETER_TYPE
```

These errors are issued in the parameter rule, requiring the name of the game object being constructed.  The only way you can get your hands on the name of the object being constructed is to use a global variable initialized in the `{here}` action.

#### Step 5

Update the symbol class so that it can handle animation blocks (i.e., update it so it can handle a pointer to class `Animation_block` using Symbol's `void *`).

The animation_block parameter of game objects is a pointer to an Animation_block.  In the final phase, this pointer will be used to execute the animation blocks.  Consider how the animation_block parameter is set:

```
forward animation bounce(rectangle cur_rectangle);

rectangle my_rectangle(animation_block = bounce);
```

The variable name bounce must be associated with a pointer to an animation block (Animation_block *).  To do this, we need to add a new type to Symbol.  (Hint: you will modify `Symbol::Symbol(string name, Gpl_type type)` constructor. You will not have an array of animation blocks.

There is a Gpl_type `ANIMATION_BLOCK`, which should be considered an `Animation_block *`.

Add the following function in the `symbol.cpp` and the function declaration in `symbol.h`:

```
Animation_block *Symbol::get_animation_block_value() const
  {
    validate_type_and_index(ANIMATION_BLOCK, UNDEFINED_INDEX);
  
    // arrays of Animation_blocks are not allowed
    assert(!is_array());
    // return &(*((Animation_block *) m_data_void_ptr));
    return (Animation_block *) m_data_void_ptr;
  }
```

The above function will return a single `Animation_block` object.

#### Step 6

When you parse a `forward` statement, two objects need to be put in the symbol table:  an `Animation_block` and a `Game_object`. 

The `Animation_block` should be a pointer to an "empty" animation block (i.e., in the Symbol's constructor:  `new Animation_block()`).  It should be inserted into the symbol table using the given name (`$3`):

```
forward animation bounce(rectangle cur_rectangle);
```

In the above example, the new animation block should be inserted into the symbol table using the name `bounce`

The parameter in the above forward statement (grammar symbol `animation_parameter`) is a rectangle and has the name `cur_rectangle`. A new rectangle (i.e., new Rectangle() ) should be created in Symbol's constructor and inserted into the symbol table using the name `cur_rectangle`.

You must also flag this new object as a parameter object by calling the following functions:

```
new_object->never_animate();
new_object->never_draw();
```

The Symbol is created in and the `never_animation()`/`never_draw()` functions are called in the action for animation_parameter.

In the last phase, you will parse animation blocks.  Then, you will look up the animation block in the symbol table and insert statements into it.  Consider this example:

`animation bounce(rectangle cur_rectangle)`

```
{
   // This example is from p7. Lookup the "bounce" Animation_block in the symbol table, insert an assignment statement into this block
   cur_rectangle.x = 42;  
}
```

When the statements in the animation block are parsed, the `cur_rectangle` object will be used.  That is why you put it in the symbol table when parsing the forward statement.

In the above example, the parameter `Rectangle cur_rectangle` must be associated with the Animation_block bounce.  The class Animation_block that I give you has member variables (`Symbol *m_parameter_symbol`, `String m_name`) to store the parameter pointer and name of the Animation_block (the Animation_block needs to know its name for printing) and a function (`Animation_block::initialize(Symbol *parameter_symbol, std::string name)`) to set these fields.  If you pass the pointer to the symbol you created in the action for animation_parameter up the tree using $$, you can get the pointer to the newly created Game_object out of the Symbol and then pass it and the name of the Animation_block to Animation_block::initialize() in the action for forward_declaration:

```
    // $3 is the name of the Animation_block, bounce in the above example.
    // $5 is the Symbol pointer passed up the tree from the animation_parameter action
    animation_block_just_created->initialize($5, *$3);  
```

#### Step 7

Update expressions and variables to handle pointers to animation blocks (need this for #7).

The grammar rule setting a parameter is:

```
parameter:
	T_ID T_ASSIGN expression
```

To handle the `animation_block` parameter, the Expression class must be able to handle a pointer for an `Animation_block`.  Add routines to Expression to handle variables of type `Animation_block`.  Parameter declarations are the only place where an Expression contains an `Animation_block`.  An Expression of type `Animation_block` cannot be anything but a single `Expression` node (technically a "tree" but only has one node).  This node is always a variable node.  So all you have to do is add `Expression::eval_animation_block()`. ~Make sure the variable knows about type `ANIMATION_BLOCK`, and make sure your `Expression` constructor that takes a pointer to a Variable can handle `ANIMATION_BLOCK` type.~
```
Animation_block * Expression::eval_animation_block()
{
   assert(m_type == ANIMATION_BLOCK);
   return m_variable->get_animation_block_value();
}
```

#### Step 8:
Step 8: Add code to your game object parameters (#4 above) so that you can initialize Animation_blocks (they won't do anything yet, just show up in the symbol table) (see `t003.gpl`, `t003.out`, and `t003.jpg`).  For example:

```
rectangle rect(animation_block = bounce);  // write code to handle parameters of type ANIMATION_BLOCK
```

If you do step 7 and augment Expression so it handles Animation_blocks, this step should be trivial.  Add code to the parameter action that can deal with type `ANIMATION_BLOCK`.  It will look just like the code that deals with `INT`, `DOUBLE`, and `STRING`.

#### Step 9:

Add member variables to class `Variable` and add code to `gpl.y` to parse member variables  (see `t004.gpl`, `t004.out`, and `t004.jpg`).  You should not have to change Expression.

For this step, you need to add an action to the variable productions:

```
variable:	
	| T_ID T_PERIOD T_ID
```

This will require a Variable constructor (or you can use an existing constructor if you use default parameter values).

The `Variable` get functions will also have to be modified to handle this type of variable. For example, look at the `Variable::get_int_value()` function below:

```

int Variable::get_int_value() const
{
  if (!m_field)
  {
    if (!m_expression)
      return m_symbol->get_int_value();
    else
      // without error checking we have
      // return m_symbol->get_int_value(m_expression->eval_int()); 
      return m_symbol->get_int_value(eval_index_with_error_checking());
  }
  else // this Variable has a field must be of form rect.x or rects[k].x
  {
      Game_object *cur_game_object;
      if (!m_expression) // of the form rect.x
        cur_game_object = m_symbol->get_game_object_value();

      else // of the form rects[k].x
        cur_game_object=m_symbol->get_game_object_value(eval_index_with_error_checking());
      int value;
      Status status = cur_game_object->get_member_variable(*m_field,value);
      assert(status == OK);
      return value;
  }
}
```

To start, add the following member variables and functions to the `Variable` class.

```
Game_object *get_game_object_value() const;
Animation_block *get_animation_block_value() const;

// if this Variable is, for example, circles[i].animation_block then it's type is ANIMATION_BLOCK
// and it's base_game_object_type is circle.
Gpl_type get_base_game_object_type() const;

bool is_game_object() const {return m_type & GAME_OBJECT;}
bool is_animation_block() const {return m_type == ANIMATION_BLOCK;}
bool is_non_member_animation_block() const;

std::string *m_field = NULL;
```

#### Step 10:

Add array member variables to class Variable and add code to gpl.y to parse array member variables (see t005.gpl, t005.out, and t005.jpg).

This step is similar to step 9.  You will need to an an action for the production:

```
variable:	
	T_ID T_LBRACKET expression T_RBRACKET T_PERIOD T_ID
```

This step will also require a new constructor and for the Variable get functions to be modified.

You will need to use Game_object::get_member_variable_type to set the Variable's type.

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
