/* -*- C++ -*- */
/**
 *  @file ETCL_Filter.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 *
 *
 */

#ifndef TAO_NS_ETCL_FILTER_H
#define TAO_NS_ETCL_FILTER_H
#include "ace/pre.h"

#include "notify_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Containers_T.h"
#include "ace/Hash_Map_Manager.h"
#include "ace/Atomic_Op.h"
#include "orbsvcs/CosNotifyFilterS.h"
#include "Notify_Constraint_Interpreter.h"

#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */

/**
 * @class TAO_ETCL_Filter
 *
 * @brief Implementation of CosNotifyFilter::Filter servant.
 *
 */
class TAO_Notify_Export TAO_NS_ETCL_Filter : public POA_CosNotifyFilter::Filter, public PortableServer::RefCountServantBase
{
public:
  /// Constuctor
  TAO_NS_ETCL_Filter (void);

  /// Destructor
  ~TAO_NS_ETCL_Filter ();

protected:
  virtual char * constraint_grammar (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     ));

  virtual CosNotifyFilter::ConstraintInfoSeq * add_constraints (const CosNotifyFilter::ConstraintExpSeq & constraint_list ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((
                     CORBA::SystemException,
                     CosNotifyFilter::InvalidConstraint
                     ));

  virtual void modify_constraints (const CosNotifyFilter::ConstraintIDSeq & del_list,
                                   const CosNotifyFilter::ConstraintInfoSeq & modify_list
                                   ACE_ENV_ARG_DECL
                                   )
    ACE_THROW_SPEC ((
                     CORBA::SystemException,
                     CosNotifyFilter::InvalidConstraint,
                     CosNotifyFilter::ConstraintNotFound
                     ));

  virtual CosNotifyFilter::ConstraintInfoSeq * get_constraints (const CosNotifyFilter::ConstraintIDSeq & id_list ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((
                     CORBA::SystemException,
                     CosNotifyFilter::ConstraintNotFound
                     ));

  virtual CosNotifyFilter::ConstraintInfoSeq * get_all_constraints (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     ));

  virtual void remove_all_constraints (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     ));

  virtual void destroy (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     ));

  virtual CORBA::Boolean match (const CORBA::Any & filterable_data ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((
                     CORBA::SystemException,
                     CosNotifyFilter::UnsupportedFilterableData
                     ));

  virtual CORBA::Boolean match_structured (const CosNotification::StructuredEvent & filterable_data ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((
                     CORBA::SystemException,
                     CosNotifyFilter::UnsupportedFilterableData
                     ));

  virtual CORBA::Boolean match_typed (const CosNotification::PropertySeq & filterable_data ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((
                     CORBA::SystemException,
                     CosNotifyFilter::UnsupportedFilterableData
                     ));

  virtual CosNotifyFilter::CallbackID attach_callback (CosNotifyComm::NotifySubscribe_ptr callback ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     ));

  virtual void detach_callback (CosNotifyFilter::CallbackID callback ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((
                     CORBA::SystemException,
                     CosNotifyFilter::CallbackNotFound
                     ));

  virtual CosNotifyFilter::CallbackIDSeq * get_callbacks (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     ));

private:
  void add_constraints_i (const CosNotifyFilter::ConstraintInfoSeq& constraint_info_seq ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((
                     CORBA::SystemException,
                     CosNotifyFilter::InvalidConstraint
                     ));

  void remove_all_constraints_i (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     ));

  struct TAO_NS_Constraint_Expr
  {
    // = DESCRIPTION
    //   Structure for associating ConstraintInfo with an interpreter.
    //
    CosNotifyFilter::ConstraintExp constr_expr;
    // Constraint Expression.

    TAO_NS_Constraint_Interpreter interpreter;
    // Constraint Interpreter.
  };

  /// Lock to serialize access to data members.
  TAO_SYNCH_MUTEX lock_;

  /// Id generator for ConstraintInfo's.
  CORBA::Long constraint_expr_ids_;

  /// A list of the constraints stored in this filter.
  typedef ACE_Hash_Map_Manager <CosNotifyFilter::ConstraintID,
                                ACE_NESTED_CLASS (TAO_NS_ETCL_Filter, TAO_NS_Constraint_Expr*),
                                ACE_SYNCH_NULL_MUTEX>
  CONSTRAINT_EXPR_LIST;

  CONSTRAINT_EXPR_LIST constraint_expr_list_;
};

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#if defined (__ACE_INLINE__)
#include "ETCL_Filter.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_ETCL_FILTER_H */
