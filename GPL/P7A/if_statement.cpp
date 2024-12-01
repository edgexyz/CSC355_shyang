#include "if_statement.h"
#include "gpl_assert.h"

#include "statement_block.h"
#include "expression.h"

If_statement::If_statement(Expression *expression,
                           Statement_block *then_block,
                           Statement_block *else_block /* = NULL */
                          )
{
  assert(expression);
  assert(expression->get_type() == INT || expression->get_type() == DOUBLE);
  assert(then_block);

  m_expression = expression;
  m_then_block = then_block;
  m_else_block = else_block;
}

/* virtual */ void If_statement::execute()
{
  //if (m_expression->eval_double() != 0.0)

  if (m_expression->eval_int() != 0)
    m_then_block->execute();
  else if (m_else_block)
    m_else_block->execute();
}
