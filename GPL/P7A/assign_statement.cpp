#include "assign_statement.h"
#include "expression.h"
#include "variable.h"
#include "gpl_assert.h"
using namespace std;

Assign_statement::Assign_statement(Assign_operator op,
                                   Variable *lhs,
                                   Expression *rhs
                                   )
{
  assert(lhs);
  assert(rhs);

  // all the type checking is done in gpl.y
  // so any error found here is a logic error and not an input error
  // make sure it is correct
  Gpl_type lhs_type = lhs->get_type();
  Gpl_type rhs_type = rhs->get_type();

  // Game_objects can never been in an assignment
  assert(!(lhs_type & GAME_OBJECT));
  assert(rhs_type != GAME_OBJECT);

  if (lhs_type & INT)
    assert(rhs_type == INT);
  else if (lhs_type & DOUBLE)
    assert(rhs_type == INT || rhs_type == DOUBLE);
  else if (lhs_type & STRING)
  {
    assert(rhs_type != ANIMATION_BLOCK);
    assert(op == ASSIGN || op == PLUS_ASSIGN);
  }
  else if (lhs_type & ANIMATION_BLOCK)
  {
    assert(rhs_type == ANIMATION_BLOCK);
    assert(op == ASSIGN);
  }

  m_operator = op;
  m_lhs = lhs;
  m_rhs = rhs;
}

Assign_statement::Assign_statement(Assign_operator op,
                                   Variable *lhs
                                   )
{
  assert(lhs);

  // all the type checking is done in gpl.y
  // so any error found here is a logic error and not an input error
  // make sure it is correct
  Gpl_type lhs_type = lhs->get_type();

  // Game_objects can never been in an assignment
  assert(lhs_type == INT);

  m_operator = op;
  m_lhs = lhs;
  m_rhs = NULL;
}

/* virtual */ void
Assign_statement::execute()
{
  // if this is ++ or --
  if (m_rhs == NULL)
  {
    switch (m_operator)
    {
      case PLUS_PLUS:
      {
        m_lhs->set(m_lhs->get_int_value() + 1);
        break;
      }
      case MINUS_MINUS:
      {
        m_lhs->set(m_lhs->get_int_value() - 1);
        break;
      }
      default: assert(0);
    }
  }
  else // this is NOT ++ or --
  {
    Gpl_type lhs_type = m_lhs->get_type();
    Gpl_type rhs_type = m_rhs->get_type();
  
    assert(!(lhs_type & GAME_OBJECT));
    assert(rhs_type != GAME_OBJECT);
  
    // all assignments are variable operator expression (m_lhs m_operator m_rhs)
    // (in a previous version there were some special case hacks here)
  
    if (lhs_type & INT)
    {
      assert(rhs_type == INT);
      switch (m_operator)
      {
        case ASSIGN:
          m_lhs->set(m_rhs->eval_int());
          break;
        case PLUS_ASSIGN:
          m_lhs->set(m_lhs->get_int_value() + m_rhs->eval_int());
          break;
        case MINUS_ASSIGN:
          m_lhs->set(m_lhs->get_int_value() - m_rhs->eval_int());
          break;
        default: assert(0);
     }
    }
    else if (lhs_type & DOUBLE)
    {
      assert(rhs_type == INT || rhs_type == DOUBLE);
      // INT can be assigned to DOUBLE
      // when an INT expression is evaluated as a doulbe (eval_double())
      // the result is an INT that has be cast to a DOUBLE
      switch (m_operator)
      {
        case ASSIGN:
          m_lhs->set(m_rhs->eval_double());
          break;
        case PLUS_ASSIGN:
          m_lhs->set(m_lhs->get_double_value() + m_rhs->eval_double());
          break;
        case MINUS_ASSIGN:
          m_lhs->set(m_lhs->get_double_value() - m_rhs->eval_double());
          break;
        default: assert(0);
     }
    }
    else if (lhs_type & STRING)
    {
      switch (m_operator)
      {
        case ASSIGN:
        {
          assert(rhs_type==STRING || rhs_type==INT || rhs_type == DOUBLE);
  
          // when an INT or a DOUBLE expression are evaluated as a string
          // (using Expression::eval_string()) numeric values converted to string
          m_lhs->set(m_rhs->eval_string());
          break;
        }
        case PLUS_ASSIGN:
        {
          m_lhs->set(m_lhs->get_string_value() + m_rhs->eval_string());
  
          break;
        }
        default: assert(0); // no other assign operators work w/STRING
     }
    }
    else if (lhs_type & ANIMATION_BLOCK)
    {
      assert(rhs_type==ANIMATION_BLOCK);
      assert(m_operator == ASSIGN);
      m_lhs->set(m_rhs->eval_animation_block());
    }
    else assert(0);
  }
}
