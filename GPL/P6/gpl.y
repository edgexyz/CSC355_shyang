%{

// there is a compatibility problem with my current cygwin environment
// this appears to fix the problem
#undef __GNUC_MINOR__

#include "error.h"
#include "gpl_assert.h"
#include "gpl_type.h"
#include "symbol.h"
#include "symbol_table.h"
#include "variable.h"
#include "expression.h"
#include "event_manager.h"
#include "statement_block.h"
#include "animation_block.h"
#include "game_object.h"
#include "triangle.h"
#include "pixmap.h"
#include "circle.h"
#include "rectangle.h"
#include "textbox.h"
#include "window.h"
#include <iostream>
#include <sstream>
#include <cmath> // for floor()
#include <stack>
using namespace std;

extern int yylex();
extern int yyerror(const char *);
extern int line_count;  // from gpl.l, used for statement blocks

int undeclared = 0;

Symbol* empty_symbol = new Symbol("__empty", 0);
Variable* empty_variable = new Variable(empty_symbol);

// Global variable to make the construction of object much less complex
// Only one object can ever be under construction at one time
Game_object *cur_object_under_construction = NULL;
string cur_object_under_construction_name;

Expression* semantic_check(Operator_type op, Expression *lhs, Expression *rhs, int valid)
{
    bool invalid = false;

    if (!(lhs->get_type() & valid))
    {
        if (rhs == NULL)
            Error::error(Error::INVALID_RIGHT_OPERAND_TYPE, operator_to_string(op));
        else
            Error::error(Error::INVALID_LEFT_OPERAND_TYPE, operator_to_string(op));
        
        invalid = true;
    }

    if (rhs != NULL && !(rhs->get_type() & valid))
    {
        Error::error(Error::INVALID_RIGHT_OPERAND_TYPE, operator_to_string(op));
        invalid = true;
    }

    if (invalid) return new Expression(0);

    return rhs != NULL ? new Expression(op, lhs, rhs) : new Expression(op, lhs);
}
%}

%union {
  int              union_int;
  double           union_double;
  std::string      *union_string;  // MUST be a pointer to a string ARG!
  Gpl_type         union_gpl_type;
  Operator_type    union_operator;
  Expression       *union_expression;
  Variable         *union_variable;
  Symbol           *union_symbol;
}

%error-verbose

%token T_INT                 "int"
%token T_DOUBLE              "double"
%token T_STRING              "string"
%token T_TRIANGLE            "triangle"
%token T_PIXMAP              "pixmap"
%token T_CIRCLE              "circle"
%token T_RECTANGLE           "rectangle"
%token T_TEXTBOX             "textbox"
%token <union_int> T_FORWARD "forward" // value is line number
%token T_INITIALIZATION      "initialization" 
%token T_TERMINATION         "termination" 

%token T_TRUE                "true"
%token T_FALSE               "false"

%token T_ON                  "on"
%token T_SPACE               "space"
%token T_LEFTARROW           "leftarrow"
%token T_RIGHTARROW          "rightarrow"
%token T_UPARROW             "uparrow"
%token T_DOWNARROW           "downarrow"
%token T_LEFTMOUSE_DOWN      "leftmouse_down"
%token T_MIDDLEMOUSE_DOWN    "middlemouse_down"
%token T_RIGHTMOUSE_DOWN     "rightmouse_down"
%token T_LEFTMOUSE_UP        "leftmouse_up"
%token T_MIDDLEMOUSE_UP      "middlemouse_up"
%token T_RIGHTMOUSE_UP       "rightmouse_up"
%token T_MOUSE_MOVE          "mouse_move"
%token T_MOUSE_DRAG          "mouse_drag"
%token T_F1                  "f1"
%token T_AKEY                "akey"
%token T_SKEY                "skey"
%token T_DKEY                "dkey"
%token T_FKEY                "fkey"
%token T_HKEY                "hkey"
%token T_JKEY                "jkey"
%token T_KKEY                "kkey"
%token T_LKEY                "lkey"
%token T_WKEY                "wkey"
%token T_ZKEY                "zkey"

%token T_TOUCHES             "touches"
%token T_NEAR                "near"

