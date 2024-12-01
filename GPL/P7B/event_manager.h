/***

Overview:

    class Event_manager stores all the event handlers.

    It is a singleton.

    When an event handler is declared in a gpl program its
    statement_block should be inserted into the Event_manager using:

      void register_handler(Window::Keystroke keystroke,
                 Statement_block *statement_block
                );

    When class Window receives an event from the windowing system (via GLUT)
    it calls:
    
        void execute_handlers(Window::Keystroke keystroke);

    This function will execute all the statement_blocks registered with
    that keystroke.

How to use:

    When a event handler is parsed (in gpl.y) pass the statement_block
    and the keystroke to register_handler()

    Class Window already calls execute_handlers(), you don't have to.
    
***/

#ifndef EVENT_MANAGER_H
#define EVENT_MANAGER_H

#include <iostream>
#include <vector>

#include "window.h" // for Keystroke enum

class Statement_block;

class Event_manager
{
  public:

    static Event_manager *instance();

    void register_handler(Window::Keystroke keystroke,
                          Statement_block *statement_block
                         );

    void execute_handlers(Window::Keystroke keystroke);

    std::ostream &print(std::ostream &os) const;

  private:
    // hide default constructor because this is a singleton
    Event_manager();

    static Event_manager *m_instance;

    // an array of vectors indexed by the keystroke
    // execute_handlers(keystroke) will execute all block in
    // m_blocks[keystroke]
    std::vector<Statement_block *> m_blocks[Window::NUMBER_OF_KEYS];

    // disable default copy constructor and default assignment
    // done as a precaution, they should never be called
    Event_manager(const Event_manager&);
    const Event_manager &operator=(const Event_manager&);
};

std::ostream & operator<<(std::ostream &os, const Event_manager *event_manager);

std::ostream & operator<<(std::ostream &os, const Event_manager &event_manager);

#endif // #ifndef EVENT_MANAGER_H
