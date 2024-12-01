#include "statement_block.h"
#include "statement.h"
#include "game_object.h"
#include "gpl_assert.h"
#include "symbol.h"

using namespace std; // for vector


/* static */ const int Statement_block::m_global_cookie = 4282309;

Statement_block::Statement_block()
{
  // create an empty Statement_block

  // use cookies during debugging to check for invalid pointers
  // there are a lot of pointers floating around through bison
  // without any real type checking
  m_cookie = m_global_cookie;
}

void Statement_block::insert(Statement *statement)
{
  m_statements.push_back(statement);
}

/* virtual */ void Statement_block::execute()
{
  vector<Statement *>::iterator iter;
  for (iter = m_statements.begin(); iter != m_statements.end(); iter++)
      (*iter)->execute();
}
