#include "symbol_table.h"
#include "symbol.h"
#include "gpl_assert.h"
using namespace std;

#include <vector>
#include <algorithm> // for sort algorithm

/* static */ Symbol_table *Symbol_table::m_instance = 0;

/* static */ Symbol_table * Symbol_table::instance()
{
  if (!m_instance)
    m_instance = new Symbol_table();
  return m_instance;
}

Symbol_table::Symbol_table(){}

Symbol_table::~Symbol_table()
{
  cerr << "~Symbol_table()... not implemented..." << endl;
}


bool Symbol_table::insert(Symbol *symbol)
{
    // COMPLETE ME
}

Symbol *Symbol_table::lookup(string name) const
{
    // COMPLETE ME
}

// comparison function for the STL sort algorithm
bool compare_symbols(Symbol *a, Symbol *b) 
{
    // COMPLETE ME
}


void Symbol_table::print(ostream &os) const
{
  // COMPLETE ME

  // HINT: What I would do here is ...
  // (1) add all symbols in the table into a vector.
  // (2) sort the vector.
  // (3) print the elements (symbols) in the vector.
}
