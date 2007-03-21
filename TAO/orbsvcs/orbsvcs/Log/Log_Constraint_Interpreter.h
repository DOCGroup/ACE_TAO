// -*- C++ -*-

//=============================================================================
/**
 *  @file   Log_Constraint_Interpreter.h
 *
 *  $Id$
 *
 *
 *
 *
 *  @author Pradeep Gore <pradeep@cs.wustl.edu>
 *  @author D A Hanvey <d.hanvey@qub.ac.uk>
 *
 */
//=============================================================================

#ifndef TAO_LOG_CONSTRAINT_INTERPRETER_H
#define TAO_LOG_CONSTRAINT_INTERPRETER_H

#include /**/ "ace/pre.h"
#include "orbsvcs/ETCL/ETCL_Interpreter.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/ETCL/ETCL_Constraint.h"
#include "orbsvcs/DsLogAdminC.h"
#include "orbsvcs/Log/log_serv_export.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_Log_Constraint_Visitor;

/**
 * @class TAO_Log_Constraint_Interpreter
 *
 * @brief "ETCL" Interpreter for the Log queries.
 */
class TAO_Log_Serv_Export TAO_Log_Constraint_Interpreter :
  public TAO_ETCL_Interpreter
{
public:

  // = Initialization and termination methods.

  /**
   * This constructor builds an expression tree representing the
   * constraint specified in <constraints>, and throws an Illegal
   * Constraint exception if the constraint given has syntax errors or
   * semantic errors, such as mismatched types.
   */
  TAO_Log_Constraint_Interpreter (const char* constraints);

  /// Destructor.
  ~TAO_Log_Constraint_Interpreter (void);

  /// Returns true if the constraint is evaluated successfully by
  /// the evaluator.
  CORBA::Boolean evaluate (TAO_Log_Constraint_Visitor &evaluator);
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /* TAO_LOG_CONSTRAINT_INTERPRETER_H */
