
#ifndef STATEMENT_BLOCK_H
#define STATEMENT_BLOCK_H

#include <vector>

class Symbol;
class Statement;

class Statement_block
{
  public:
    Statement_block();

    void insert(Statement *statement);

    bool valid() const {return m_cookie == m_global_cookie;}

    void execute();

  protected:
    // vector of all statements in the block
    std::vector<Statement *> m_statements;


    // cookies are used to check for valid types for when debugging code
    // they server no other purpose
    static const int m_global_cookie;
    int m_cookie;

    // disable default copy constructor and default assignment
    // done as a precaution, they should never be called
    Statement_block(const Statement_block &);
    const Statement_block &operator=(const Statement_block &);
};

#endif // #ifndef STATEMENT_BLOCK_H





