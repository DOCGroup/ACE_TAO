/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    Servant_Dispatcher.h
 *
 *  $Id$
 *
 *  @author Frank Hunleth <fhunleth@cs.wustl.edu>
 */
//=============================================================================


#ifndef TAO_SERVANT_DISPATCHER_H
#define TAO_SERVANT_DISPATCHER_H
#include "ace/pre.h"

#include "ace/SString.h"
#include "portableserver_export.h"
#include "Object_Adapter.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

// Forward references.
class TAO_POA_Manager;
class TAO_POA_Policy_Set;
class TAO_POA;
class ACE_Lock;
class TAO_ORB_Core;
class TAO_ServerRequest;

/**
 * @class TAO_Servant_Dispatcher
 *
 * @brief Interface for POA servant dispatching strategies.
 *
 */
class TAO_PortableServer_Export TAO_Servant_Dispatcher
{
public:
  virtual ~TAO_Servant_Dispatcher (void);

  /// Pre_invoke remote request.
  virtual void pre_invoke_remote_request (TAO_POA &poa,
                                          CORBA::Short servant_priority,
                                          //TAO_Service_Context &request_service_context,
                                          //TAO_Service_Context &reply_service_context,
										  TAO_ServerRequest &req,
                                          TAO_Object_Adapter::Servant_Upcall::Pre_Invoke_State &pre_invoke_state
                                          ACE_ENV_ARG_DECL) = 0;

  /// Pre_invoke collocated request.
  virtual void pre_invoke_collocated_request (TAO_POA &poa,
                                              CORBA::Short servant_priority,
                                              TAO_Object_Adapter::Servant_Upcall::Pre_Invoke_State &pre_invoke_state
                                              ACE_ENV_ARG_DECL) = 0;

  /// Post_invoke request.
  virtual void post_invoke (TAO_POA &poa,
                            TAO_Object_Adapter::Servant_Upcall::Pre_Invoke_State &pre_invoke_state) = 0;

  /// Factory method for creating new POA's.
  virtual TAO_POA *create_POA (const ACE_CString &name,
                               TAO_POA_Manager &poa_manager,
                               const TAO_POA_Policy_Set &policies,
                               TAO_POA *parent,
                               ACE_Lock &lock,
                               TAO_SYNCH_MUTEX &thread_lock,
                               TAO_ORB_Core &orb_core,
                               TAO_Object_Adapter *object_adapter
                               ACE_ENV_ARG_DECL) = 0;

};

#include "ace/post.h"
#endif /* TAO_SERVANT_DISPATCHER_H */