%token T_ANIMATION           "animation"

%token T_IF                  "if"
%token T_FOR                 "for"
%token T_ELSE                "else"
%token <union_int> T_PRINT   "print" // value is line number
%token <union_int> T_EXIT    "exit" // value is line number

%token T_LPAREN              "("
%token T_RPAREN              ")"
%token T_LBRACE              "{"
%token T_RBRACE              "}"
%token T_LBRACKET            "["
%token T_RBRACKET            "]"
%token T_SEMIC               ";"
%token T_COMMA               ","
%token T_PERIOD              "."

%token T_ASSIGN              "="
%token T_PLUS_ASSIGN         "+="
%token T_MINUS_ASSIGN        "-="
%token T_PLUS_PLUS           "++"
%token T_MINUS_MINUS         "--"

%token T_MULTIPLY            "*"
%token T_DIVIDE              "/"
%token T_MOD                 "%"
%token T_PLUS                "+"
%token T_MINUS               "-"
%token T_SIN                 "sin"
%token T_COS                 "cos"
%token T_TAN                 "tan"
%token T_ASIN                "asin"
%token T_ACOS                "acos"
%token T_ATAN                "atan"
%token T_SQRT                "sqrt"
%token T_FLOOR               "floor"
%token T_ABS                 "abs"
%token T_RANDOM              "random"

%token T_LESS                "<"
%token T_GREATER             ">"
%token T_LESS_EQUAL          "<="
%token T_GREATER_EQUAL       ">="
%token T_EQUAL               "=="
%token T_NOT_EQUAL           "!="

%token T_AND                 "&&"
%token T_OR                  "||"
%token T_NOT                 "!"

%token <union_string> T_ID              "identifier"
%token <union_int> T_INT_CONSTANT       "int constant"
%token <union_double> T_DOUBLE_CONSTANT "double constant"
%token <union_string> T_STRING_CONSTANT "string constant"

%token T_ERROR               "error"

%type <union_gpl_type> simple_type
%type <union_gpl_type> object_type
%type <union_operator> math_operator
%type <union_symbol> animation_parameter
%type <union_expression> optional_initializer
%type <union_expression> expression
%type <union_expression> primary_expression
%type <union_variable> variable


%nonassoc IF_NO_ELSE
%nonassoc T_ELSE

%left T_NEAR T_TOUCHES
%left T_OR 
%left T_AND
%left T_EQUAL T_NOT_EQUAL
%left T_LESS T_GREATER T_LESS_EQUAL T_GREATER_EQUAL 
%left T_PLUS T_MINUS
%left T_MULTIPLY T_DIVIDE T_MOD
%nonassoc UNARY_OPS

%%

//---------------------------------------------------------------------
program:
    declaration_list block_list
    ;

//---------------------------------------------------------------------
declaration_list:
    declaration_list declaration
    | empty
    ;

//---------------------------------------------------------------------
declaration:
    variable_declaration T_SEMIC
    | object_declaration T_SEMIC
    | forward_declaration T_SEMIC
    ;

