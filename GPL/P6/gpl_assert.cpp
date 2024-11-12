// updated 2/14/2016

#include <iostream>
#include <stdlib.h>
using namespace std;

extern int line_count;  // from gpl.l

void __gpl_assert(const char *filename, int line, const char *text)
{
  // When your program fails an assert, put a breakpoint in the debugger
  // $ gdb gpl
  // ...
  // (gdb) break gpl_assert.cpp:22
  // (gdb) run tests/t01.gpl
  //
  // when it stops you can see the run time stack using where
  // (gdb) where
  //  and move up/down the stack printing variables
  // (gdb) up 2
  // (gdb) p m_expression
  cerr << "assertion \"" << text << "\" failed: file \""
       << filename << "\", line " << line
       << ".  Input line " << line_count << "."
       << endl;

  exit(1);
}
