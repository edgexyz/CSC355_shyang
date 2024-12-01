#ifndef IF_STATEMENT_H
#define IF_STATEMENT_H

#include "statement.h"

class Statement_block;
class Expression;

class If_statement : public Statement
{
  public:
    If_statement(Expression *expression,
                 Statement_block *then_block,
                 Statement_block *else_block = 0
                );

virtual ~If_statement() {}

    virtual void execute();

  private:
    Expression *m_expression = nullptr;
    Statement_block *m_then_block = nullptr;
    Statement_block *m_else_block = nullptr;
};

#endif // ifndef IF_STATEMENT_H
