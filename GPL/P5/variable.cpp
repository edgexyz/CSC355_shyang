#include "variable.h"
#include "symbol.h"
#include "expression.h"
#include "gpl_type.h"
#include "gpl_assert.h"
#include "error.h"
#include <sstream>
using namespace std;

Variable::Variable(Symbol *symbol)
{
  m_symbol = symbol;
  m_type = symbol->get_type();
}

Variable::Variable(Symbol *symbol, Expression *expression)
{
  m_symbol = symbol;
  m_expression = expression;
  // m_type = symbol->get_type();
  m_type = symbol->get_base_type();
}

string Variable::get_name() const
{
  string name = m_symbol->get_name();
  // Add [] at the end of name string to indicate the variable is an array.
  if (m_expression)
  {
    name += "[]";
  }
  return name;
}

int Variable::get_int_value() const
{
  if (m_expression)
  {
    int index = m_expression->eval_int();

    if (index >= 0 && index < m_symbol->size())
    {
      return m_symbol->get_int_value(index);
    }
    else
    {
      Error::error(Error::ARRAY_INDEX_OUT_OF_BOUNDS, m_symbol->get_name(), to_string(index));
      return m_symbol->get_int_value(0);
    }
  }
  else
  {
    return m_symbol->get_int_value();
  }
}

double Variable::get_double_value() const
{
  if (m_expression)
  {
    int index = m_expression->eval_int();

    if (index >= 0 && index < m_symbol->size())
    {
      return m_symbol->get_double_value(index);
    }
    else
    {
      Error::error(Error::ARRAY_INDEX_OUT_OF_BOUNDS, m_symbol->get_name(), to_string(index));
      return m_symbol->get_double_value(0);
    }
  }
  else
  {
    return m_symbol->get_double_value();
  }
}

string Variable::get_string_value() const
{
  if (m_expression)
  {
    int index = m_expression->eval_int();

    if (index >= 0 && index < m_symbol->size())
    {
      return m_symbol->get_string_value(index);
    }
    else
    {
      Error::error(Error::ARRAY_INDEX_OUT_OF_BOUNDS, m_symbol->get_name(), to_string(index));
      return m_symbol->get_string_value(0);
    }
  }
  else
  {
    return m_symbol->get_string_value();
  }
}

void Variable::set(int value)
{
  if (m_expression)
  {
    int index = m_expression->eval_int();

    if (index >= 0 && index < m_symbol->size())
    {
      m_symbol->set(value, index);
    }
    else
    {
      Error::error(Error::ARRAY_INDEX_OUT_OF_BOUNDS, m_symbol->get_name(), to_string(index));
    }
  }
  else
  {
    m_symbol->set(value);
  }
}

void Variable::set(double value)
{
  if (m_expression)
  {
    int index = m_expression->eval_int();

    if (index >= 0 && index < m_symbol->size())
    {
      m_symbol->set(value, index);
    }
    else
    {
      Error::error(Error::ARRAY_INDEX_OUT_OF_BOUNDS, m_symbol->get_name(), to_string(index));
    }
  }
  else
  {
    m_symbol->set(value);
  }
}

void Variable::set(string value)
{
  if (m_expression)
  {
    int index = m_expression->eval_int();

    if (index >= 0 && index < m_symbol->size())
    {
      m_symbol->set(value, index);
    }
    else
    {
      Error::error(Error::ARRAY_INDEX_OUT_OF_BOUNDS, m_symbol->get_name(), to_string(index));
    }
  }
  else
  {
    m_symbol->set(value);
  }
}

// Evaluate expression if there is one, return index if index is out of bounds, 
// issue error, return 0 (0 is always in bounds)
int Variable::eval_index_with_error_checking() const
{
  assert(m_expression); // should only be called if this is an array

  int index = m_expression->eval_int();

  // an annoying special case
  // if the index is -1 it confuses symbol that uses -1 for 
  //   not an array
  if (m_symbol->index_within_range(index))
  {
    return index;
  }
  else
  {
    Error::error(Error::ARRAY_INDEX_OUT_OF_BOUNDS,
                 m_symbol->get_name(),to_string(index));

    return 0; // 0 is always within range
  }
}
