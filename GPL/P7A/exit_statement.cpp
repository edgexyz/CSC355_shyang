#include <stdlib.h>
#include "exit_statement.h"
#include "gpl_assert.h"
#include "expression.h"

Exit_statement::Exit_statement(Expression *exit_status, int line)
{
  m_exit_status = exit_status;
  m_line = line;
  assert(m_exit_status);
  assert(m_exit_status->get_type() == INT);
}

/* virtual */ void Exit_statement::execute()
{
  int exit_status = m_exit_status->eval_int();
  cout << "gpl[" << m_line << "]: exit(" << exit_status << ")" << endl;

  Event_manager::instance()->execute_handlers(Window::TERMINATE);

  exit(exit_status);
}