//---------------------------------------------------------------------
variable_declaration:
    simple_type  T_ID  optional_initializer
    {
        Symbol_table *symbol_table = Symbol_table::instance();

        Symbol* s;

        switch ($1)
        {
            case INT:
            {
                int initial_value = 0;

                if ($3 != NULL)
                {
                    if ($3->get_type() != INT)
                        Error::error(Error::INVALID_TYPE_FOR_INITIAL_VALUE, gpl_type_to_string($3->get_type()), *$2, gpl_type_to_string($1));
                    else initial_value = $3->eval_int();
                }

                s = new Symbol(*$2, initial_value);
                break;
            }
                
            case DOUBLE:
            {
                double initial_value = 0.0;

                if ($3 != NULL)
                {
                    if ($3->get_type() != DOUBLE && $3->get_type() != INT)
                        Error::error(Error::INVALID_TYPE_FOR_INITIAL_VALUE, gpl_type_to_string($3->get_type()), *$2, gpl_type_to_string($1));
                    else initial_value = $3->eval_double();
                }

                s = new Symbol(*$2, initial_value);
                break;
            }
                
            case STRING:
            {
                string initial_value = "";

                if ($3 != NULL)
                {
                    if ($3->get_type() != STRING && $3->get_type() != INT && $3->get_type() != DOUBLE)
                        Error::error(Error::INVALID_TYPE_FOR_INITIAL_VALUE, gpl_type_to_string($3->get_type()), *$2, gpl_type_to_string($1));
                    else initial_value = $3->eval_string();
                }

                s = new Symbol(*$2, initial_value);
                break;
            }
            default: Error::error(Error::UNDEFINED_ERROR); break;
        }

        if (!symbol_table->insert(s))
        {
            Error::error(Error::PREVIOUSLY_DECLARED_VARIABLE, *$2);
        }
    }
    | simple_type  T_ID  T_LBRACKET expression T_RBRACKET
    {
        Symbol_table *symbol_table = Symbol_table::instance();
        int size = 1;

        if ($4->get_type() != INT)
        {
            Error::error(Error::ARRAY_SIZE_MUST_BE_AN_INTEGER, gpl_type_to_string($4->get_type()), *$2);
        }
        else
        {
            int evaluated = $4->eval_int();

            if (evaluated > 0)
            {
                size = evaluated;
            }
            else
            {
                Error::error(Error::INVALID_ARRAY_SIZE, *$2, to_string(evaluated));
            }
        }

        Symbol* s = new Symbol(*$2, (Gpl_type)$1, size);
        if (!symbol_table->insert(s))
        {
            Error::error(Error::PREVIOUSLY_DECLARED_VARIABLE, *$2);
        }
    }
    ;

//---------------------------------------------------------------------
simple_type:
    T_INT
    {
        $$ = INT;
    }
    | T_DOUBLE
    {
        $$ = DOUBLE;
    }
    | T_STRING
    {
        $$ = STRING;
    }
    ;

//---------------------------------------------------------------------
optional_initializer:
    T_ASSIGN expression
    {
        $$ = $2;
    }
    | empty
    {
        $$ = NULL;
    }
    ;

//---------------------------------------------------------------------
object_declaration:
    object_type T_ID 
    {
        // create a new object and it's symbol
        // (Symbol() creates the new object);
        Symbol_table *symbol_table = Symbol_table::instance();
        Symbol *s = new Symbol(*$2, $1);
    
        if (!symbol_table->insert(s))
        {
            Error::error(Error::PREVIOUSLY_DECLARED_VARIABLE, *$2);
        }

        // assign to global variable so the parameters can be inserted into
        // this object when each parameter is parsed
        cur_object_under_construction = s->get_game_object_value();
        cur_object_under_construction_name = s->get_name();
    } 
    T_LPAREN parameter_list_or_empty T_RPAREN
    {
        cur_object_under_construction = NULL;
        delete $2; // Scanner allocates memory for each T_ID string
    }
    | object_type T_ID T_LBRACKET expression T_RBRACKET
    {
        Symbol_table *symbol_table = Symbol_table::instance();
        int size = 1;

        if ($4->get_type() != INT)
        {
            Error::error(Error::ARRAY_SIZE_MUST_BE_AN_INTEGER, gpl_type_to_string($4->get_type()), *$2);
        }
        else
        {
            int evaluated = $4->eval_int();

            if (evaluated > 0)
            {
                size = evaluated;
            }
            else
            {
                Error::error(Error::INVALID_ARRAY_SIZE, *$2, to_string(evaluated));
            }
        }

        Symbol* s = new Symbol(*$2, (Gpl_type)$1, size);
        if (!symbol_table->insert(s))
        {
            Error::error(Error::PREVIOUSLY_DECLARED_VARIABLE, *$2);
        }
    }
    ;

//---------------------------------------------------------------------
object_type:
    T_TRIANGLE
    {
        $$ = TRIANGLE;
    }
    | T_PIXMAP
    {
        $$ = PIXMAP;
    }
    | T_CIRCLE
    {
        $$ = CIRCLE;
    }
    | T_RECTANGLE
    {
        $$ = RECTANGLE;
    }
    | T_TEXTBOX
    {
        $$ = TEXTBOX;
    }
    ;

