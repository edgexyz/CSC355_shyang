// updated 2/14/2016

/*
  gpl.cpp provides the main() function for the gpl interpreter

  It is designed to work for phases p3 - p8 of the assignment.

  It is customized by using the C preprocessor to turn on and off
  segments of code.  The symbols used are:

    SYMBOL_TABLE            assume that the symbol table exists
    GRAPHICS                open a window and show graphics
    __APPLE__               compiled for OSX

  Symbols can be defined by using the -D argument to the compiler.  For
  example the following command:

    g++ -DSYMBOL_TABLE gpl.cpp

  will define the C preprocessor variable SYMBOL_TABLE before it
  start compilation.

  Edit the Makefile so it uses the correct gpl.cpp flags for the given phase.

*/

#ifdef SYMBOL_TABLE
#include "symbol_table.h"
#include "symbol.h"
#endif

#include "parser.h" // substitute for y.tab.h
#include "error.h"

#ifdef GRAPHICS
#include "window.h"
#endif
#include "gpl_assert.h"

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <time.h> // for time()
#include <stdio.h> // for fopen()

#if defined P7B
  #include "event_manager.h"
#endif
using namespace std;

extern int yylex();
extern int yyparse();

const int DEFAULT_WINDOW_X = 200;
const int DEFAULT_WINDOW_Y = 200;
const int DEFAULT_WINDOW_WIDTH = 500;
const int DEFAULT_WINDOW_HEIGHT = 500;
const double DEFAULT_WINDOW_RED = 1.0;
const double DEFAULT_WINDOW_GREEN = 1.0;
const double DEFAULT_WINDOW_BLUE = 1.0;
const string DEFAULT_WINDOW_TITLE = "gpl window";
const int DEFAULT_ANIMATION_SPEED = 88;

class Window;
Window *window = NULL;

// there does not appear to be a default yywrap in OSX
#ifdef __APPLE__
extern "C" int yywrap ( );
int
yywrap(void)
{
  return true;
}
#endif


int
yyerror(const char *str)
{
  Error::error(Error::PARSE_ERROR, str);
  return 1;
}

#ifdef SYMBOL_TABLE
static Symbol_table *symbol_table = Symbol_table::instance();
#endif

void illegal_usage(const char *qualifier = NULL)
{
  cerr << "illegal command line argument(s)" << endl
       << "Usage:  $ gpl [-s seed] [-stdin] [-dump_pixels filename] filename[.gpl]" << endl;

  if (qualifier)
      cerr << qualifier << endl;

  exit(1);

}

bool dump_pixels = false;
char *dump_pixels_filename = 0;
bool graphics_flag = false;

// This function is called from window.cpp when the user quits the program
void user_quit_program()
{
  #if defined P7B
    cout << endl << "User quit program.  Executing termination blocks" << endl;
    Event_manager::instance()->execute_handlers(Window::TERMINATE);
  #elif defined P6 || P7A
    cout << endl << "User quit program. Printing the symbol table." << endl;
    static Symbol_table *symbol_table = Symbol_table::instance();
    symbol_table->print(cout);
  #endif

  #ifdef GRAPHICS
    if (dump_pixels)
    {
      assert(graphics_flag);
      assert(window);
    
      window->dump_pixels(dump_pixels_filename);
    }
  #endif
}

