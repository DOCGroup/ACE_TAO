// $Id$
// ==========================================================================
//
// = LIBRARY
//   orbsvcs
//
// = FILENAME
//   Notify_Filter_i.h
//
// = DESCRIPTION
//
//
// = AUTHOR
//    Pradeep Gore <pradeep@cs.wustl.edu>
//
// ==========================================================================

#ifndef TAO_NOTIFY_FILTER_I_H
#define TAO_NOTIFY_FILTER_I_H
#include "ace/pre.h"

#include "Notify_Constraint_Interpreter.h"
#include "Notify_ID_Pool_T.h"
#include "orbsvcs/CosNotifyFilterS.h"
#include "ace/Containers_T.h"
#include "ace/Hash_Map_Manager.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class TAO_Notify_Export TAO_Notify_Filter_i : public POA_CosNotifyFilter::Filter
{
  // = TITLE
  //   TAO_Notify_Filter_i
  // = DESCRIPTION
  //
  //

public:
  //Constructor
  TAO_Notify_Filter_i (void);

  //Destructor
  virtual ~TAO_Notify_Filter_i (void);

  CosNotifyFilter::Filter_ptr
  get_ref (CORBA::Environment &ACE_TRY_ENV);
  // Activate with the default POA

virtual char * constraint_grammar (
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

virtual CosNotifyFilter::ConstraintInfoSeq * add_constraints (
    const CosNotifyFilter::ConstraintExpSeq & constraint_list,
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosNotifyFilter::InvalidConstraint
  ));

virtual void modify_constraints (
    const CosNotifyFilter::ConstraintIDSeq & del_list,
    const CosNotifyFilter::ConstraintInfoSeq & modify_list,
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosNotifyFilter::InvalidConstraint,
    CosNotifyFilter::ConstraintNotFound
  ));

virtual CosNotifyFilter::ConstraintInfoSeq * get_constraints (
    const CosNotifyFilter::ConstraintIDSeq & id_list,
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosNotifyFilter::ConstraintNotFound
  ));

virtual CosNotifyFilter::ConstraintInfoSeq * get_all_constraints (
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

virtual void remove_all_constraints (
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

virtual void destroy (
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

virtual CORBA::Boolean match (
    const CORBA::Any & filterable_data,
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosNotifyFilter::UnsupportedFilterableData
  ));

virtual CORBA::Boolean match_structured (
    const CosNotification::StructuredEvent & filterable_data,
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosNotifyFilter::UnsupportedFilterableData
  ));

virtual CORBA::Boolean match_typed (
    const CosNotification::PropertySeq & filterable_data,
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosNotifyFilter::UnsupportedFilterableData
  ));

virtual CosNotifyFilter::CallbackID attach_callback (
    CosNotifyComm::NotifySubscribe_ptr callback,
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

virtual void detach_callback (
    CosNotifyFilter::CallbackID callback,
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosNotifyFilter::CallbackNotFound
  ));

virtual CosNotifyFilter::CallbackIDSeq * get_callbacks (
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));


private:
  void add_constraints_i (
    const CosNotifyFilter::ConstraintInfoSeq& constraint_info_seq,
    CORBA::Environment &ACE_TRY_ENV
  )
    ACE_THROW_SPEC ((
                     CORBA::SystemException,
                     CosNotifyFilter::InvalidConstraint
                     ));


    struct Notify_Constraint_Expr
    {
      // = DESCRIPTION
      //   Structure for associating ConstraintInfo with an interpreter.
      //
      CosNotifyFilter::ConstraintExp constr_expr;
      // Constraint Expression.

      TAO_Notify_Constraint_Interpreter interpreter;
      // Constraint Interpreter.
    };

  TAO_Notify_ID_Pool<CosNotifyFilter::ConstraintID> constraint_expr_ids_;
  // Id generator for ConstraintInfo's.

  ACE_Hash_Map_Manager <CosNotifyFilter::ConstraintID,
                        ACE_NESTED_CLASS (TAO_Notify_Filter_i, Notify_Constraint_Expr*),
                        ACE_SYNCH_MUTEX>
  constraint_expr_list_;
  // A list of the constraints stored in this filter.

  typedef ACE_Hash_Map_Iterator <CosNotifyFilter::ConstraintID,
                                 ACE_NESTED_CLASS (TAO_Notify_Filter_i, Notify_Constraint_Expr*),
                                                  ACE_SYNCH_MUTEX>
  CONSTRAINT_EXPR_LIST_ITER;

  typedef ACE_Hash_Map_Entry<CosNotifyFilter::ConstraintID,
                             ACE_NESTED_CLASS (TAO_Notify_Filter_i, Notify_Constraint_Expr*)>
  CONSTRAINT_EXPR_ENTRY;
};

#include "ace/post.h"
#endif /* TAO_NOTIFY_FILTER_I_H */
