// -*- C++ -*-

//=============================================================================
/**
 *  @file   Log_Constraint_Visitors.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@cs.wustl.edu>
 *  @author Jeff Parsons <parsons@cs.wustl.edu>
 *  @author D A Hanvey <d.hanvey@qub.ac.uk>
 */
//=============================================================================


#ifndef TAO_LOG_CONSTRAINT_VISITORS_H
#define TAO_LOG_CONSTRAINT_VISITORS_H

#include /**/ "ace/pre.h"

#include "ace/Hash_Map_Manager.h"
#include "ace/Unbounded_Queue.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/ETCL/ETCL_Constraint_Visitor.h"
#include "ace/Null_Mutex.h"

#include "tao/ETCL/TAO_ETCL_Constraint.h"

#include "orbsvcs/DsLogAdminC.h"

#include "orbsvcs/Log/log_serv_export.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_ETCL_Constraint;
class TAO_ETCL_Literal_Constraint;
class TAO_Log_Property_Constraint;

/**
 * @class TAO_Log_Constraint_Visitor
 *
 * @brief "ETCL" Visitor for the Log queries.
 */
class TAO_Log_Serv_Export TAO_Log_Constraint_Visitor :
  public ETCL_Constraint_Visitor
{
public:

  /// Constructor.
  TAO_Log_Constraint_Visitor (const DsLogAdmin::LogRecord &rec);

  /**
   * Returns 1 if the offer satisfies the constraint
   * represented by the the expression tree rooted at <root>, 0 if it
   * doesn't. If an error occurs during process, the traversal
   * automatically fails.
   */
  CORBA::Boolean evaluate_constraint (ETCL_Constraint *root);

  // = The overridden methods.
  int visit_literal (TAO_ETCL_Literal_Constraint *);
  int visit_identifier (TAO_ETCL_Identifier *);
  int visit_union_value (TAO_ETCL_Union_Value *);
  int visit_union_pos (TAO_ETCL_Union_Pos *);
  int visit_component_pos (TAO_ETCL_Component_Pos *);
  int visit_component_assoc (TAO_ETCL_Component_Assoc *);
  int visit_component_array (TAO_ETCL_Component_Array *);
  int visit_special (TAO_ETCL_Special *);
  int visit_component (TAO_ETCL_Component *);
  int visit_dot (TAO_ETCL_Dot *);
  int visit_eval (TAO_ETCL_Eval *);
  int visit_default (TAO_ETCL_Default *);
  int visit_exist (TAO_ETCL_Exist *);
  int visit_unary_expr (TAO_ETCL_Unary_Expr *);
  int visit_binary_expr (TAO_ETCL_Binary_Expr *);
  int visit_preference (TAO_ETCL_Preference *);

private:
  // = Sub-methods for visit_binary_expr().
  int visit_or (TAO_ETCL_Binary_Expr *);
  int visit_and (TAO_ETCL_Binary_Expr *);
  int visit_twiddle (TAO_ETCL_Binary_Expr *);
  int visit_in (TAO_ETCL_Binary_Expr *);
  int visit_binary_op (TAO_ETCL_Binary_Expr *binary_expr,
                       int op_type);

  // = These use dynamic anys look inside the ETCL component.
  CORBA::Boolean sequence_does_contain (const CORBA::Any *any,
                                        TAO_ETCL_Literal_Constraint &item);
  CORBA::Boolean array_does_contain (const CORBA::Any *any,
                                     TAO_ETCL_Literal_Constraint &item);
  CORBA::Boolean struct_does_contain (const CORBA::Any *any,
                                      TAO_ETCL_Literal_Constraint &item);
  CORBA::Boolean union_does_contain (const CORBA::Any *any,
                                     TAO_ETCL_Literal_Constraint &item);
  CORBA::Boolean any_does_contain (const CORBA::Any *any,
                                   TAO_ETCL_Literal_Constraint &item);

  /// Utility function to compare a TAO_ETCL_Literal_Constraint type
  /// and a type code.
  CORBA::Boolean simple_type_match (int expr_type,
                                    CORBA::TCKind tc_kind);

private:
  /// Size of property_lookup_ hash map.
  /// TODO: define inline once VC6 support is deprecated.
  static const size_t property_lookup_size_;

  typedef ACE_Hash_Map_Manager <ACE_CString,
                                CORBA::Any,
                                ACE_Null_Mutex> HASH_MAP;

  typedef HASH_MAP::ENTRY HASH_ENTRY;

  /// Used to lookup property name and values.
  HASH_MAP property_lookup_;

  /// The result of a non_boolean operation.
  ACE_Unbounded_Queue<TAO_ETCL_Literal_Constraint> queue_;

  /// Holder for a value found in property_lookup_ or for a
  /// nested type within that value.
  CORBA::Any_var current_member_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /* TAO_LOG_CONSTRAINT_VISITORS_H */
