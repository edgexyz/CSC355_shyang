/*
  Pure virtual base class for actual statement classes

  Classes that inherit class Statement must override two functions:

      void execute() should contain the code that implements the statement

  gpl contains three types of statements:

    If_statement
    Assign_sttement
    Statement_block   // a vector of statements

*/
#ifndef STATEMENT_H
#define STATEMENT_H

class Statement
{
  public:
    // Statement() {}
    // virtual ~Statement() {}
    virtual void execute() = 0;
};

#endif // ifndef STATEMENT_H
