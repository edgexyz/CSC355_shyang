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
#include "if_statement.h"
#include "for_statement.h"
#include "print_statement.h"
#include "exit_statement.h"
#include "assign_statement.h"
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

// Global stack of statement blocks
stack<Statement_block *> statement_block_stack;

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
  int                   union_int;
  double                union_double;
  std::string           *union_string;  // MUST be a pointer to a string ARG!
  Gpl_type              union_gpl_type;
  Operator_type         union_operator;
  Expression            *union_expression;
  Variable              *union_variable;
  Symbol                *union_symbol;
  Statement_block       *union_statement_block;
  Window::Keystroke     union_keystroke;
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
%type <union_statement_block> statement_block_creator
%type <union_statement_block> statement_block
%type <union_statement_block> if_block
%type <union_keystroke> keystroke


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
    {
        Event_manager *event_manager = Event_manager::instance();

        event_manager->register_handler(Window::INITIALIZE, $2);
    }
    ;

//---------------------------------------------------------------------
termination_block:
    T_TERMINATION statement_block
    {
        Event_manager *event_manager = Event_manager::instance();

        event_manager->register_handler(Window::TERMINATE, $2);
    }
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
    {
        Event_manager *event_manager = Event_manager::instance();

        event_manager->register_handler($2, $3);
    }
    ;

//---------------------------------------------------------------------
keystroke:
    T_SPACE
    {
        $$ = Window::SPACE;
    }
    | T_LEFTARROW
    {
        $$ = Window::LEFTARROW;
    }
    | T_RIGHTARROW
    {
        $$ = Window::RIGHTARROW;
    }
    | T_UPARROW
    {
        $$ = Window::UPARROW;
    }
    | T_DOWNARROW
    {
        $$ = Window::DOWNARROW;
    }
    | T_LEFTMOUSE_DOWN
    {
        $$ = Window::LEFTMOUSE_DOWN;
    }
    | T_MIDDLEMOUSE_DOWN
    {
        $$ = Window::MIDDLEMOUSE_DOWN;
    }
    | T_RIGHTMOUSE_DOWN
    {
        $$ = Window::RIGHTMOUSE_DOWN;
    }
    | T_LEFTMOUSE_UP
    {
        $$ = Window::LEFTMOUSE_UP;
    }
    | T_MIDDLEMOUSE_UP
    {
        $$ = Window::MIDDLEMOUSE_UP;
    }
    | T_RIGHTMOUSE_UP
    {
        $$ = Window::RIGHTMOUSE_UP;
    }
    | T_MOUSE_MOVE
    {
        $$ = Window::MOUSE_MOVE;
    }
    | T_MOUSE_DRAG
    {
        $$ = Window::MOUSE_DRAG;
    }
    | T_F1
    {
        $$ = Window::F1;
    }
    | T_AKEY
    {
        $$ = Window::AKEY;
    }
    | T_SKEY
    {
        $$ = Window::SKEY;
    }
    | T_DKEY
    {
        $$ = Window::DKEY;
    }
    | T_FKEY
    {
        $$ = Window::FKEY;
    }
    | T_HKEY
    {
        $$ = Window::HKEY;
    }
    | T_JKEY
    {
        $$ = Window::JKEY;
    }
    | T_KKEY
    {
        $$ = Window::KKEY;
    }
    | T_LKEY
    {
        $$ = Window::LKEY;
    }
    | T_WKEY
    {
        $$ = Window::WKEY;
    }
    | T_ZKEY
    {
        $$ = Window::ZKEY;
    }
    ;

//---------------------------------------------------------------------
if_block:
    statement_block_creator statement end_of_statement_block
    {
        $$ = $1;
    }
    | statement_block
    {
        $$ = $1;
    }
    ;

//---------------------------------------------------------------------
statement_block:
    T_LBRACE statement_block_creator statement_list T_RBRACE end_of_statement_block
    {
        $$ = $2;
    }
    ;

//---------------------------------------------------------------------
statement_block_creator:
    {
        Statement_block *new_block = new Statement_block();
        statement_block_stack.push(new_block);
        $$ = new_block;
    }
    ;

