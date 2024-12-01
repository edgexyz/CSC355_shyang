#ifndef PRINT_STATEMENT_H
#define PRINT_STATEMENT_H

#include "statement.h"

class Expression;

class Print_statement : public Statement
{
  public:
    Print_statement(Expression *expression, int line);

    virtual ~Print_statement() {}

    virtual void execute();

  private:
    Expression *m_expression;
    int m_line;
};

#endif // ifndef PRINT_STATEMENT_H
