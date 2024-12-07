// updated 2/14/2016

#include <vector>
#include "animation_block.h"
#include "game_object.h"
#include "gpl_assert.h"
#include "symbol.h"
using namespace std;

void Animation_block::initialize(Symbol *parameter_symbol, string name)
{
  m_name = name;
  // during error recovery a NULL parameter_symbol can be passed to Animation_block()
  if (parameter_symbol)
    assert(parameter_symbol->is_game_object());
  m_parameter_symbol = parameter_symbol;
}

void Animation_block::execute(Game_object *argument)
{
  Game_object *parameter_animation_block = m_parameter_symbol->get_game_object_value();
  m_parameter_symbol->set(argument);
  Statement_block::execute();
  m_parameter_symbol->set(parameter_animation_block);
}

