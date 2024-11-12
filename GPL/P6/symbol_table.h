#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <iostream>
#include <string>
#include <unordered_map>

#include "gpl_type.h"

class Symbol;

class Symbol_table
{
  public:
    static Symbol_table *instance();
    bool insert(Symbol *symbol);
    Symbol *lookup(std::string name) const;
    void print(std::ostream &os) const;
    ~Symbol_table();

  private:
    Symbol_table();
    Symbol_table(const Symbol_table&);  // disable default copy constructor
    // disable default assignment
    const Symbol_table &operator=(const Symbol_table&);

    static Symbol_table *m_instance;
    std::unordered_map<std::string, Symbol*> m_symbols;
};

#endif // #ifndef SYMBOL_TABLE_H
