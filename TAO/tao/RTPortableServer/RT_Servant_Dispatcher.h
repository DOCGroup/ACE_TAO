/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    RT_Servant_Dispatcher.h
 *
 *  $Id$
 *
 *  @author Frank Hunleth (fhunleth@cs.wustl.edu)
 */
//=============================================================================


#ifndef TAO_RT_SERVANT_DISPATCHER_H
#define TAO_RT_SERVANT_DISPATCHER_H
#include "ace/pre.h"

#include "rtportableserver_export.h"
#include "tao/PortableServer/Servant_Dispatcher.h"

#if defined (TAO_HAS_CORBA_MESSAGING) && TAO_HAS_CORBA_MESSAGING != 0

#define TAO_RTCORBA_SAFE_INCLUDE
#include "tao/RTCORBA/RTCORBAC.h"
#undef TAO_RTCORBA_SAFE_INCLUDE

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/**
 * @class TAO_RT_Servant_Dispatcher
 *
 * @brief Concrete instantiation of the Servant_Dispatcher strategy.
 *
 *
 */
class TAO_RTPortableServer_Export TAO_RT_Servant_Dispatcher
  : public TAO_Servant_Dispatcher
{
public:
  virtual ~TAO_RT_Servant_Dispatcher (void);

  /// Pre_invoke remote request.
  void pre_invoke_remote_request (TAO_POA &poa,
                                  CORBA::Short servant_priority,
                                  TAO_ServerRequest &req,
								  TAO_Object_Adapter::Servant_Upcall::Pre_Invoke_State &pre_invoke_state
                                  ACE_ENV_ARG_DECL);

  /// Pre_invoke collocated request.
  void pre_invoke_collocated_request (TAO_POA &poa,
                                      CORBA::Short servant_priority,
                                      TAO_Object_Adapter::Servant_Upcall::Pre_Invoke_State &pre_invoke_state
                                      ACE_ENV_ARG_DECL);

  /// Post_invoke request.
  void post_invoke (TAO_POA &poa,
                    TAO_Object_Adapter::Servant_Upcall::Pre_Invoke_State &pre_invoke_state);

  /// Factory method for creating new POA's.
  TAO_POA *create_POA (const ACE_CString &name,
                       TAO_POA_Manager &poa_manager,
                       const TAO_POA_Policy_Set &policies,
                       TAO_POA *parent,
                       ACE_Lock &lock,
                       TAO_SYNCH_MUTEX &thread_lock,
                       TAO_ORB_Core &orb_core,
                       TAO_Object_Adapter *object_adapter
                       ACE_ENV_ARG_DECL);
};

#endif /* TAO_HAS_CORBA_MESSAGING && TAO_HAS_CORBA_MESSAGING != 0 */

#include "ace/post.h"
#endif /* TAO_RT_SERVANT_DISPATCHER_H */
