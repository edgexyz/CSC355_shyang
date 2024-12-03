#include "symbol.h"
#include "game_object.h"
#include "triangle.h"
#include "pixmap.h"
#include "circle.h"
#include "rectangle.h"
#include "textbox.h"
#include "animation_block.h"

#include <string>

using namespace std;

Symbol::Symbol(string name, int initial_value)
{
  m_name = name;
  m_type = INT;
  m_size = UNDEFINED_SIZE;
  m_data_void_ptr = (void *) new int(initial_value);
  validate();
}

Symbol::Symbol(string name, double initial_value)
{
  m_name = name;
  m_type = DOUBLE;
  m_size = UNDEFINED_SIZE;
  m_data_void_ptr = (void *) new double(initial_value);
  validate();
}

Symbol::Symbol(string name, string initial_value)
{
  m_name = name;
  m_type = STRING;
  m_size = UNDEFINED_SIZE;
  m_data_void_ptr = (void *) new string(initial_value);
  validate();
}

Symbol::Symbol(string name, Gpl_type type)
{
  assert(type == CIRCLE ||
         type == RECTANGLE ||
         type == TRIANGLE ||
         type == TEXTBOX ||
         type == PIXMAP ||
         type == ANIMATION_BLOCK
        );
  
  m_name = name;
  m_type = type;
  m_size = UNDEFINED_SIZE;

  switch (type)
  {
    case CIRCLE: m_data_void_ptr = (void *) new Circle(); break;
    case RECTANGLE: m_data_void_ptr = (void *) new Rectangle(); break;
    case TRIANGLE: m_data_void_ptr = (void *) new Triangle(); break;
    case TEXTBOX: m_data_void_ptr = (void *) new Textbox(); break;
    case PIXMAP: m_data_void_ptr = (void *) new Pixmap(); break;
    case ANIMATION_BLOCK: m_data_void_ptr = (void *) new Animation_block(); break;
    default: assert(0);
  }
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

    case CIRCLE:
    {
      Circle *arr = new Circle[size];
      m_data_void_ptr = (void *)arr;
      break;
    }

    case RECTANGLE:
    {
      Rectangle *arr = new Rectangle[size];
      m_data_void_ptr = (void *)arr;
      break;
    }

    case TRIANGLE:
    {
      Triangle *arr = new Triangle[size];
      m_data_void_ptr = (void *)arr;
      break;
    }

    case TEXTBOX:
    {
      Textbox *arr = new Textbox[size];
      m_data_void_ptr = (void *)arr;
      break;
    }

    case PIXMAP:
    {
      Pixmap *arr = new Pixmap[size];
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

Game_object* Symbol::get_game_object_value(int index /* = UNDEFINED_INDEX */) const
{
  validate_type_and_index(GAME_OBJECT, index);

  if (is_array())
  {
    switch (m_type)
    {
      case CIRCLE_ARRAY: return (Circle *) m_data_void_ptr + index; break;
      case RECTANGLE_ARRAY: return (Rectangle *) m_data_void_ptr + index; break;
      case TRIANGLE_ARRAY: return (Triangle *) m_data_void_ptr + index; break;
      case TEXTBOX_ARRAY: return (Textbox *) m_data_void_ptr + index; break;
      case PIXMAP_ARRAY: return (Pixmap *) m_data_void_ptr + index; break;
      default:
      {
        assert(false && "given type is not handled by switch");
        return NULL;
      }
    }
  }
  else
  {
    return (Game_object *) m_data_void_ptr;
  }
}

Animation_block *Symbol::get_animation_block_value() const
{
  validate_type_and_index(ANIMATION_BLOCK, UNDEFINED_INDEX);
  
  // arrays of Animation_blocks are not allowed
  assert(!is_array());
  // return &(*((Animation_block *) m_data_void_ptr));
  return (Animation_block *) m_data_void_ptr;
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

void Symbol::set(Animation_block* value)
{
  validate_type_and_index(ANIMATION_BLOCK, UNDEFINED_INDEX);
  m_data_void_ptr = (void *)value;
}

void Symbol::print(ostream &os) const
{
  if (is_array())
  {
    for(int i=0; i<m_size; i++) {
      string current_name = m_name + "[" + to_string(i) + "]";

      if (m_type & GAME_OBJECT)
      {
        ((Game_object *)m_data_void_ptr)->print(current_name, os);
      }
      else
      {
        os << gpl_type_to_base_string(m_type) << " " << current_name << " = ";

        switch (m_type)
        {
          case INT_ARRAY: os << get_int_value(i); break;
          case DOUBLE_ARRAY: os << get_double_value(i); break;
          case STRING_ARRAY: os << "\"" << get_string_value(i) << "\""; break;
          default: os << m_data_void_ptr;
        }
      }

      os << endl;
    }
  }
  else
  {
    if (m_type & GAME_OBJECT)
    {
      ((Game_object *)m_data_void_ptr)->print(m_name, os);
    }
    else if (m_type == ANIMATION_BLOCK)
    {
      os << gpl_type_to_base_string(m_type) << " " << m_name;
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
