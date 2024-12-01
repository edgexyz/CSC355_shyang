#ifndef ASSIGN_STATEMENT_H
#define ASSIGN_STATEMENT_H

#include "statement.h"
#include "gpl_type.h"

class Expression;
class Variable;

class Assign_statement : public Statement
{
  public:

    Assign_statement(Assign_operator op,
                     Variable *lhs,
                     Expression *rhs
                    );

    Assign_statement(Assign_operator op,
                     Variable *lhs
                    );

    virtual ~Assign_statement() {}
    virtual void execute();

  private:
    Assign_operator m_operator;
    Variable *m_lhs;
    Expression *m_rhs;
};

#endif // ifndef ASSIGN_STATEMENT_H
