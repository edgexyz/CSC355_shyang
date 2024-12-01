#ifndef EXIT_STATEMENT_H
#define EXIT_STATEMENT_H

#include "event_manager.h"

#include "statement.h"
class Expression;

class Exit_statement : public Statement
{
  public:
    Exit_statement(Expression *exit_status, int line);

    virtual ~Exit_statement() {}

    virtual void execute();

  private:
    Expression *m_exit_status;
    int m_line;
};

#endif // ifndef EXIT_STATEMENT_H