//---------------------------------------------------------------------
parameter_list_or_empty :
    parameter_list
    | empty
    ;

//---------------------------------------------------------------------
parameter_list :
    parameter_list T_COMMA parameter
    | parameter
    ;

//---------------------------------------------------------------------
parameter:
    T_ID T_ASSIGN expression
    {
        string parameter = *$1;
        delete $1; // Scanner allocates memory for each T_ID string
		
		Expression *value_expression = $3;
		Gpl_type value_expression_type = value_expression->get_type();
		
		// get type of the parameter T_ID of cur_object_under_construction
		Status status;
		Gpl_type parameter_type;
		status = cur_object_under_construction->get_member_variable_type(parameter, parameter_type);

        if (status == MEMBER_NOT_DECLARED)
        {
            Error::error(Error::UNKNOWN_CONSTRUCTOR_PARAMETER, gpl_type_to_string(cur_object_under_construction->get_type()), parameter);
        }
        
        switch (parameter_type)
        {
            case INT:
            {
                if (value_expression_type != INT)
                {
                    Error::error(Error::INCORRECT_CONSTRUCTOR_PARAMETER_TYPE, cur_object_under_construction_name, parameter);
                }
                else 
                {
                    int value = value_expression->eval_int();
                    status = cur_object_under_construction->set_member_variable(parameter, value);
                    assert(status == OK);
                }

                break;
            }

            case DOUBLE:
            {
                if (value_expression_type != INT && value_expression_type != DOUBLE)
                {
                    Error::error(Error::INCORRECT_CONSTRUCTOR_PARAMETER_TYPE, cur_object_under_construction_name, parameter);
                }
                else 
                {
                    double value = value_expression->eval_double();
                    status = cur_object_under_construction->set_member_variable(parameter, value);
                    assert(status == OK);
                }

                break;
            }

            case STRING:
            {
                if (value_expression_type != INT && value_expression_type != DOUBLE && value_expression_type != STRING)
                {
                    Error::error(Error::INCORRECT_CONSTRUCTOR_PARAMETER_TYPE, cur_object_under_construction_name, parameter);
                }
                else 
                {
                    string value = value_expression->eval_string();
                    status = cur_object_under_construction->set_member_variable(parameter, value);
                    assert(status == OK);
                }

                break;
            }

            case ANIMATION_BLOCK:
            {
                if (value_expression_type != ANIMATION_BLOCK)
                {
                    Error::error(Error::INCORRECT_CONSTRUCTOR_PARAMETER_TYPE, cur_object_under_construction_name, parameter);
                }
                else 
                {
                    Animation_block* value = value_expression->eval_animation_block();
                    
                    Gpl_type animation_block_type = value->get_parameter_symbol()->get_type();
                    Gpl_type object_type = cur_object_under_construction->get_type();

                    if (animation_block_type != object_type)
                    {
                        Error::error(Error::TYPE_MISMATCH_BETWEEN_ANIMATION_BLOCK_AND_OBJECT, cur_object_under_construction_name, value->name());
                    }
                    else
                    {
                        status = cur_object_under_construction->set_member_variable(parameter, value);
                        assert(status == OK);
                    }
                }

                break;
            }
        }
    }
    ;

//---------------------------------------------------------------------
forward_declaration:
    T_FORWARD T_ANIMATION T_ID T_LPAREN animation_parameter T_RPAREN
    {
        Symbol_table *symbol_table = Symbol_table::instance();
        Symbol *s = new Symbol(*$3, ANIMATION_BLOCK);

        Animation_block *animation = s->get_animation_block_value();
        animation->initialize($5, *$3);

        if (!symbol_table->insert(s))
        {
            Error::error(Error::PREVIOUSLY_DECLARED_VARIABLE, *$3);
        }
    }
    ;

//---------------------------------------------------------------------
block_list:
    block_list block
    | empty
    ;

//---------------------------------------------------------------------
block:
    initialization_block
    | termination_block
    | animation_block
    | on_block
    ;

//---------------------------------------------------------------------
initialization_block:
    T_INITIALIZATION statement_block
    ;

