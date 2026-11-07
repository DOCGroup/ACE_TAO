/* -*- C++ -*- */
/**
 *  @file RT_Priority_Mapping.h
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 */

#ifndef TAO_Notify_Tests_RT_PRIORITY_MAPPING_H
#define TAO_Notify_Tests_RT_PRIORITY_MAPPING_H
#include /**/ "ace/pre.h"

#include "rt_notify_test_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "../lib/Priority_Mapping.h"
#include "tao/RTCORBA/Priority_Mapping_Manager.h"

/**
 * @class TAO_Notify_Tests_RT_Priority_Mapping
 *
 * @brief Delegates to a RTCORBA::PriorityMapping
 */
class TAO_RT_NOTIFY_TEST_Export TAO_Notify_Tests_RT_Priority_Mapping : public TAO_Notify_Tests_Priority_Mapping
{
public:
  TAO_Notify_Tests_RT_Priority_Mapping () = default;

  /// The destructor
  ~TAO_Notify_Tests_RT_Priority_Mapping () override = default;

  TAO_Notify_Tests_RT_Priority_Mapping (TAO_Notify_Tests_RT_Priority_Mapping const&) = delete;
  TAO_Notify_Tests_RT_Priority_Mapping (TAO_Notify_Tests_RT_Priority_Mapping&&) = delete;
  TAO_Notify_Tests_RT_Priority_Mapping& operator= (TAO_Notify_Tests_RT_Priority_Mapping const&) = delete;
  TAO_Notify_Tests_RT_Priority_Mapping& operator= (TAO_Notify_Tests_RT_Priority_Mapping&&) = delete;

  /// Init the Mapping manager.
  void init (CORBA::ORB_ptr orb);

  CORBA::Boolean to_native (CORBA::Short corba_priority,
                            CORBA::Short& native_priority) override;
  CORBA::Boolean to_CORBA (CORBA::Short native_priority,
                           CORBA::Short& corba_priority) override;

protected:
  /// The Priority Mapping helper.
  // Borrowed from PriorityMappingManager, which owns the mapping.
  RTCORBA::PriorityMapping* priority_mapping_ {};
};

#include /**/ "ace/post.h"
#endif /* TAO_Notify_Tests_RT_PRIORITY_MAPPING_H */
