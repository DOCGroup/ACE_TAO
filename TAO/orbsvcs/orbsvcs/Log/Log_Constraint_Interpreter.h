/* -*- C++ -*- */
// $Id$

// ==========================================================================
//
// = LIBRARY
//   orbsvcs
//
// = FILENAME
//   Log_Constraint_Interpreter.h
//
// = DESCRIPTION
//
//
// = AUTHOR
//   Pradeep Gore <pradeep@cs.wustl.edu> 
//   D A Hanvey <d.hanvey@qub.ac.uk>
//
// ==========================================================================

#ifndef TAO_LOG_CONSTRAINT_INTERPRETER_H
#define TAO_LOG_CONSTRAINT_INTERPRETER_H

#include "ace/pre.h"
#include "orbsvcs/ETCL/ETCL_Interpreter.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/ETCL/ETCL_Constraint.h"
#include "orbsvcs/DsLogAdminC.h"
#include "log_export.h"

class TAO_Log_Constraint_Visitor;

class TAO_Log_Export TAO_Log_Constraint_Interpreter
  : public TAO_ETCL_Interpreter
{
  //
  // = TITLE
  //   TAO_Log_Constraint_Interpreter
  //
  // = DESCRIPTION
  //   "ETCL" Interpreter for the Log queries.
public:
  // = Initialization and termination methods.
  TAO_Log_Constraint_Interpreter (const char* constraints
                   ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((DsLogAdmin::InvalidConstraint,
                     CORBA::NO_MEMORY));
  // This constructor builds an expression tree representing the
  // constraint specified in <constraints>, and throws an Illegal
  // Constraint exception if the constraint given has syntax errors or
  // semantic errors, such as mismatched types.

  ~TAO_Log_Constraint_Interpreter (void);
  // Destructor

  CORBA::Boolean evaluate (TAO_Log_Constraint_Visitor &evaluator);
  // Returns true if the constraint is evaluated successfully by
  // the evaluator.
};
#include "ace/post.h"
#endif /* TAO_LOG_CONSTRAINT_INTERPRETER_H */
