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

#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */

class TAO_Notify_Export TAO_Notify_Filter_i : public POA_CosNotifyFilter::Filter,
  public PortableServer::RefCountServantBase
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
  get_ref (TAO_ENV_SINGLE_ARG_DECL);
  // Activate with the default POA

virtual char * constraint_grammar (
    TAO_ENV_SINGLE_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

virtual CosNotifyFilter::ConstraintInfoSeq * add_constraints (
    const CosNotifyFilter::ConstraintExpSeq & constraint_list
    TAO_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosNotifyFilter::InvalidConstraint
  ));

virtual void modify_constraints (
    const CosNotifyFilter::ConstraintIDSeq & del_list,
    const CosNotifyFilter::ConstraintInfoSeq & modify_list
    TAO_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosNotifyFilter::InvalidConstraint,
    CosNotifyFilter::ConstraintNotFound
  ));

virtual CosNotifyFilter::ConstraintInfoSeq * get_constraints (
    const CosNotifyFilter::ConstraintIDSeq & id_list
    TAO_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosNotifyFilter::ConstraintNotFound
  ));

virtual CosNotifyFilter::ConstraintInfoSeq * get_all_constraints (
    TAO_ENV_SINGLE_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

virtual void remove_all_constraints (
    TAO_ENV_SINGLE_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

virtual void destroy (
    TAO_ENV_SINGLE_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

virtual CORBA::Boolean match (
    const CORBA::Any & filterable_data
    TAO_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosNotifyFilter::UnsupportedFilterableData
  ));

virtual CORBA::Boolean match_structured (
    const CosNotification::StructuredEvent & filterable_data
    TAO_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosNotifyFilter::UnsupportedFilterableData
  ));

virtual CORBA::Boolean match_typed (
    const CosNotification::PropertySeq & filterable_data
    TAO_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosNotifyFilter::UnsupportedFilterableData
  ));

virtual CosNotifyFilter::CallbackID attach_callback (
    CosNotifyComm::NotifySubscribe_ptr callback
    TAO_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

virtual void detach_callback (
    CosNotifyFilter::CallbackID callback
    TAO_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosNotifyFilter::CallbackNotFound
  ));

virtual CosNotifyFilter::CallbackIDSeq * get_callbacks (
    TAO_ENV_SINGLE_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));


private:
  void add_constraints_i (
    const CosNotifyFilter::ConstraintInfoSeq& constraint_info_seq
    TAO_ENV_ARG_DECL
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
                        TAO_SYNCH_MUTEX>
  constraint_expr_list_;
  // A list of the constraints stored in this filter.

  typedef ACE_Hash_Map_Iterator <CosNotifyFilter::ConstraintID,
                                 ACE_NESTED_CLASS (TAO_Notify_Filter_i, Notify_Constraint_Expr*),
                                                  TAO_SYNCH_MUTEX>
  CONSTRAINT_EXPR_LIST_ITER;

  typedef ACE_Hash_Map_Entry<CosNotifyFilter::ConstraintID,
                             ACE_NESTED_CLASS (TAO_Notify_Filter_i, Notify_Constraint_Expr*)>
  CONSTRAINT_EXPR_ENTRY;
};

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#include "ace/post.h"
#endif /* TAO_NOTIFY_FILTER_I_H */