//---------------------------------------------------------------------
end_of_statement_block:
    {
        assert(!statement_block_stack.empty());
        statement_block_stack.pop();
    }
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
    {
        Expression *expr = $3;
        if (expr->get_type() != INT)
        {
            Error::error(Error::INVALID_TYPE_FOR_IF_STMT_EXPRESSION);
            expr = new Expression(0);
        }
        statement_block_stack.top()->insert(new If_statement(expr,$5));
    }
    | T_IF T_LPAREN expression T_RPAREN if_block T_ELSE if_block
    {
        Expression *expr = $3;
        if (expr->get_type() != INT)
        {
            Error::error(Error::INVALID_TYPE_FOR_IF_STMT_EXPRESSION);
            expr = new Expression(0);
        }
        statement_block_stack.top()->insert(new If_statement(expr,$5, $7));
    }
    ;

//---------------------------------------------------------------------
for_statement:
    T_FOR T_LPAREN statement_block_creator assign_statement_or_empty end_of_statement_block T_SEMIC expression T_SEMIC statement_block_creator assign_statement_or_empty end_of_statement_block T_RPAREN statement_block
    {
        Expression *expr = $7;
        if (expr->get_type() != INT)
        {
            Error::error(Error::INVALID_TYPE_FOR_FOR_STMT_EXPRESSION);
            expr = new Expression(0);
        }
        statement_block_stack.top()->insert(new For_statement($3, expr, $9, $13));
    }
    ;

//---------------------------------------------------------------------
print_statement:
    T_PRINT T_LPAREN expression T_RPAREN
    {
        Expression *expr = $3;
        if (expr->get_type() != INT
            && expr->get_type() != DOUBLE
            && expr->get_type() != STRING
        )
        {
            Error::error(Error::INVALID_TYPE_FOR_PRINT_STMT_EXPRESSION);
            // for error handling
            expr = new Expression(0);
        }
        statement_block_stack.top()->insert(new Print_statement(expr, $1)); // $1 has line_number
    }
    ;

//---------------------------------------------------------------------
exit_statement:
    T_EXIT T_LPAREN expression T_RPAREN
    {
        Expression *expr = $3;
        if(expr->get_type() != INT)
        {
            Error::error(Error::EXIT_STATUS_MUST_BE_AN_INTEGER, gpl_type_to_string(expr->get_type()));
            // for error handling
            expr = new Expression(0);
        }

        statement_block_stack.top()->insert(new Exit_statement(expr, $1)); // $1 has line_number
    }
    ;

//---------------------------------------------------------------------
assign_statement_or_empty:
    assign_statement
    | empty
    ;

