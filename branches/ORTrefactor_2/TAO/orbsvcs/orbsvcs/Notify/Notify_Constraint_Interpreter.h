/* -*- C++ -*- */
//=============================================================================
/**
 *  @file   Notify_Constraint_Interpreter.h
 *
 *  $Id$
 *
 *
 *  @author Pradeep Gore <pradeep@cs.wustl.edu>
 */
//=============================================================================


#ifndef TAO_NOTIFY_CONSTRAINT_INTERPRETER_H
#define TAO_NOTIFY_CONSTRAINT_INTERPRETER_H

#include /**/ "ace/pre.h"
#include "orbsvcs/ETCL/ETCL_Interpreter.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/ETCL/ETCL_Constraint.h"
#include "orbsvcs/CosNotifyFilterC.h"
#include "notify_export.h"

class TAO_Notify_Constraint_Visitor;

/**
 * @class TAO_Notify_Constraint_Interpreter
 *
 * @brief "ETCL" Interpreter for the Notify queries.
 */
class TAO_Notify_Export TAO_Notify_Constraint_Interpreter : public TAO_ETCL_Interpreter
{
public:
  // = Initialization and termination methods.
  TAO_Notify_Constraint_Interpreter (void);

  /// Destructor
  ~TAO_Notify_Constraint_Interpreter (void);

  /**
   * This method builds an expression tree representing the
   * constraint specified in <constraints>, and throws an Illegal
   * Constraint exception if the constraint given has syntax errors or
   * semantic errors, such as mismatched types.
   */
  void build_tree (const char* constraints
                   ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CosNotifyFilter::InvalidConstraint,
                     CORBA::NO_MEMORY));

  /// Returns true if the constraint is evaluated successfully by
  /// the evaluator.
  CORBA::Boolean evaluate (TAO_Notify_Constraint_Visitor &evaluator);
};
#include /**/ "ace/post.h"
#endif /* TAO_NOTIFY_CONSTRAINT_INTERPRETER_H */
