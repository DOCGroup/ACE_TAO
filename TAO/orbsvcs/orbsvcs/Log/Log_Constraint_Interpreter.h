/* -*- C++ -*- */
// $Id$

// ========================================================================
//
// = LIBRARY
//    orbsvcs
//
// = FILENAME
//    Log_Constraint_Interpreter.h
//
// = AUTHOR
//    Pradeep Gore <pradeep@cs.wustl.edu>
//
// ========================================================================

#ifndef TAO_LOG_CONSTRAINT_INTERPRETER_H
#define TAO_LOG_CONSTRAINT_INTERPRETER_H
#include "ace/pre.h"

#include "orbsvcs/orbsvcs/Trader/Constraint_Nodes.h"
#include "orbsvcs/orbsvcs/Trader/Constraint_Visitors.h"
#include "orbsvcs/orbsvcs/Trader/Interpreter.h"
#include "orbsvcs/orbsvcs/DsLogAdminC.h"
#include "log_export.h"

class TAO_Constraint_Evaluator;

class TAO_Log_Export TAO_Log_Constraint_Interpreter : public TAO_Interpreter
{
  //
  // = TITLE
  //   TAO_Log_Constraint_Interpreter
  //
  // = DESCRIPTION
  //   "TCL" Interpreter for the Log queries.
public:
  // = Initialization and termination methods.
  TAO_Log_Constraint_Interpreter (const char* constraints
                              TAO_ENV_ARG_DECL)
    ACE_THROW_SPEC ((DsLogAdmin::InvalidConstraint,
                     CORBA::NO_MEMORY));
  // This constructor builds an expression tree representing the
  // constraint specified in <constraints>, and throws an Illegal
  // Constraint exception if the constraint given has syntax errors or
  // semantic errors, such as mismatched types.

  ~TAO_Log_Constraint_Interpreter (void);
  // Destructor

  CORBA::Boolean evaluate (TAO_Constraint_Evaluator& evaluator);
  // Returns true if the constraint is evaluated successfully by
  // the evaluator.
};
#include "ace/post.h"
#endif /* TAO_LOG_CONSTRAINT_INTERPRETER_H */
