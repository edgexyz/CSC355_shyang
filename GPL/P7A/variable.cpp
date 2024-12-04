#include "variable.h"
#include "symbol.h"
#include "expression.h"
#include "gpl_type.h"
#include "gpl_assert.h"
#include "game_object.h"
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

Variable::Variable(Symbol *symbol, string *field)
{
  m_symbol = symbol;
  m_field = field;
  Status status = symbol->get_game_object_value()->get_member_variable_type(*field, m_type);
  assert(status == OK);
}

Variable::Variable(Symbol *symbol, Expression *expression, string *field)
{
  m_symbol = symbol;
  m_expression = expression;
  m_field = field;
  Status status = symbol->get_game_object_value(0)->get_member_variable_type(*field, m_type);
  assert(status == OK);
}

string Variable::get_name() const
{
  string name = m_symbol->get_name();
  // Add [] at the end of name string to indicate the variable is an array.
  if (m_expression)
  {
    name += "[]";
  }

  if (m_field)
  {
    name += "." + *m_field;
  }

  return name;
}

int Variable::get_int_value() const
{
  if (m_field)
  {
    Game_object *cur_game_object;

    if (m_expression)
      cur_game_object = m_symbol->get_game_object_value(eval_index_with_error_checking());
    else
      cur_game_object = m_symbol->get_game_object_value();

    int value;
    Status status = cur_game_object->get_member_variable(*m_field, value);

    assert(status == OK);
    return value;
  }
  else
  {
    if (m_expression)
      return m_symbol->get_int_value(eval_index_with_error_checking());
    else
      return m_symbol->get_int_value();
  }
}

double Variable::get_double_value() const
{
  if (m_field)
  {
    Game_object *cur_game_object;

    if (m_expression)
      cur_game_object = m_symbol->get_game_object_value(eval_index_with_error_checking());
    else
      cur_game_object = m_symbol->get_game_object_value();

    double value;
    Status status = cur_game_object->get_member_variable(*m_field, value);

    assert(status == OK);
    return value;
  }
  else
  {
    if (m_expression)
      return m_symbol->get_double_value(eval_index_with_error_checking());
    else
      return m_symbol->get_double_value();
  }
}

string Variable::get_string_value() const
{
  if (m_field)
  {
    Game_object *cur_game_object;

    if (m_expression)
      cur_game_object = m_symbol->get_game_object_value(eval_index_with_error_checking());
    else
      cur_game_object = m_symbol->get_game_object_value();

    string value;
    Status status = cur_game_object->get_member_variable(*m_field, value);

    assert(status == OK);
    return value;
  }
  else
  {
    if (m_expression)
      return m_symbol->get_string_value(eval_index_with_error_checking());
    else
      return m_symbol->get_string_value();
  }
}

Game_object* Variable::get_game_object_value() const
{
  assert(m_type == GAME_OBJECT);

  if (m_expression)
    return m_symbol->get_game_object_value(eval_index_with_error_checking());
  else
    return m_symbol->get_game_object_value();
}

Animation_block* Variable::get_animation_block_value() const
{
  assert(m_type == ANIMATION_BLOCK);
  if (m_field)
  {
    Game_object *cur_game_object;

    cur_game_object = m_symbol->get_game_object_value();

    Animation_block *value;
    Status status = cur_game_object->get_member_variable(*m_field, value);

    assert(status == OK);
    return value;
  }
  else
  {
    return m_symbol->get_animation_block_value();
  }
}

Gpl_type Variable::get_base_game_object_type() const
{
  return m_symbol->get_base_type();
}

void Variable::set(int value)
{
  if (m_field)
  {
    Game_object *cur_game_object;

    if (m_expression)
      cur_game_object = m_symbol->get_game_object_value(eval_index_with_error_checking());
    else
      cur_game_object = m_symbol->get_game_object_value();

    Status status = cur_game_object->set_member_variable(*m_field, value);

    assert(status == OK);
  }
  else
  {
    if (m_expression)
    {
      int index = eval_index_with_error_checking();
      m_symbol->set(value, index);
    }
    else
    {
      m_symbol->set(value);
    }
  }
}

void Variable::set(double value)
{
  if (m_field)
  {
    Game_object *cur_game_object;

    if (m_expression)
      cur_game_object = m_symbol->get_game_object_value(eval_index_with_error_checking());
    else
      cur_game_object = m_symbol->get_game_object_value();

    Status status = cur_game_object->set_member_variable(*m_field, value);

    assert(status == OK);
  }
  else {
    if (m_expression)
    {
      int index = eval_index_with_error_checking();
      m_symbol->set(value, index);
    }
    else
    {
      m_symbol->set(value);
    }
  }
}

void Variable::set(string value)
{
  if (m_field)
  {
    Game_object *cur_game_object;

    if (m_expression)
      cur_game_object = m_symbol->get_game_object_value(eval_index_with_error_checking());
    else
      cur_game_object = m_symbol->get_game_object_value();

    Status status = cur_game_object->set_member_variable(*m_field, value);

    assert(status == OK);
  }
  else
  {
    if (m_expression)
    {
      int index = eval_index_with_error_checking();
      m_symbol->set(value, index);
    }
    else
    {
      m_symbol->set(value);
    }
  }
}

void Variable::set(Animation_block* value)
{
  assert(!m_expression); // should only be called if not an array
  m_symbol->set(value);
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
