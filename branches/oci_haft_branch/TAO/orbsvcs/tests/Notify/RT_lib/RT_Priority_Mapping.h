/* -*- C++ -*- */
/**
 *  @file RT_Priority_Mapping.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 *
 *
 */

#ifndef TAO_Notify_Tests_RT_PRIORITY_MAPPING_H
#define TAO_Notify_Tests_RT_PRIORITY_MAPPING_H
#include "ace/pre.h"

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
 *
 */
class TAO_RT_NOTIFY_TEST_Export TAO_Notify_Tests_RT_Priority_Mapping : public TAO_Notify_Tests_Priority_Mapping
{
public:
  /// The destructor
  virtual ~TAO_Notify_Tests_RT_Priority_Mapping (void);

  /// Init the Mapping manager.
  void init (CORBA::ORB_ptr orb ACE_ENV_ARG_DECL);

  virtual CORBA::Boolean
      to_native (CORBA::Short corba_priority,
                 CORBA::Short &native_priority);
  virtual CORBA::Boolean
      to_CORBA (CORBA::Short native_priority,
                CORBA::Short &corba_priority);

protected:

  /// The Priority Mapping helper.
  RTCORBA::PriorityMapping *priority_mapping_;
};

#include "ace/post.h"
#endif /* TAO_Notify_Tests_RT_PRIORITY_MAPPING_H */