int main(int argc, char **argv)
{
  bool symbol_table_flag = false;

  // set local flags based on the command line argument when compiling gpl.cpp
  // (see comments in Makefile)
  #ifdef SYMBOL_TABLE
    symbol_table_flag = true;
  #endif

  #ifdef GRAPHICS
    graphics_flag = true;
  #endif


  extern FILE *yyin;
  char *filename = 0;
  int seed = time(0);
  bool read_keypresses_from_standard_input = false;

  // I use to use getopt() to parse the command line arguments
  // BUT it was not working on OS-X, so I wrote the following hack

  // if any argument is -stdin, set the flag
  // if any argument is -s, the next one must be a number
  //    if it is a number use it as the srand seed
  // if any argument is -dump_pixels, the next one must be the filename
  // any other argument is assumed to be the filename

  for (int i = 1; i < argc; i++)
  {
    if (!strcmp(argv[i], "-stdin"))
      read_keypresses_from_standard_input = true;
    else if (!strcmp(argv[i], "-s"))
    {
      if (i+1 >= argc)
        illegal_usage();
      // make sure the argument after the -s is a number
      for (char *c = argv[i+1]; *c; c++)
      {
        if (!isdigit(*c))
        {
          cerr << "Illegal random number generator seed: "
               << argv[i+1]
               << endl;
          exit(1);
        }
      }
      seed = atoi(argv[i+1]);
      i += 1; // skip the seed value
    }
    else if (!strcmp(argv[i], "-dump_pixels"))
    {
      if (!graphics_flag)
        illegal_usage("Cannot dump the window using -dump_pixels unless graphics are enabled.");

      if (i+1 >= argc)
        illegal_usage();
      dump_pixels_filename = argv[i+1];
      dump_pixels = true;
      i += 1; // skip the dump filename
    }
    else
    {
      // can only specify one filename
      if (filename != 0)
        illegal_usage();

      filename = argv[i];
    }

  }
  if (!filename)
    illegal_usage();

  char *filename_with_extension = new char[strlen(filename) + 4];
  strcpy(filename_with_extension, filename);

  yyin = fopen(filename,"r");

  // if open failed, append .gpl to the filename and try again
  if (!yyin)
  {
    strcat(filename_with_extension, ".gpl");
    yyin = fopen(filename_with_extension,"r");
  }

  // cannot open filename or filename+.gpl
  if (!yyin)
  {
    cerr << "Cannot open input file <" << filename << ">." << endl;
    exit(1);
  }

  srand(seed);

  cout << "gpl.cpp::main()" << endl
       << "  input file(" << filename_with_extension << ")" << endl
       << "  random seed(" << seed << ")" << endl
       << "  read_keypresses_from_standard_input" << "("
       << (read_keypresses_from_standard_input ? "true" : "false") << ")" << endl;

  if (dump_pixels)
    cout << "  dump_pixels(true, file = "  << dump_pixels_filename << ")"<< endl;
  else
    cout << "  dump_pixels(false)" << endl;

  cout << "  symbol_table("
       << (symbol_table_flag ? "true" : "false") << ")" << endl
       << "  graphics("
       << (graphics_flag ? "true" : "false") << ")" << endl << endl;

  delete [] filename_with_extension;

  cout << "gpl.cpp::main() Calling yyparse()" << endl << endl;

  int parse_result = yyparse();

  cout << endl << "gpl.cpp::main() after call to yyparse()."<<endl<< endl;

  // tell the Error object that execution is starting
  // Error class prints different messages once execution starts
  Error::starting_execution();
  

  // if -DGRAPHICS was specified when compiling gpl.cpp then include this code
  #ifdef GRAPHICS
    int window_x = DEFAULT_WINDOW_X;
    int window_y = DEFAULT_WINDOW_Y;
    int window_width = DEFAULT_WINDOW_WIDTH;
    int window_height = DEFAULT_WINDOW_HEIGHT;
    double window_red = DEFAULT_WINDOW_RED;
    double window_green = DEFAULT_WINDOW_GREEN;
    double window_blue = DEFAULT_WINDOW_BLUE;
    string window_title = DEFAULT_WINDOW_TITLE;
    int animation_speed = DEFAULT_ANIMATION_SPEED;
  
    // If any of the reserved variables are in the symbol table,
    // that means user wants to override the default values set above.
    // However, the variables must be of the correct type
    //    e.g. if there is a "window_x" in the symbol table AND it of type INT
    //         use its value instead of the default value

    Symbol *symbol;
  
    // look for each of the system variables in the symbol table
    // if they are found, use the value in the symbol table
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
    symbol = symbol_table->lookup("window_y");
    if (symbol != NULL)
    {
      if (symbol->get_type() != INT)
        Error::error(Error::INVALID_TYPE_FOR_RESERVED_VARIABLE,
                     "window_y",
                     gpl_type_to_string(symbol->get_type()),
                     "int"
                    );
      else window_y = symbol->get_int_value();
    }
    symbol = symbol_table->lookup("window_width");
    if (symbol != NULL)
    {
      if (symbol->get_type() != INT)
        Error::error(Error::INVALID_TYPE_FOR_RESERVED_VARIABLE,
                     "window_width",
                     gpl_type_to_string(symbol->get_type()),
                     "int"
                    );
      else window_width = symbol->get_int_value();
    }
    symbol = symbol_table->lookup("window_height");
    if (symbol != NULL)
    {
      if (symbol->get_type() != INT)
        Error::error(Error::INVALID_TYPE_FOR_RESERVED_VARIABLE,
                     "window_height",
                     gpl_type_to_string(symbol->get_type()),
                     "int"
                    );
      else window_height = symbol->get_int_value();
    }
    symbol = symbol_table->lookup("window_red");
    if (symbol != NULL)
    {
      if (symbol->get_type() != DOUBLE)
        Error::error(Error::INVALID_TYPE_FOR_RESERVED_VARIABLE,
                     "window_red",
                     gpl_type_to_string(symbol->get_type()),
                     "double"
                    );
      else window_red = symbol->get_double_value();
    }
    symbol = symbol_table->lookup("window_green");
    if (symbol != NULL)
    {
      if (symbol->get_type() != DOUBLE)
        Error::error(Error::INVALID_TYPE_FOR_RESERVED_VARIABLE,
                     "window_green",
                     gpl_type_to_string(symbol->get_type()),
                     "double"
                    );
      else window_green = symbol->get_double_value();
    }
    symbol = symbol_table->lookup("window_blue");
    if (symbol != NULL)
    {
      if (symbol->get_type() != DOUBLE)
        Error::error(Error::INVALID_TYPE_FOR_RESERVED_VARIABLE,
                     "window_blue",
                     gpl_type_to_string(symbol->get_type()),
                     "double"
                    );
      else window_blue = symbol->get_double_value();
    }
    symbol = symbol_table->lookup("window_title");
    if (symbol != NULL)
    {
      if (symbol->get_type() != STRING)
        Error::error(Error::INVALID_TYPE_FOR_RESERVED_VARIABLE,
                     "window_title",
                     gpl_type_to_string(symbol->get_type()),
                     "string"
                    );
      else window_title = symbol->get_string_value();
    }
    symbol = symbol_table->lookup("animation_speed");
    if (symbol != NULL)
    {
      if (symbol->get_type() != INT)
        Error::error(Error::INVALID_TYPE_FOR_RESERVED_VARIABLE,
                     "animation_speed",
                     gpl_type_to_string(symbol->get_type()),
                     "int"
                    );
      else animation_speed = symbol->get_int_value();
    }
  #endif


  if (parse_result != 0 || Error::num_errors() != 0)
  {
    // This is sent to cout instead of cerr so it
    // ends up in the .out files
    // It makes it easier to understand what happened when
    // reading the .out files
    cout << Error::num_errors() << " error";
    if (Error::num_errors() > 1)
      cout << "s";
    cout << " found."
         << endl
         << "gpl giving up."
         << endl;

    exit(1);
  }

  #ifndef GRAPHICS
  else
  {
    cout << "No errors found (parser probably worked correctly)."
         << endl
         << endl;
  }
  #endif

  #ifdef SYMBOL_TABLE
    if (parse_result == 0 && Error::num_errors() == 0)
    {
      cout << endl << "Printing the symbol table from main()" << endl;
      static Symbol_table *symbol_table = Symbol_table::instance();
      symbol_table->print(cout);
    }
  #endif

  #ifndef GRAPHICS
    cout << "Graphics is turned off by the Makefile.  Program exiting."
         << endl << endl;
    cout << "gpl.cpp::main() done." << endl;
  #else
    window = new Window(window_x, window_y, window_width,
                        window_height, window_title, animation_speed,
                        window_red, window_green, window_blue,
                        read_keypresses_from_standard_input
                       );
  
    cout << "gpl.cpp::main() Calling window->initialize()." << endl << endl;
    window->initialize();
  
    cout << endl << "gpl.cpp::main() Passing control to window->main_loop()."
         << endl;
  
    // Tell glut to start the main event loop
    window->main_loop();

  #endif
}
