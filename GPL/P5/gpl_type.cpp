// updated 2/14/2016

#include "gpl_type.h"
#include "gpl_assert.h"
using namespace std;

string gpl_type_to_string(Gpl_type gpl_type)
{
  switch (gpl_type)
  {
    case NO_TYPE: return "no type";
    case INT: return "int";
    case DOUBLE: return "double";
    case STRING: return "string";
    case ANIMATION_BLOCK: return "animation_block";
    case GAME_OBJECT: return "game_object";
    case CIRCLE: return "circle";
    case RECTANGLE: return "rectangle";
    case TRIANGLE: return "triangle";
    case TEXTBOX: return "textbox";
    case PIXMAP: return "pixmap";
    case ARRAY: return "array";
    case INT_ARRAY: return "int array";
    case DOUBLE_ARRAY: return "double array";
    case STRING_ARRAY: return "string array";
    case GAME_OBJECT_ARRAY: return "game_object array";
    case CIRCLE_ARRAY: return "circle array";
    case RECTANGLE_ARRAY: return "rectangle array";
    case TRIANGLE_ARRAY: return "triangle array";
    case TEXTBOX_ARRAY: return "textbox array";
    case PIXMAP_ARRAY: return "pixmap array";
    default: assert(false); return "error";
  }
}

string gpl_type_to_base_string(Gpl_type gpl_type)
{
  // if this is an array, use bitwise exclusive OR to unset ARRAY bit
  if (gpl_type & ARRAY)
    return gpl_type_to_string((Gpl_type) (gpl_type ^ ARRAY));
  else 
    return gpl_type_to_string(gpl_type);
}

string status_to_string(Status status)
{
  switch(status)
  {
    case OK: return "OK";
    case MEMBER_NOT_OF_GIVEN_TYPE: return "MEMBER_NOT_OF_GIVEN_TYPE";
    case MEMBER_NOT_DECLARED: return "MEMBER_NOT_DECLARED";
    default: assert(false);
  }
  assert(false);
  return ""; // to prevent a compilation warning
}

string operator_to_string(Operator_type op)
{
  switch(op)
  {
    case PLUS: return "+";
    case MINUS: return "-";
    case DIVIDE: return "/";
    case MULTIPLY: return "*";
    case MOD: return "%";
    case UNARY_MINUS: return "-";
    case NOT: return "!";
    case AND: return "&&";
    case OR: return "||";
    case EQUAL: return "==";
    case NOT_EQUAL: return "!=";
    case LESS_THAN: return "<";
    case LESS_EQUAL: return "<=";
    case GREATER_THAN: return ">";
    case GREATER_EQUAL: return ">=";
    case NEAR: return "near";
    case TOUCHES: return "touches";
    case SIN: return "sin";
    case COS: return "cos";
    case TAN : return "tan";
    case ASIN : return "asin";
    case ACOS : return "acos";
    case ATAN : return "atan";
    case SQRT : return "sqrt";
    case ABS: return "abs";
    case FLOOR: return "floor";
    case RANDOM: return "random"; 
    default: assert(false);
  }
  assert(false);
  return ""; // to prevent a compilation warning
}
