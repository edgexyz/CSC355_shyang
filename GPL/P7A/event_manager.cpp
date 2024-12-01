#include "event_manager.h"
#include "statement_block.h"
using namespace std;

/* static */ Event_manager *Event_manager::m_instance = 0;

Event_manager::Event_manager()
{
  // don't need to do anyting in here
}

/* static */ Event_manager * Event_manager::instance()
{
  if (!m_instance)
    m_instance = new Event_manager();
  return m_instance;
}

void Event_manager::register_handler(Window::Keystroke keystroke, Statement_block *statement_block)
{
  m_blocks[keystroke].push_back(statement_block);
}

void Event_manager::execute_handlers(Window::Keystroke keystroke)
{
  for (unsigned int i = 0; i < m_blocks[keystroke].size(); i++)
    m_blocks[keystroke][i]->execute();

  /*
  vector<Statement_block *>::const_iterator iter;
  for (iter = m_blocks[keystroke].begin(); iter != m_blocks[keystroke].end(); iter++)
      (*iter)->execute();
  */
}
