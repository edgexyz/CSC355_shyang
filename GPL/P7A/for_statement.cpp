#include "for_statement.h"
#include "gpl_assert.h"

#include "statement_block.h"
#include "expression.h"


For_statement::For_statement(Statement_block *initializer,
                             Expression *expression,
                             Statement_block *incrementor,
                             Statement_block *body_block
                            )
{
  assert(initializer);
  assert(expression);
  assert(expression->get_type() == INT);
  assert(incrementor);
  assert(body_block);
  m_initializer = initializer;
  m_expression= expression;
  m_incrementor = incrementor;
  m_body_block = body_block;

  //cout << "For_statement::For_statement() = " << endl;
  //cout << this;
}

/* virtual */ void For_statement::execute()
{
for(m_initializer->execute();m_expression->eval_int();m_incrementor->execute())
    m_body_block->execute();
}
