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
  if (lookup(symbol->get_name()) == NULL) {
    m_symbols.insert({symbol->get_name(), symbol});

    return true;
  }

  return false;
}

Symbol *Symbol_table::lookup(string name) const
{
  auto find = m_symbols.find(name);

  if (find != m_symbols.end()) {
    return find->second;
  }

  return NULL;
}

// comparison function for the STL sort algorithm
bool compare_symbols(Symbol *a, Symbol *b) 
{
  return a->get_name() < b->get_name() ? true : false;
}


void Symbol_table::print(ostream &os) const
{
  vector<Symbol *> symbols;

  for (const auto& map_pair: m_symbols) {
    symbols.push_back(map_pair.second);
  }

  sort(symbols.begin(), symbols.end(), compare_symbols);

  for (const auto& symbol: symbols) {
    symbol->print(os);
  }
}