//---------------------------------------------------------------------
termination_block:
    T_TERMINATION statement_block
    ;

//---------------------------------------------------------------------
animation_block:
    T_ANIMATION T_ID T_LPAREN check_animation_parameter T_RPAREN T_LBRACE statement_list T_RBRACE end_of_statement_block
    ;

//---------------------------------------------------------------------
animation_parameter:
    object_type T_ID
    {
        Symbol_table *symbol_table = Symbol_table::instance();
        Symbol *s = new Symbol(*$2, $1);

        Game_object *game_object = s->get_game_object_value();
        game_object->never_animate();
        game_object->never_draw();

        if (!symbol_table->insert(s))
        {
            Error::error(Error::ANIMATION_PARAMETER_NAME_NOT_UNIQUE, *$2);
            $$ = NULL;
        }
        else
        {
            $$ = s;
        }
    }
    ;

//---------------------------------------------------------------------
check_animation_parameter:
    object_type T_ID
    ;

//---------------------------------------------------------------------
on_block:
    T_ON keystroke statement_block
    ;

//---------------------------------------------------------------------
keystroke:
    T_SPACE
    | T_LEFTARROW
    | T_RIGHTARROW
    | T_UPARROW
    | T_DOWNARROW
    | T_LEFTMOUSE_DOWN
    | T_MIDDLEMOUSE_DOWN
    | T_RIGHTMOUSE_DOWN
    | T_LEFTMOUSE_UP
    | T_MIDDLEMOUSE_UP
    | T_RIGHTMOUSE_UP
    | T_MOUSE_MOVE
    | T_MOUSE_DRAG
    | T_F1
    | T_AKEY
    | T_SKEY
    | T_DKEY
    | T_FKEY
    | T_HKEY
    | T_JKEY
    | T_KKEY
    | T_LKEY
    | T_WKEY
    | T_ZKEY
    ;

//---------------------------------------------------------------------
if_block:
    statement_block_creator statement end_of_statement_block
    | statement_block
    ;

//---------------------------------------------------------------------
statement_block:
    T_LBRACE statement_block_creator statement_list T_RBRACE end_of_statement_block
    ;

//---------------------------------------------------------------------
statement_block_creator:
    // this goes to nothing so that you can put an action here in p7
    ;

//---------------------------------------------------------------------
end_of_statement_block:
    // this goes to nothing so that you can put an action here in p7
    ;

//---------------------------------------------------------------------
statement_list:
    statement_list statement
    | empty
    ;

//---------------------------------------------------------------------
statement:
    | assign_statement T_SEMIC
    | print_statement T_SEMIC
    | exit_statement T_SEMIC
    | if_statement
    | for_statement
    ;

//---------------------------------------------------------------------
if_statement:
    T_IF T_LPAREN expression T_RPAREN if_block %prec IF_NO_ELSE
    | T_IF T_LPAREN expression T_RPAREN if_block T_ELSE if_block
    ;

//---------------------------------------------------------------------
for_statement:
    T_FOR T_LPAREN statement_block_creator assign_statement_or_empty end_of_statement_block T_SEMIC expression T_SEMIC statement_block_creator assign_statement_or_empty end_of_statement_block T_RPAREN statement_block
    ;

//---------------------------------------------------------------------
print_statement:
    T_PRINT T_LPAREN expression T_RPAREN
    ;

//---------------------------------------------------------------------
exit_statement:
    T_EXIT T_LPAREN expression T_RPAREN
    ;

//---------------------------------------------------------------------
assign_statement_or_empty:
    assign_statement
    | empty
    ;

//---------------------------------------------------------------------
assign_statement:
    variable T_ASSIGN expression
    | variable T_PLUS_ASSIGN expression
    | variable T_MINUS_ASSIGN expression
    | variable T_PLUS_PLUS
    | variable T_MINUS_MINUS
    ;

