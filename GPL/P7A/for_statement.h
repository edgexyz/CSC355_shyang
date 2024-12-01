#ifndef FOR_STATEMENT_H
#define FOR_STATEMENT_H

#include "statement.h"

class Statement_block;
class Expression;

class For_statement : public Statement
{
  public:
    For_statement(Statement_block *initializer,
           Expression *expression,
           Statement_block *incrementor,
           Statement_block *body_block
          );

    virtual ~For_statement() {}

    virtual void execute();

  private:
    Statement_block *m_initializer;
    Expression *m_expression;
    Statement_block *m_incrementor;
    Statement_block *m_body_block;
};

#endif // ifndef FOR_STATEMENT_H
