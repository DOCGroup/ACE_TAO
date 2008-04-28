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

#ifndef TAO_Notify_Tests_RT_Test_FILTER_H
#define TAO_Notify_Tests_RT_Test_FILTER_H
#include /**/ "ace/pre.h"

#include "rt_test_filter_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/CosNotifyFilterS.h"

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4250)
#endif /* _MSC_VER */

/**
 * @class TAO_RT_Test_Filter
 *
 * @brief Implementation of CosNotifyFilter::Filter servant.
 *
 */
class TAO_RT_Test_Filter_Export TAO_Notify_Tests_RT_Test_Filter
  : public POA_CosNotifyFilter::Filter
{
public:
  /// Constructor
  TAO_Notify_Tests_RT_Test_Filter (void);

  /// Destructor
  ~TAO_Notify_Tests_RT_Test_Filter ();

protected:
  virtual char * constraint_grammar (void);

  virtual CosNotifyFilter::ConstraintInfoSeq * add_constraints (const CosNotifyFilter::ConstraintExpSeq & constraint_list);

  virtual void modify_constraints (const CosNotifyFilter::ConstraintIDSeq & del_list,
                                   const CosNotifyFilter::ConstraintInfoSeq & modify_list);

  virtual CosNotifyFilter::ConstraintInfoSeq * get_constraints (const CosNotifyFilter::ConstraintIDSeq & id_list);

  virtual CosNotifyFilter::ConstraintInfoSeq * get_all_constraints (void);

  virtual void remove_all_constraints (void);

  virtual void destroy (void);

  virtual CORBA::Boolean match (const CORBA::Any & filterable_data);

  virtual CORBA::Boolean match_structured (const CosNotification::StructuredEvent & filterable_data);

  virtual CORBA::Boolean match_typed (const CosNotification::PropertySeq & filterable_data);

  virtual CosNotifyFilter::CallbackID attach_callback (CosNotifyComm::NotifySubscribe_ptr callback);

  virtual void detach_callback (CosNotifyFilter::CallbackID callback);

  virtual CosNotifyFilter::CallbackIDSeq * get_callbacks (void);

protected:

  /// Lock to serialize access to data members.
  TAO_SYNCH_MUTEX lock_;

  CORBA::ULong expected_pool_id_;
  CORBA::ULong expected_lane_id_;
};

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#include /**/ "ace/post.h"
#endif /* TAO_RT_Test_FILTER_H */
