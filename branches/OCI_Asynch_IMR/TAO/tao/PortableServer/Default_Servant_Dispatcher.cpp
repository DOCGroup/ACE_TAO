// $Id$

#include "tao/PortableServer/Default_Servant_Dispatcher.h"
#include "tao/PortableServer/Root_POA.h"
#include "tao/PortableServer/Network_Priority_Hook.h"
#include "ace/CORBA_macros.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Default_Servant_Dispatcher::~TAO_Default_Servant_Dispatcher (void)
{
}

TAO_Root_POA *
TAO_Default_Servant_Dispatcher::create_Root_POA (const ACE_CString &name,
                                                 PortableServer::POAManager_ptr poa_manager,
                                                 const TAO_POA_Policy_Set &policies,
                                                 ACE_Lock &lock,
                                                 TAO_SYNCH_MUTEX &thread_lock,
                                                 TAO_ORB_Core &orb_core,
                                                 TAO_Object_Adapter *object_adapter)
{
  TAO_Root_POA *poa = 0;

  ACE_NEW_THROW_EX (poa,
                    TAO_Root_POA (name,
                                  poa_manager,
                                  policies,
                                  0,
                                  lock,
                                  thread_lock,
                                  orb_core,
                                  object_adapter),
                    CORBA::NO_MEMORY ());

  return poa;
}

void
TAO_Default_Servant_Dispatcher::pre_invoke_remote_request (
  TAO_Root_POA &poa,
  CORBA::Short,
  TAO_ServerRequest &req,
  TAO::Portable_Server::Servant_Upcall::Pre_Invoke_State &)
{
  poa.network_priority_hook ()-> set_dscp_codepoint (req, poa);
}

void
TAO_Default_Servant_Dispatcher::pre_invoke_collocated_request (
  TAO_Root_POA &,
  CORBA::Short,
  TAO::Portable_Server::Servant_Upcall::Pre_Invoke_State &)
{
}

void
TAO_Default_Servant_Dispatcher::post_invoke (
  TAO_Root_POA &,
  TAO::Portable_Server::Servant_Upcall::Pre_Invoke_State &)
{
}

TAO_END_VERSIONED_NAMESPACE_DECL
