/* parser.y: Simplest version of a calculator */
%{
#include <stdio.h>
#include <stdlib.h>

/* External declaration for the lexer */
extern int yylex();
extern int yylval;
void yyerror(const char *s);
%}

/* Declare tokens */
%token NUMBER
%token ADD SUB MUL DIV ABS
%token EOL

%%

/* Grammar rules */

calclist:
            /* nothing */
  | calclist exp EOL { printf("= %d\n", $2); }  /* $2 is the result of 'exp', EOL indicates the end of the expression */
  ;

exp:
       factor          { $$ = $1; }  /* Default action: copy the value of 'factor' */
  | exp ADD factor  { $$ = $1 + $3; }  /* Addition */
  | exp SUB factor  { $$ = $1 - $3; }  /* Subtraction */
  ;

factor:
          term            { $$ = $1; }  /* Default action: copy the value of 'term' */
  | factor MUL term { $$ = $1 * $3; }  /* Multiplication */
  | factor DIV term { $$ = $1 / $3; }  /* Division */
  ;

term:
        NUMBER          { $$ = $1; }  /* A number returns its value */
  ;

%%

/* Main function */

int main(int argc, char **argv)
{
    printf("Simple Calculator\n");
    printf("Enter expressions, one per line (CTRL+D to end):\n");
    return yyparse();
}

/* Error handling function */

void yyerror(const char *s)
{
    fprintf(stderr, "Error: %s\n", s);
}
