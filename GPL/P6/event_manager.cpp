// updated 2/14/2016

#include "event_manager.h"
#include "gpl_assert.h"
using namespace std;

/* static */ Event_manager *Event_manager::m_instance = 0;

/* static */ Event_manager * Event_manager::instance()
{
  if (!m_instance)
    m_instance = new Event_manager();
  return m_instance;
}

Event_manager::Event_manager()
{
}

Event_manager::~Event_manager()
{
}


void 
Event_manager::execute_handlers(Window::Keystroke keystroke)
{
  #ifdef P7
    assert(false && "make sure you implement this for p7");
  #endif
}
