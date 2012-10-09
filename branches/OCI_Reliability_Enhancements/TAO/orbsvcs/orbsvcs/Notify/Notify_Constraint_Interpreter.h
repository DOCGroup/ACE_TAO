// -*- C++ -*-

//=============================================================================
/**
 *  @file   Notify_Constraint_Interpreter.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@cs.wustl.edu>
 */
//=============================================================================

#ifndef TAO_NOTIFY_CONSTRAINT_INTERPRETER_H
#define TAO_NOTIFY_CONSTRAINT_INTERPRETER_H

#include /**/ "ace/pre.h"

#include "ace/ETCL/ETCL_Interpreter.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/ETCL/TAO_ETCL_Constraint.h"

#include "orbsvcs/CosNotifyFilterC.h"
#include "orbsvcs/Notify/notify_serv_export.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_Notify_Constraint_Visitor;

/**
 * @class TAO_Notify_Constraint_Interpreter
 *
 * @brief "ETCL" Interpreter for the Notify queries.
 */
class TAO_Notify_Serv_Export TAO_Notify_Constraint_Interpreter
  : public ETCL_Interpreter
{
public:
  // = Initialization and termination methods.
  TAO_Notify_Constraint_Interpreter (void);

  /// Destructor
  virtual ~TAO_Notify_Constraint_Interpreter (void);

  /**
   * This method builds an expression tree representing the
   * constraint specified in @a constraints, and throws an Illegal
   * Constraint exception if the constraint given has syntax errors or
   * semantic errors, such as mismatched types.
   */
  void build_tree (const CosNotifyFilter::ConstraintExp& exp);

  /// Returns true if the constraint is evaluated successfully by
  /// the evaluator.
  CORBA::Boolean evaluate (TAO_Notify_Constraint_Visitor &evaluator);

private:
  void build_tree (const char* constraints);
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /* TAO_NOTIFY_CONSTRAINT_INTERPRETER_H */
