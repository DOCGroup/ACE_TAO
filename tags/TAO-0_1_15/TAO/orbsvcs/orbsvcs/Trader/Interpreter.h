// ============================================================================
// $Id$
//
// = LIBRARY
//    orbsvcs
// 
// = FILENAME
//    Interpreter.h
//
// = AUTHOR
//    Seth Widoff <sbw1@cs.wustl.edu>
// 
// ============================================================================


#ifndef TAO_INTERPRETER_H
#define TAO_INTERPRETER_H

#include "Constraint.h"
#include "Constraint_Nodes.h"

class TAO_Interpreter
// = TITLE
//      TAO_Interpreter is the superclass for all interpreters. It's
//      build tree method invoke the yacc parser to parse a constraint 
//      or preference string. 
{   
protected:

  TAO_Interpreter (void) : root_ (0) {}

  ~TAO_Interpreter (void);
  
  int build_tree (const char* preferences);
  // Using the Yacc generated parser, construct an expression
  // tree representing <constraints> from the tokens returned by it.

  static int is_empty_string (const char* str);
  
  TAO_Constraint* root_;
  // The root of the expression tree, not equal to null if build_tree
  // successfully builds a tree from the constraints.
};

#endif /* TAO_Interpreter */