//---------------------------------------------------------------------
assign_statement:
    variable T_ASSIGN expression
    {
        Variable *lhs = $1;
        Expression *rhs = $3;
        Gpl_type lhs_type = lhs->get_type();
        Gpl_type rhs_type = rhs->get_type();

        // game_object is illegal on lhs of assignment
        if (lhs_type & GAME_OBJECT)
        {
            Error::error(Error::INVALID_LHS_OF_ASSIGNMENT,
                lhs->get_name(),
                gpl_type_to_string(lhs_type)
                );
        }

        // if variable is an INT, expression must be INT
        // if variable is a DOUBLE, expression must be INT or DOUBLE
        // if variable is a STRING, expression must be STRING,INT, or DOUBLE
        // if variable is a ANIMATION_BLOCK, expression ANIMATION_BLOCK
        else if ((lhs_type == INT && rhs_type != INT)
            ||(lhs_type==DOUBLE&&(rhs_type != INT && rhs_type!=DOUBLE))
            ||(lhs_type == STRING && rhs_type == ANIMATION_BLOCK)
            ||(lhs_type==ANIMATION_BLOCK&& rhs_type != ANIMATION_BLOCK)
            )
        {
            Error::error(Error::ASSIGNMENT_TYPE_ERROR,
                        gpl_type_to_string(lhs_type),
                        gpl_type_to_string(rhs_type)
                        );
        }
            else if (lhs_type==ANIMATION_BLOCK)
            {
            // since lhs is an ANIMATION_BLOCK, it SHOULD take one of these forms
            // circle.animation_block =
            // circles[index].animation_block =

            // this is ok
            //   my_rect.animation_block = bounce;
            // this is NOT ok
            //   bounce = move;
            // check to make sure it is not this illegal form
            if (lhs->is_non_member_animation_block())
            {
                Error::error(Error::CANNOT_ASSIGN_TO_NON_MEMBER_ANIMATION_BLOCK,
                            lhs->get_name()
                            );
            }
            else
            {
    
                // get the type of the Game_object on the LHS
                Gpl_type lhs_base_object_type = lhs->get_base_game_object_type();
    
                Gpl_type rhs_param_type = rhs->eval_animation_block()->get_parameter_symbol()->get_type();
    
                // Animation_block *block = rhs->eval_animation_block();
                // Symbol *sym = block->get_parameter_symbol();
    
                if (lhs_base_object_type != rhs_param_type)
                {
                Error::error(Error::ANIMATION_BLOCK_ASSIGNMENT_PARAMETER_TYPE_ERROR,
                            gpl_type_to_string(lhs_base_object_type),
                            gpl_type_to_string(rhs_param_type)
                            );
    
                }
                else statement_block_stack.top()->insert(new Assign_statement(ASSIGN, lhs, rhs));
                }
            }
        else // the types are ok
        {
            statement_block_stack.top()->insert(new Assign_statement(ASSIGN, lhs, rhs));
        }
    }
    | variable T_PLUS_ASSIGN expression
    {
        Gpl_type lhs_type = $1->get_type();
        Gpl_type rhs_type = $3->get_type();

        // game_object & statement_block are illegal on lhs of +=
        if ((lhs_type & GAME_OBJECT) || (lhs_type == ANIMATION_BLOCK))
        {
            Error::error(Error::INVALID_LHS_OF_PLUS_ASSIGNMENT,
                    $1->get_name(),
                    gpl_type_to_string(lhs_type)
                );
        }

        // if variable is an INT, expression must be INT
        // if variable is a DOUBLE, expression must be INT or DOUBLE
        // if variable is a STRING, expression must be STRING,INT, or DOUBLE
        else if ((lhs_type == INT && rhs_type != INT)
            ||(lhs_type==DOUBLE&&(rhs_type != INT && rhs_type!=DOUBLE))
            ||(lhs_type == STRING && rhs_type == ANIMATION_BLOCK)
            )
        {
            Error::error(Error::PLUS_ASSIGNMENT_TYPE_ERROR,
                gpl_type_to_string(lhs_type),
                gpl_type_to_string(rhs_type)
                );
        }
        else // the types are ok
        {
            statement_block_stack.top()->insert(new Assign_statement(PLUS_ASSIGN, $1, $3));
        }
    }
    | variable T_MINUS_ASSIGN expression
    {
        Gpl_type lhs_type = $1->get_type();
        Gpl_type rhs_type = $3->get_type();

        // game_object & statement_block & string are illegal on lhs of +=
        if (lhs_type != INT && lhs_type != DOUBLE)
        {
            Error::error(Error::INVALID_LHS_OF_MINUS_ASSIGNMENT,
                    $1->get_name(),
                    gpl_type_to_string(lhs_type)
                    );
        }

        // if variable is an INT, expression must be INT
        // if variable is a DOUBLE, expression must be INT or DOUBLE
        else if ((lhs_type == INT && rhs_type != INT)
            ||(lhs_type==DOUBLE&&(rhs_type != INT && rhs_type!=DOUBLE))
            )
        {
            Error::error(Error::MINUS_ASSIGNMENT_TYPE_ERROR,
                gpl_type_to_string(lhs_type),
                gpl_type_to_string(rhs_type)
                );
        }
        else // the types are ok
        {
            statement_block_stack.top()->insert(new Assign_statement(MINUS_ASSIGN, $1, $3));
        }
    }
    | variable T_PLUS_PLUS
    {
        Gpl_type lhs_type = $1->get_type();
        if (lhs_type != INT)
        {
            Error::error(Error::INVALID_LHS_OF_PLUS_PLUS,
                    $1->get_name(),
                    gpl_type_to_string(lhs_type)
                    );
        }
        else // the types are ok
        {
            statement_block_stack.top()->insert(new Assign_statement(PLUS_PLUS, $1));
        }
    }
    | variable T_MINUS_MINUS
    {
        Gpl_type lhs_type = $1->get_type();
        if (lhs_type != INT)
        {
            Error::error(Error::INVALID_LHS_OF_MINUS_MINUS,
                    $1->get_name(),
                    gpl_type_to_string(lhs_type)
                    );
        }
        else // the types are ok
        {
            statement_block_stack.top()->insert(new Assign_statement(MINUS_MINUS, $1));
        }
    }
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
