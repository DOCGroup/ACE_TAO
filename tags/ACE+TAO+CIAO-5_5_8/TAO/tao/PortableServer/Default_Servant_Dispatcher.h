// -*- C++ -*-

//=============================================================================
/**
 *  @file    Default_Servant_Dispatcher.h
 *
 *  $Id$
 *
 *  @author Frank Hunleth (fhunleth@cs.wustl.edu)
 */
//=============================================================================

#ifndef TAO_DEFAULT_SERVANT_DISPATCHER_H
#define TAO_DEFAULT_SERVANT_DISPATCHER_H
#include /**/ "ace/pre.h"

#include "tao/PortableServer/portableserver_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/PortableServer/Servant_Dispatcher.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_Default_Servant_Dispatcher
 *
 * @brief Concrete instantiation of the Servant_Dispatcher strategy.
 *
 * This class just calls _dispatch on the servant_upcall.  No extra
 * processing is done or needed.
 *
 */
class TAO_PortableServer_Export TAO_Default_Servant_Dispatcher
  : public TAO_Servant_Dispatcher
{
public:
  virtual ~TAO_Default_Servant_Dispatcher (void);

  /// Pre_invoke remote request.
  void pre_invoke_remote_request (
        TAO_Root_POA &poa,
        CORBA::Short servant_priority,
        TAO_ServerRequest &req,
        TAO::Portable_Server::Servant_Upcall::Pre_Invoke_State &pre_invoke_state);

  /// Pre_invoke collocated request.
  void pre_invoke_collocated_request (
        TAO_Root_POA &poa,
        CORBA::Short servant_priority,
        TAO::Portable_Server::Servant_Upcall::Pre_Invoke_State &pre_invoke_state);

  /// Post_invoke request.
  void post_invoke (
        TAO_Root_POA &poa,
        TAO::Portable_Server::Servant_Upcall::Pre_Invoke_State &pre_invoke_state);

  /// Factory method for creating new POA's.
  TAO_Root_POA *create_Root_POA (
        const ACE_CString &name,
        PortableServer::POAManager_ptr poa_manager,
        const TAO_POA_Policy_Set &policies,
        ACE_Lock &lock,
        TAO_SYNCH_MUTEX &thread_lock,
        TAO_ORB_Core &orb_core,
        TAO_Object_Adapter *object_adapter);
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /* TAO_DEFAULT_SERVANT_DISPATCHER_H */
