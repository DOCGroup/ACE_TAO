/* -*- C++ -*- */
/**
 *  @file RT_Test_Filter.h
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
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
 */
class TAO_RT_Test_Filter_Export TAO_Notify_Tests_RT_Test_Filter
  : public POA_CosNotifyFilter::Filter
{
public:
  /// Constructor
  TAO_Notify_Tests_RT_Test_Filter () = default;
  TAO_Notify_Tests_RT_Test_Filter (TAO_Notify_Tests_RT_Test_Filter const&) = delete;
  TAO_Notify_Tests_RT_Test_Filter (TAO_Notify_Tests_RT_Test_Filter&&) = delete;
  TAO_Notify_Tests_RT_Test_Filter& operator= (TAO_Notify_Tests_RT_Test_Filter const&) = delete;
  TAO_Notify_Tests_RT_Test_Filter& operator= (TAO_Notify_Tests_RT_Test_Filter&&) = delete;
  ~TAO_Notify_Tests_RT_Test_Filter () override = default;

protected:
  char* constraint_grammar () override;

  CosNotifyFilter::ConstraintInfoSeq* add_constraints (
    CosNotifyFilter::ConstraintExpSeq const& constraint_list) override;

  void modify_constraints (CosNotifyFilter::ConstraintIDSeq const& del_list,
                           CosNotifyFilter::ConstraintInfoSeq const& modify_list) override;

  CosNotifyFilter::ConstraintInfoSeq* get_constraints (
    CosNotifyFilter::ConstraintIDSeq const& id_list) override;

  CosNotifyFilter::ConstraintInfoSeq* get_all_constraints () override;

  void remove_all_constraints () override;

  void destroy () override;

  CORBA::Boolean match (CORBA::Any const& filterable_data) override;

  CORBA::Boolean match_structured (CosNotification::StructuredEvent const& filterable_data) override;

  CORBA::Boolean match_typed (CosNotification::PropertySeq const& filterable_data) override;

  CosNotifyFilter::CallbackID attach_callback (CosNotifyComm::NotifySubscribe_ptr callback) override;

  void detach_callback (CosNotifyFilter::CallbackID callback) override;

  CosNotifyFilter::CallbackIDSeq* get_callbacks () override;

protected:
  /// Lock to serialize access to data members.
  TAO_SYNCH_MUTEX lock_;

  CORBA::ULong expected_pool_id_ {};
  CORBA::ULong expected_lane_id_ {};
};

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#include /**/ "ace/post.h"
#endif /* TAO_RT_Test_FILTER_H */
