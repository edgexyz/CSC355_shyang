// updated 2/14/2016

/*
  Gpl_type are the types in gpl

  string gpl_type_to_string(Gpl_type gpl_type) returns a C++ string for
  the given type

*/

#ifndef GPL_TYPE_H
#define GPL_TYPE_H

#include <string>

// Assigned powers of 2 so that bitwise operations can be used
// Learning about bitwise operations will save you some time
// Use the enumerated value (e.g. CIRCLE), never the numbers (e.g. 48)
// 
// The number comments are provided to help with debugging
// NOTE: Comments must be changed if the values are ever changed
enum Gpl_type {NO_TYPE = 0,
               INT = 1,
               DOUBLE = 2,
               STRING = 4,
               ANIMATION_BLOCK = 8,
               GAME_OBJECT = 16,
               CIRCLE = 32 + GAME_OBJECT,                 // 48
               RECTANGLE = 64 + GAME_OBJECT,              // 80
               TRIANGLE = 128 + GAME_OBJECT,              // 144
               TEXTBOX = 256 + GAME_OBJECT,               // 272
               PIXMAP = 512 + GAME_OBJECT,                // 528
               ARRAY = 1024,                              // 1024
               INT_ARRAY = INT + ARRAY,                   // 1025
               DOUBLE_ARRAY = DOUBLE + ARRAY,             // 1026
               STRING_ARRAY = STRING + ARRAY,             // 1028
               GAME_OBJECT_ARRAY = GAME_OBJECT + ARRAY,   // 1040
               CIRCLE_ARRAY = CIRCLE + ARRAY,             // 1072
               RECTANGLE_ARRAY = RECTANGLE + ARRAY,       // 1104
               TRIANGLE_ARRAY = TRIANGLE + ARRAY,         // 1168
               TEXTBOX_ARRAY = TEXTBOX + ARRAY,           // 1296
               PIXMAP_ARRAY =  PIXMAP + ARRAY             // 1552
              };

// convert the Gpl_type enumerated value to a string (INT --> "int")
std::string gpl_type_to_string(Gpl_type gpl_type);

// same as above but removes the "array"
std::string gpl_type_to_base_string(Gpl_type gpl_type);

// status returned from Game_object functions
enum Status {OK,
             MEMBER_NOT_OF_GIVEN_TYPE,
             MEMBER_NOT_DECLARED,
             UNDEFINED_STATUS
             };

std::string status_to_string(Status status);

// NOTE: ++ and -- are not arithmetic operators
enum Operator_type {NO_OP,
                    PLUS, MINUS, MULTIPLY, DIVIDE, MOD,
                    UNARY_MINUS, NOT,
                    AND, OR,
                    EQUAL, NOT_EQUAL,
                    LESS_THAN, LESS_EQUAL,
                    GREATER_THAN, GREATER_EQUAL,
                    NEAR, TOUCHES,
                    SIN, COS, TAN,
                    ASIN, ACOS, ATAN,
                    SQRT, FLOOR, ABS, RANDOM
                   };

enum Assign_operator {ASSIGN, 
                      PLUS_ASSIGN, 
                      MINUS_ASSIGN, 
                      PLUS_PLUS, 
                      MINUS_MINUS
                     };

std::string operator_to_string(Operator_type op);

#endif // #ifndef GPL_TYPE_H
