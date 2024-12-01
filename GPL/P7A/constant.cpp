#include "constant.h"

int Constant::get_int_value() 
{
  assert(m_type == INT); 
  return m_union_value.union_int;
}

double Constant::get_double_value() 
{
  assert(m_type == DOUBLE);
  return m_union_value.union_double;
}

std::string Constant::get_string_value() 
{
  assert(m_type == STRING);
  return *m_union_value.union_string;
}
