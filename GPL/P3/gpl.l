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
  // COMPLETE ME
}

int emit_id(int token)
{
  // COMPLETE ME
}

int emit_int(int token)
{
  // COMPLETE ME
}

int emit_double(int token)
{
  // COMPLETE ME
}

int emit_error(int token)
{
  error_handler.error(Error::ILLEGAL_TOKEN,yytext);
  yylval.union_string = new string(yytext);
  return emit(token);
}

int j;

%}

%%

\/\/.*                             discard();
[ \n\t\r]                          discard();

int                                 return emit(T_INT);
double                              return emit(T_DOUBLE);

// COMPLETE ME
