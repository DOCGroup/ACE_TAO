/* -*- C++ -*- */
/**
 *  @file RT_Test_Filter.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 *
 *
 */

#ifndef TAO_NS_RT_Test_FILTER_H
#define TAO_NS_RT_Test_FILTER_H
#include "ace/pre.h"

#include "rt_test_filter_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/CosNotifyFilterS.h"

#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */

/**
 * @class TAO_RT_Test_Filter
 *
 * @brief Implementation of CosNotifyFilter::Filter servant.
 *
 */
class TAO_RT_Test_Filter_Export TAO_NS_RT_Test_Filter : public POA_CosNotifyFilter::Filter, public PortableServer::RefCountServantBase
{
public:
  /// Constuctor
  TAO_NS_RT_Test_Filter (void);

  /// Destructor
  ~TAO_NS_RT_Test_Filter ();

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

protected:

  /// Lock to serialize access to data members.
  TAO_SYNCH_MUTEX lock_;

  CORBA::ULong expected_pool_id_;
  CORBA::ULong expected_lane_id_;
};

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#include "ace/post.h"
#endif /* TAO_RT_Test_FILTER_H */
