// -*- C++ -*-

//=============================================================================
/**
 *  @file   Notify_Constraint_Visitors.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@cs.wustl.edu>
 *  @author Jeff Parsons <parsons@cs.wustl.edu>
 */
//=============================================================================

#ifndef NOTIFY_CONSTRAINT_VISITORS_H
#define NOTIFY_CONSTRAINT_VISITORS_H

#include /**/ "ace/pre.h"

#include "ace/Hash_Map_Manager.h"
#include "ace/Unbounded_Queue.h"
#include "ace/Null_Mutex.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/ETCL/ETCL_Constraint_Visitor.h"

#include "tao/ETCL/TAO_ETCL_Constraint.h"

#include "orbsvcs/CosNotificationC.h"

#include "orbsvcs/Notify/notify_serv_export.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_Notify_Property_Constraint;

class TAO_Notify_Serv_Export TAO_Notify_Constraint_Visitor
  : public ETCL_Constraint_Visitor
{
public:
  /// Constructor.
  TAO_Notify_Constraint_Visitor (void);

  /// Put the event data into our hash map.
  int bind_structured_event (const CosNotification::StructuredEvent &s_event);

  /**
   * Returns true if the event satisfies the constraint
   * represented by the the expression tree rooted at @a root, false if it
   * doesn't. If an error occurs during the process, the traversal
   * automatically fails.
   */
  CORBA::Boolean evaluate_constraint (ETCL_Constraint *root);

  // The overridden methods.
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

protected:
  // Sub-methods for visit_binary_expr().
  int visit_or (TAO_ETCL_Binary_Expr *);
  int visit_and (TAO_ETCL_Binary_Expr *);
  int visit_twiddle (TAO_ETCL_Binary_Expr *);
  int visit_in (TAO_ETCL_Binary_Expr *);
  int visit_binary_op (TAO_ETCL_Binary_Expr *binary_expr,
                       int op_type);

  // These use dynamic anys to look inside the ETCL component.
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
  CORBA::Boolean simple_type_match (int expr_type, CORBA::TCKind tc_kind);

  enum structured_event_field
    {
      FILTERABLE_DATA,
      HEADER,
      FIXED_HEADER,
      EVENT_TYPE,
      DOMAIN_NAME,
      TYPE_NAME,
      EVENT_NAME,
      VARIABLE_HEADER,
      REMAINDER_OF_BODY,
      EMPTY
    };

  /// Storage for the type of implicit id the component has (if any).
  structured_event_field implicit_id_;

  /// Size of implicit_ids_ hash map.
  /// @note A fixed set of 9 keys are stored in this map.  In the absence
  /// of a minimal perfect hash, ACE's default hash_pjw() and a hash size
  /// of 27 ensures each element is hashed to a unique bucket.
  /// @todo define inline once VC6 support is deprecated.
  static const size_t implicit_ids_size_;

  /// Lookup table for the implicit ids, to avoid string comparisons in
  /// derived visitors.
  ACE_Hash_Map_Manager <ACE_CString, structured_event_field, ACE_Null_Mutex>
    implicit_ids_;

  /// Size of filterable_data_ hash map.
  /// @todo define inline once VC6 support is deprecated.
  static const size_t filterable_data_size_;

  /// Used to lookup names and values in the event's 'filterable_data' field.
  ACE_Hash_Map_Manager <ACE_CString, CORBA::Any, ACE_Null_Mutex>
    filterable_data_;

  /// Size of variable_header_ hash map.
  /// @todo define inline once VC6 support is deprecated.
  static const size_t variable_header_size_;

  /// Used to lookup names and values in the event's 'variable_header' field.
  ACE_Hash_Map_Manager <ACE_CString, CORBA::Any, ACE_Null_Mutex>
    variable_header_;

  /// Storage for string names under the structured event's
  /// 'fixed_header' field.
  CORBA::String_var domain_name_;
  CORBA::String_var type_name_;
  CORBA::String_var event_name_;

  /// Storage for the structured_event's 'remainder_of_body' field.
  CORBA::Any remainder_of_body_;

  /// The result of a non_boolean operation.
  ACE_Unbounded_Queue <TAO_ETCL_Literal_Constraint> queue_;

  /// Holder for a value found in the event fields filterable_data,
  /// variable_header or remainder_of_body.
  CORBA::Any_var current_value_;

  /// Holder for a string name in the event fields fixed_header,
  /// variable_header, or filterable_data.
  CORBA::String_var current_name_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /* NOTIFY_CONSTRAINT_VISITORS_H */
