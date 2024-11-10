#include "symbol.h"
#include "gpl_assert.h"

#include <string>

using namespace std;

Symbol::Symbol(string name, int initial_value)
{
  m_name = name;
  m_type = INT;
  m_data_void_ptr = (void *) new int(initial_value);
  m_size = UNDEFINED_SIZE;
  validate();
}

Symbol::Symbol(string name, double initial_value)
{
  m_name = name;
  m_type = DOUBLE;
  m_data_void_ptr = (void *) new double(initial_value);
  m_size = UNDEFINED_SIZE;
  validate();
}

Symbol::Symbol(string name, string initial_value)
{
  m_name = name;
  m_type = STRING;
  m_data_void_ptr = (void *) new string(initial_value);
  m_size = UNDEFINED_SIZE;
  validate();
}


Symbol::Symbol(string name, Gpl_type type, int size)
{
  m_name = name;
  m_type = (Gpl_type) (ARRAY + type);
  
  switch (type)
  {
    case INT:
    {
      int *arr = new int[size];
      for (int i=0; i<size; i++) {
        arr[i] = 0;
      }
      m_data_void_ptr = (void *)arr;
      break;
    }

    case DOUBLE:
    {
      double *arr = new double[size];
      for (int i=0; i<size; i++) {
        arr[i] = 0.0;
      }
      m_data_void_ptr = (void *)arr;
      break;
    }

    case STRING:
    {
      string *arr = new string[size];
      for (int i=0; i<size; i++) {
        arr[i] = "";
      }
      m_data_void_ptr = (void *)arr;
      break;
    }
    
    default: assert(0);
  }

  m_size = size;
}

Symbol::~Symbol()
{
  // The Symbol "owns" the object it contains, it must delete it
  if (!is_array())
    switch (m_type)
    {
      case INT: delete (int *) m_data_void_ptr; break;
      case DOUBLE: delete (double *) m_data_void_ptr; break;
      case STRING: delete (string *) m_data_void_ptr; break;
      default: assert(0);
    }
  else
    switch (m_type)
    {
      case INT_ARRAY: delete [] (int *) m_data_void_ptr; break;
      case DOUBLE_ARRAY: delete [] (double *) m_data_void_ptr; break;
      case STRING_ARRAY: delete [] (string *) m_data_void_ptr; break;
      default: assert(0);
    }
}

// strip away the ARRAY bit from the type if there is one
Gpl_type Symbol::get_base_type() const 
{

  if (m_type & ARRAY)
      return (Gpl_type) (m_type - ARRAY);
  else
      return m_type;
}

void Symbol::validate_type_and_index(Gpl_type type, int index) const
{
  assert(m_type & type);

  assert((index == UNDEFINED_INDEX && m_size == UNDEFINED_SIZE) 
         || (index >= 0 && m_size >= 1 && index < m_size));
}

int Symbol::get_int_value(int index /* = UNDEFINED_INDEX */) const
{
  validate_type_and_index(INT, index);
  if (is_array())
    return ((int *) m_data_void_ptr)[index];
  else
    return *((int *) m_data_void_ptr);
}

double Symbol::get_double_value(int index /* = UNDEFINED_INDEX */) const
{
  validate_type_and_index(DOUBLE, index);
  if (is_array())
    return ((double *) m_data_void_ptr)[index];
  else
    return *((double *) m_data_void_ptr);
}

string Symbol::get_string_value(int index /* = UNDEFINED_INDEX */) const
{
  validate_type_and_index(STRING, index);
  if (is_array())
    return ((string *) m_data_void_ptr)[index];
  else
    return *((string *) m_data_void_ptr);
}

void Symbol::set(int value, int index /* = UNDEFINED_INDEX */)
{
  validate_type_and_index(INT, index);
  if (is_array())
    ((int *)m_data_void_ptr)[index] = value;
  else
    *(int *)m_data_void_ptr = value;
}

void Symbol::set(double value, int index /* = UNDEFINED_INDEX */)
{
  validate_type_and_index(DOUBLE, index);
  if (is_array())
    ((double *)m_data_void_ptr)[index] = value;
  else
    *(double *)m_data_void_ptr = value;
}

void Symbol::set(string value, int index /* = UNDEFINED_INDEX */)
{
  validate_type_and_index(STRING, index);
  if (is_array())
    ((string *)m_data_void_ptr)[index] = value;
  else
    *(string *)m_data_void_ptr = value;
}

void Symbol::print(ostream &os) const
{
  if (is_array())
  {
    for(int i=0; i<m_size; i++) {
      os << gpl_type_to_base_string(m_type) << " " << m_name << "[" << i << "] = ";

      switch (get_base_type())
      {
        case INT: os << get_int_value(i); break;
        case DOUBLE: os << get_double_value(i); break;
        case STRING: os << "\"" << get_string_value(i) << "\""; break;
        default: os << m_data_void_ptr;
      }

      os << endl;
    }
  }
  else
  {
    os << gpl_type_to_base_string(m_type) << " " << m_name << " = ";

    switch (m_type)
    {
      case INT: os << get_int_value(); break;
      case DOUBLE: os << get_double_value(); break;
      case STRING: os << "\"" << get_string_value() << "\""; break;
      default: os << m_data_void_ptr;
    }

    os << endl;
  }
}

void Symbol::validate() const
{
  if (m_type & ARRAY)
    assert(m_size > 0);
  else
    assert(m_size == UNDEFINED_SIZE);

  assert(m_data_void_ptr != NULL);
  assert(m_name != "");
  assert(m_size != 0);
}
