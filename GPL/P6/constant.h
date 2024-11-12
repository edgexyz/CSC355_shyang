/*
  class Constant provides a means for storing an INT, DOUBLE, or STRING constant
  
  Do not alter this class to low changes to the values.  If you are changing
  constant values, you are doing something incorrect that will come back
  to bite you in phase 7.

  YOU MUST use this class to store ALL constant values!

  The Expression class should point to a Constant object

  A constant never changes value or type.  Once it is created it is the 
  same value and type for the rest of the program's execution.

  How to use:

    construct a constant:  
      Constant my_const(42);

    get the value of a constant:
      m_const.get_int_value();

    get the type of a constant:
      m_const.get_type();
      
    An assert will fail (the the program will be terminated) if get_*_value() 
    is evaluated for an incompatible type (one that cannot be cast).

    For example:

      Constant my_int_const(42);
      my_int_const.get_int_value();       // this is OK
      my_int_const.get_double_value();    // this is OK, INT casts to DOUBLE
      my_int_const.get_string_value();    // this is OK, INT casts to STRING

      Constant my_double_const(3.14);
      my_double_const.get_int_value();    // ERROR, DOUBLE can't cast to INT
      my_double_const.get_double_value(); // this is OK
      my_double_const.get_string_value(); // this is OK, DOUBLE casts to STRING

      Constant my_string_const(3.14);
      my_string_const.get_int_value();    // ERROR, STRING can't cast to INT
      my_string_const.get_double_value(); // this is OK
      my_string_const.get_string_value(); // this is OK

*/
#ifndef CONSTANT_H
#define CONSTANT_H

#include "gpl_type.h"
#include "gpl_assert.h"
#include <string>

class Constant
{
  public:
    Constant(int value) : m_union_value(value) {m_type = INT;}
    Constant(double value) : m_union_value(value) {m_type = DOUBLE;}
    Constant(std::string value) : m_union_value(value) {m_type = STRING;}

    Gpl_type get_type() {return m_type;}

    int get_int_value();
    double get_double_value();
    std::string get_string_value();

  private:
    union Constant_union
    {
      Constant_union(int value) {union_int = value;}
      Constant_union(double value) {union_double = value;}
      Constant_union(std::string value) {union_string = new std::string(value);}
      ~Constant_union() {};
      int union_int;
      double union_double;
      std::string *union_string;
    };
    
    Constant_union m_union_value;

    Gpl_type m_type = NO_TYPE; // should never equal NO_TYPE, set for debugging
};

#endif // #ifndef CONSTANT_H
