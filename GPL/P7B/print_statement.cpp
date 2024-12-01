#include "print_statement.h"

#include <string>
#include "gpl_assert.h"
#include "expression.h"


Print_statement::Print_statement(Expression *expression, int line
                                )
{
  assert(expression);
  m_expression = expression;
  m_line = line;

  assert(m_expression->get_type() == INT
         || m_expression->get_type() == DOUBLE
         || m_expression->get_type() == STRING
        );
}

/* virtual */ void Print_statement::execute()
{
  cout << "print[" << m_line << "]: " << m_expression->eval_string() << endl;
}