//---------------------------------------------------------------------
variable:
    T_ID
    {
        Symbol_table *symbol_table = Symbol_table::instance();
        Symbol* s = symbol_table->lookup(*$1);

        if (s != NULL)
        {
            if (s->is_array())
            {
                Error::error(Error::VARIABLE_IS_AN_ARRAY, *$1);
                $$ = empty_variable;
            }
            else
            {
                $$ = new Variable(s);
            }
        }
        else
        {
            Error::error(Error::UNDECLARED_VARIABLE, *$1);
            $$ = empty_variable;
        }
    }
    | T_ID T_LBRACKET expression T_RBRACKET
    {
        Symbol_table *symbol_table = Symbol_table::instance();
        Symbol* s = symbol_table->lookup(*$1);

        if (s != NULL)
        {
            if ($3->get_type() != INT)
            {
                string msg = "";

                switch ($3->get_type())
                {
                    case DOUBLE: msg = "A double expression"; break;
                    case STRING: msg = "A string expression"; break;
                    case ANIMATION_BLOCK: msg = "A animation_block expression"; break;
                }

                Error::error(Error::ARRAY_INDEX_MUST_BE_AN_INTEGER, *$1, msg);
                $$ = empty_variable;
            }
            else
            {
                if (s->is_array())
                {
                    $$ = new Variable(s, $3);
                }
                else
                {
                    Error::error(Error::VARIABLE_NOT_AN_ARRAY, *$1);
                    $$ = empty_variable;
                }
            }
        }
        else
        {
            Error::error(Error::UNDECLARED_VARIABLE, *$1+"[]");
            $$ = empty_variable;
        }
    }
    | T_ID T_PERIOD T_ID
    {
        Symbol_table *symbol_table = Symbol_table::instance();
        Symbol* s = symbol_table->lookup(*$1);

        if (s != NULL)
        {
            if (s->is_array())
            {
                Error::error(Error::VARIABLE_IS_AN_ARRAY, *$1);
                $$ = empty_variable;
            }
            else if(!(s->get_type() & GAME_OBJECT))
		    {
			    Error::error(Error::LHS_OF_PERIOD_MUST_BE_OBJECT,*$1);
                $$ = empty_variable;
            }
            else
            {
                Game_object* game_object = s->get_game_object_value();
                Gpl_type type;

                Status status = game_object->get_member_variable_type(*$3, type);

                if (status == MEMBER_NOT_DECLARED)
                {
                    Error::error(Error::UNDECLARED_MEMBER, *$1, *$3);
                    $$ = empty_variable;
                }
                else
                    $$ = new Variable(s, $3);
            }
        }
        else
        {
            Error::error(Error::UNDECLARED_VARIABLE, *$1);
            $$ = empty_variable;
        }
    }
    | T_ID T_LBRACKET expression T_RBRACKET T_PERIOD T_ID
    {
        Symbol_table *symbol_table = Symbol_table::instance();
        Symbol* s = symbol_table->lookup(*$1);

        if (s != NULL)
        {
            if ($3->get_type() != INT)
            {
                string msg = "";

                switch ($3->get_type())
                {
                    case DOUBLE: msg = "A double expression"; break;
                    case STRING: msg = "A string expression"; break;
                    case ANIMATION_BLOCK: msg = "A animation_block expression"; break;
                }

                Error::error(Error::ARRAY_INDEX_MUST_BE_AN_INTEGER, *$1, msg);
                $$ = empty_variable;
            }
            else
            {
                if (!(s->is_array()))
                {
                    Error::error(Error::VARIABLE_NOT_AN_ARRAY, *$1);
                    $$ = empty_variable;
                }
                else if (!(s->get_type() & GAME_OBJECT))
                {
                    Error::error(Error::LHS_OF_PERIOD_MUST_BE_OBJECT,*$1);
                    $$ = empty_variable;
                }
                else
                {
                    Game_object* game_object = s->get_game_object_value($3->eval_int());
                    Gpl_type type;

                    Status status = game_object->get_member_variable_type(*$6, type);

                    if (status == MEMBER_NOT_DECLARED)
                    {
                        Error::error(Error::UNDECLARED_MEMBER, *$1+"[]", *$6);
                        $$ = empty_variable;
                    }
                    else
                        $$ = new Variable(s, $3, $6);
                    }
            }
        }
        else
        {
            Error::error(Error::UNDECLARED_VARIABLE, *$1+"[]");
            $$ = empty_variable;
        }
    }
    ;

