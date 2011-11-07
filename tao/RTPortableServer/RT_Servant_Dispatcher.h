// -*- C++ -*-

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
#include /**/ "ace/pre.h"

#include "tao/RTPortableServer/rtportableserver_export.h"
#include "tao/orbconf.h"

#if defined (TAO_HAS_CORBA_MESSAGING) && TAO_HAS_CORBA_MESSAGING != 0

#define TAO_RTCORBA_SAFE_INCLUDE
#include "tao/RTCORBA/RTCORBAC.h"
#undef TAO_RTCORBA_SAFE_INCLUDE

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/PortableServer/Servant_Dispatcher.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_RT_Servant_Dispatcher
 *
 * @brief Concrete instantiation of the Servant_Dispatcher strategy.
 *
 */
class TAO_RTPortableServer_Export TAO_RT_Servant_Dispatcher
  : public TAO_Servant_Dispatcher
{
public:
  virtual ~TAO_RT_Servant_Dispatcher (void);

  /// Pre_invoke remote request.
  void pre_invoke_remote_request (TAO_Root_POA &poa,
                                  CORBA::Short servant_priority,
                                  TAO_ServerRequest &req,
                                  TAO::Portable_Server::Servant_Upcall::Pre_Invoke_State &pre_invoke_state
                                 );

  /// Pre_invoke collocated request.
  void pre_invoke_collocated_request (TAO_Root_POA &poa,
                                      CORBA::Short servant_priority,
                                      TAO::Portable_Server::Servant_Upcall::Pre_Invoke_State &pre_invoke_state
                                     );

  /// Post_invoke request.
  void post_invoke (TAO_Root_POA &poa,
                    TAO::Portable_Server::Servant_Upcall::Pre_Invoke_State &pre_invoke_state);

  /// Factory method for creating new POA's.
  TAO_Root_POA *create_Root_POA (const ACE_CString &name,
                                 PortableServer::POAManager_ptr poa_manager,
                                 const TAO_POA_Policy_Set &policies,
                                 ACE_Lock &lock,
                                 TAO_SYNCH_MUTEX &thread_lock,
                                 TAO_ORB_Core &orb_core,
                                 TAO_Object_Adapter *object_adapter
                                );
};

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_HAS_CORBA_MESSAGING && TAO_HAS_CORBA_MESSAGING != 0 */

#include /**/ "ace/post.h"

#endif /* TAO_RT_SERVANT_DISPATCHER_H */
