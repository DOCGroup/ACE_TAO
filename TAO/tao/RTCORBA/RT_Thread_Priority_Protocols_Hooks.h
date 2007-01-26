// -*- C++ -*-

// ===================================================================
/**
 *  @file   RT_Thread_Priority_Protocols_Hooks.h
 *
 *  $Id$
 *
 *  @author Jaiganesh Balasubramanian <jai@dre.vanderbilt.edu>
 *          Johnny Willemsen <jwillemsen@remedy.nl>
 */
// ===================================================================

#ifndef TAO_RT_THREAD_PRIORITY_PROTOCOLS_HOOKS_H
#define TAO_RT_THREAD_PRIORITY_PROTOCOLS_HOOKS_H

#include /**/ "ace/pre.h"

#include "tao/orbconf.h"

#if defined (TAO_HAS_CORBA_MESSAGING) && TAO_HAS_CORBA_MESSAGING != 0

#include "tao/RTCORBA/Priority_Mapping_Manager.h"
#include "tao/RTCORBA/Network_Priority_Mapping_Manager.h"
#include "tao/Thread_Priority_Protocols_Hooks.h"
#include "ace/Service_Config.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_RTCORBA_Export TAO_RT_Thread_Priority_Protocols_Hooks 
      : public TAO_Thread_Priority_Protocols_Hooks
{
public:

  /// Constructor
  TAO_RT_Thread_Priority_Protocols_Hooks (void);

  /// Destructor
  virtual ~TAO_RT_Thread_Priority_Protocols_Hooks (void);

  /// Initialize the protocols hooks instance.
  void init_hooks (TAO_ORB_Core *orb_core
                   ACE_ENV_ARG_DECL);

  int get_thread_CORBA_priority (CORBA::Short &
                                 ACE_ENV_ARG_DECL);

  int get_thread_native_priority (CORBA::Short &
                                  ACE_ENV_ARG_DECL);

  int get_thread_CORBA_and_native_priority (CORBA::Short &,
                                            CORBA::Short &
                                            ACE_ENV_ARG_DECL);

  int set_thread_CORBA_priority (CORBA::Short
                                 ACE_ENV_ARG_DECL);

  int set_thread_native_priority (CORBA::Short
                                  ACE_ENV_ARG_DECL);

protected:

  TAO_ORB_Core *orb_core_;

  // Save a reference to the priority mapping manager.
  TAO_Priority_Mapping_Manager_var mapping_manager_;
  TAO_Network_Priority_Mapping_Manager_var network_mapping_manager_;

  RTCORBA::Current_var current_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

ACE_STATIC_SVC_DECLARE_EXPORT (TAO_RTCORBA, 
                               TAO_RT_Thread_Priority_Protocols_Hooks)
ACE_FACTORY_DECLARE (TAO_RTCORBA, TAO_RT_Thread_Priority_Protocols_Hooks)

#endif /* TAO_HAS_CORBA_MESSAGING && TAO_HAS_CORBA_MESSAGING != 0 */

#include /**/ "ace/post.h"
#endif /* TAO_RT_THREAD_PRIORITY_PROTOCOLS_HOOKS_H */
