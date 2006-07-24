// $Id$

#include "tao/PortableServer/Default_Servant_Dispatcher.h"
#include "tao/PortableServer/Root_POA.h"

ACE_RCSID(PortableServer,
          Default_Servant_Dispatcher,
          "$Id$")

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
                                                 TAO_Object_Adapter *object_adapter
                                                 ACE_ENV_ARG_DECL)
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
                                  object_adapter
                                  ACE_ENV_ARG_PARAMETER), CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (0);

  return poa;
}

void
TAO_Default_Servant_Dispatcher::pre_invoke_remote_request (
  TAO_Root_POA &,
  CORBA::Short,
  TAO_ServerRequest &,
  TAO::Portable_Server::Servant_Upcall::Pre_Invoke_State &
  ACE_ENV_ARG_DECL_NOT_USED)
{
}

void
TAO_Default_Servant_Dispatcher::pre_invoke_collocated_request (
  TAO_Root_POA &,
  CORBA::Short,
  TAO::Portable_Server::Servant_Upcall::Pre_Invoke_State &
  ACE_ENV_ARG_DECL_NOT_USED)
{
}

void
TAO_Default_Servant_Dispatcher::post_invoke (
  TAO_Root_POA &,
  TAO::Portable_Server::Servant_Upcall::Pre_Invoke_State &)
{
}

TAO_END_VERSIONED_NAMESPACE_DECL
