//=============================================================================
/**
 *  @file   Notify_Filter_i.h
 *
 *  $Id$
 *
 *
 *
 *
 *  @author Pradeep Gore <pradeep@cs.wustl.edu>
 */
//=============================================================================


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

/**
 * @class TAO_Notify_Filter_i
 *
 */

class TAO_Notify_Export TAO_Notify_Filter_i : public POA_CosNotifyFilter::Filter,
  public PortableServer::RefCountServantBase
{
public:
  /// Constructor
  TAO_Notify_Filter_i (const char* constraint_grammar);

  /// Destructor
  virtual ~TAO_Notify_Filter_i (void);

  /// Activate with the default POA
  CosNotifyFilter::Filter_ptr
  get_ref (ACE_ENV_SINGLE_ARG_DECL);

virtual char * constraint_grammar (
    ACE_ENV_SINGLE_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

virtual CosNotifyFilter::ConstraintInfoSeq * add_constraints (
    const CosNotifyFilter::ConstraintExpSeq & constraint_list
    ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosNotifyFilter::InvalidConstraint
  ));

virtual void modify_constraints (
    const CosNotifyFilter::ConstraintIDSeq & del_list,
    const CosNotifyFilter::ConstraintInfoSeq & modify_list
    ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosNotifyFilter::InvalidConstraint,
    CosNotifyFilter::ConstraintNotFound
  ));

virtual CosNotifyFilter::ConstraintInfoSeq * get_constraints (
    const CosNotifyFilter::ConstraintIDSeq & id_list
    ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosNotifyFilter::ConstraintNotFound
  ));

virtual CosNotifyFilter::ConstraintInfoSeq * get_all_constraints (
    ACE_ENV_SINGLE_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

virtual void remove_all_constraints (
    ACE_ENV_SINGLE_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

virtual void destroy (
    ACE_ENV_SINGLE_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

virtual CORBA::Boolean match (
    const CORBA::Any & filterable_data
    ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosNotifyFilter::UnsupportedFilterableData
  ));

virtual CORBA::Boolean match_structured (
    const CosNotification::StructuredEvent & filterable_data
    ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosNotifyFilter::UnsupportedFilterableData
  ));

virtual CORBA::Boolean match_typed (
    const CosNotification::PropertySeq & filterable_data
    ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosNotifyFilter::UnsupportedFilterableData
  ));

virtual CosNotifyFilter::CallbackID attach_callback (
    CosNotifyComm::NotifySubscribe_ptr callback
    ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

virtual void detach_callback (
    CosNotifyFilter::CallbackID callback
    ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosNotifyFilter::CallbackNotFound
  ));

virtual CosNotifyFilter::CallbackIDSeq * get_callbacks (
    ACE_ENV_SINGLE_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

private:
  void add_constraints_i (
      const CosNotifyFilter::ConstraintInfoSeq& constraint_info_seq
      ACE_ENV_ARG_DECL
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

  /// Id generator for ConstraintInfo's.
  TAO_Notify_ID_Pool <CosNotifyFilter::ConstraintID> constraint_expr_ids_;

  /// A list of the constraints stored in this filter.
  ACE_Hash_Map_Manager <CosNotifyFilter::ConstraintID,
                        ACE_NESTED_CLASS (TAO_Notify_Filter_i, Notify_Constraint_Expr*),
                        TAO_SYNCH_MUTEX>
  constraint_expr_list_;

  typedef ACE_Hash_Map_Iterator <CosNotifyFilter::ConstraintID,
                                 ACE_NESTED_CLASS (TAO_Notify_Filter_i, Notify_Constraint_Expr*),
                                 TAO_SYNCH_MUTEX>
  CONSTRAINT_EXPR_LIST_ITER;

  typedef ACE_Hash_Map_Entry <CosNotifyFilter::ConstraintID,
                              ACE_NESTED_CLASS (TAO_Notify_Filter_i,
                                                Notify_Constraint_Expr*)>
  CONSTRAINT_EXPR_ENTRY;
  CORBA::String_var constraint_grammar_;
};

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#include "ace/post.h"
#endif /* TAO_NOTIFY_FILTER_I_H */