//---------------------------------------------------------------------
expression:
    primary_expression
    {
        $$ = $1;
    }
    | expression T_OR expression
    {
        $$ = semantic_check(OR, $1, $3, INT|DOUBLE);
    }
    | expression T_AND expression
    {
        $$ = semantic_check(AND, $1, $3, INT|DOUBLE);
    }
    | expression T_LESS_EQUAL expression
    {
        $$ = semantic_check(LESS_EQUAL, $1, $3, INT|DOUBLE|STRING);
    }
    | expression T_GREATER_EQUAL  expression
    {
        $$ = semantic_check(GREATER_EQUAL, $1, $3, INT|DOUBLE|STRING);
    }
    | expression T_LESS expression
    {
        $$ = semantic_check(LESS_THAN, $1, $3, INT|DOUBLE|STRING);
    }
    | expression T_GREATER  expression
    {
        $$ = semantic_check(GREATER_THAN, $1, $3, INT|DOUBLE|STRING);
    }
    | expression T_EQUAL expression
    {
        $$ = semantic_check(EQUAL, $1, $3, INT|DOUBLE|STRING);
    }
    | expression T_NOT_EQUAL expression
    {
        $$ = semantic_check(NOT_EQUAL, $1, $3, INT|DOUBLE|STRING);
    }
    | expression T_PLUS expression
    {
        $$ = semantic_check(PLUS, $1, $3, INT|DOUBLE|STRING);
    }
    | expression T_MINUS expression
    {
        $$ = semantic_check(MINUS, $1, $3, INT|DOUBLE);
    }
    | expression T_MULTIPLY expression
    {
        $$ = semantic_check(MULTIPLY, $1, $3, INT|DOUBLE);
    }
    | expression T_DIVIDE expression
    {
        $$ = semantic_check(DIVIDE, $1, $3, INT|DOUBLE);
    }
    | expression T_MOD expression
    {
        $$ = semantic_check(MOD, $1, $3, INT);
    }
    | T_MINUS  expression %prec UNARY_OPS
    {
        $$ = semantic_check(UNARY_MINUS, $2, NULL, INT|DOUBLE);
    }
    | T_NOT  expression %prec UNARY_OPS
    {
        $$ = semantic_check(NOT, $2, NULL, INT|DOUBLE);
    }
    | math_operator T_LPAREN expression T_RPAREN
    {
        $$ = semantic_check($1, $3, NULL, INT|DOUBLE);
    }
    | expression T_NEAR expression
    | expression T_TOUCHES expression
    ;

//---------------------------------------------------------------------
primary_expression:
    T_LPAREN  expression T_RPAREN
    {
        $$ = $2;
    }
    | variable
    {
        $$ = new Expression($1);
    }
    | T_INT_CONSTANT
    {
        $$ = new Expression($1);
    }
    | T_TRUE
    {
        $$ = new Expression(1);
    }
    | T_FALSE
    {
        $$ = new Expression(0);
    }
    | T_DOUBLE_CONSTANT
    {
        $$ = new Expression($1);
    }
    | T_STRING_CONSTANT
    {
        $$ = new Expression($1);
    }
    ;

//---------------------------------------------------------------------
math_operator:
    T_SIN
    {
        $$ = SIN;
    }
    | T_COS
    {
        $$ = COS;
    }
    | T_TAN
    {
        $$ = TAN;
    }
    | T_ASIN
    {
        $$ = ASIN;
    }
    | T_ACOS
    {
        $$ = ACOS;
    }
    | T_ATAN
    {
        $$ = ATAN;
    }
    | T_SQRT
    {
        $$ = SQRT;
    }
    | T_ABS
    {
        $$ = ABS;
    }
    | T_FLOOR
    {
        $$ = FLOOR;
    }
    | T_RANDOM
    {
        $$ = RANDOM;
    }
    ;

//---------------------------------------------------------------------
empty:
    // empty goes to nothing so that you can use empty in productions
    // when you want a production to go to nothing
    ;
