%{

#include "error.h"
#include "parser.h" // replacement for y.tab.h

#include <ctype.h>
#include <iostream>
using namespace std;


int line_count = 1;          // used to keep track of the input line

extern Error error_handler;  // error reporting object

/* count line feeds */
void count_lines(char *str)
{
  while(*str)
  {
    if (*str++ == '\n')
    {
        line_count ++;
    }
  }
}

void discard()
{
  count_lines(yytext);
}

/* the following functions are "called" when a token is found */

int emit(int token)
{
  count_lines(yytext);
  return(token);
}

int emit_with_line_number(int token)
{
  yylval.union_int = line_count;
  return emit(token);
}

int emit_str_constant(int token)
{
  // remove the last quote
  yytext[strlen(yytext)-1] = 0;
  // skip the first quote
  yylval.union_string = new string(yytext + 1);
  return emit(token);
}

int emit_id(int token)
{
  yylval.union_string = new string(yytext);
  return emit(token);
}

int emit_int(int token)
{
  yylval.union_int = atoi(yytext);
  return emit(token);
}

int emit_double(int token)
{
  yylval.union_double = atof(yytext);
  return emit(token);
}

int emit_error(int token)
{
  error_handler.error(Error::ILLEGAL_TOKEN,yytext);
  yylval.union_string = new string(yytext);
  return emit(token);
}

int j;

%}

DIGIT             [0-9]
INT_CONSTANT      ({DIGIT}|[1-9]{DIGIT}+)
DOUBLE_CONSTANT   {DIGIT}+[.]{DIGIT}*|{DIGIT}*[.]{DIGIT}+
STRING_CONSTANT   \"[^"\n]*\"
ID                [A-Za-z_][A-Za-z_0-9]*

%%

\/\/.*                              discard();
[ \n\t\r]                           discard();

int                                 return emit(T_INT);
double                              return emit(T_DOUBLE);
string                              return emit(T_STRING);
triangle                            return emit(T_TRIANGLE);
pixmap                              return emit(T_PIXMAP);
circle                              return emit(T_CIRCLE);
rectangle                           return emit(T_RECTANGLE);
textbox                             return emit(T_TEXTBOX);

forward                             return emit_with_line_number(T_FORWARD);
exit                                return emit_with_line_number(T_EXIT);
print                               return emit_with_line_number(T_PRINT);

initialization                      return emit(T_INITIALIZATION);
termination                         return emit(T_TERMINATION);
on                                  return emit(T_ON);
animation                           return emit(T_ANIMATION);
if                                  return emit(T_IF);
for                                 return emit(T_FOR);
else                                return emit(T_ELSE);
true                                return emit(T_TRUE);
false                               return emit(T_FALSE);

space                               return emit(T_SPACE);
leftarrow                           return emit(T_LEFTARROW);
rightarrow                          return emit(T_RIGHTARROW);
uparrow                             return emit(T_UPARROW);
downarrow                           return emit(T_DOWNARROW);
leftmouse_down                      return emit(T_LEFTMOUSE_DOWN);
middlemouse_down                    return emit(T_MIDDLEMOUSE_DOWN);
rightmouse_down                     return emit(T_RIGHTMOUSE_DOWN);
leftmouse_up                        return emit(T_LEFTMOUSE_UP);
middlemouse_up                      return emit(T_MIDDLEMOUSE_UP);
rightmouse_up                       return emit(T_RIGHTMOUSE_UP);
mouse_move                          return emit(T_MOUSE_MOVE);
mouse_drag                          return emit(T_MOUSE_DRAG);
f1                                  return emit(T_F1);
akey                                return emit(T_AKEY);
skey                                return emit(T_SKEY);
dkey                                return emit(T_DKEY);
fkey                                return emit(T_FKEY);
hkey                                return emit(T_HKEY);
jkey                                return emit(T_JKEY);
kkey                                return emit(T_KKEY);
lkey                                return emit(T_LKEY);
wkey                                return emit(T_WKEY);
zkey                                return emit(T_ZKEY);

"("                                 return emit(T_LPAREN);
")"                                 return emit(T_RPAREN);
"{"                                 return emit(T_LBRACE);
"}"                                 return emit(T_RBRACE);
"["                                 return emit(T_LBRACKET);
"]"                                 return emit(T_RBRACKET);
";"                                 return emit(T_SEMIC);
","                                 return emit(T_COMMA);
"."                                 return emit(T_PERIOD);

"="                                 return emit(T_ASSIGN);
"+="                                return emit(T_PLUS_ASSIGN);
"-="                                return emit(T_MINUS_ASSIGN);
"++"                                return emit(T_PLUS_PLUS);
"--"                                return emit(T_MINUS_MINUS);

sin                                 return emit(T_SIN);
cos                                 return emit(T_COS);
tan                                 return emit(T_TAN);
asin                                return emit(T_ASIN);
acos                                return emit(T_ACOS);
atan                                return emit(T_ATAN);
sqrt                                return emit(T_SQRT);
abs                                 return emit(T_ABS);
floor                               return emit(T_FLOOR);
random                              return emit(T_RANDOM);

touches                             return emit(T_TOUCHES);
near                                return emit(T_NEAR);

"*"                                 return emit(T_MULTIPLY);
"/"                                 return emit(T_DIVIDE);
"+"                                 return emit(T_PLUS);
"-"                                 return emit(T_MINUS);
"%"                                 return emit(T_MOD);

"<"                                 return emit(T_LESS);
">"                                 return emit(T_GREATER);
"<="                                return emit(T_LESS_EQUAL);
">="                                return emit(T_GREATER_EQUAL);
"=="                                return emit(T_EQUAL);
"!="                                return emit(T_NOT_EQUAL);
"!"                                 return emit(T_NOT);
"&&"                                return emit(T_AND);
"||"                                return emit(T_OR);

{INT_CONSTANT}                      return emit_int(T_INT_CONSTANT);
{DOUBLE_CONSTANT}                   return emit_double(T_DOUBLE_CONSTANT);
{STRING_CONSTANT}                   return emit_str_constant(T_STRING_CONSTANT);
{ID}                                return emit_id(T_ID);

.                                   return emit_error(T_ERROR);
