// @(#) $Id$

#include "Default_Servant_Dispatcher.h"
#include "POA.h"

ACE_RCSID(tao, Default_Acceptor_Filter, "$Id$")

TAO_Default_Servant_Dispatcher::~TAO_Default_Servant_Dispatcher (void)
{
}

TAO_POA *
TAO_Default_Servant_Dispatcher::create_POA (const ACE_CString &name,
                                            TAO_POA_Manager &poa_manager,
                                            const TAO_POA_Policy_Set &policies,
                                            TAO_POA *parent,
                                            ACE_Lock &lock,
                                            TAO_SYNCH_MUTEX &thread_lock,
                                            TAO_ORB_Core &orb_core,
                                            TAO_Object_Adapter *object_adapter
                                            ACE_ENV_ARG_DECL)
{
  TAO_POA *poa;

  ACE_NEW_THROW_EX (poa,
                    TAO_POA (name,
                             poa_manager,
                             policies,
                             parent,
                             lock,
                             thread_lock,
                             orb_core,
                             object_adapter
                             ACE_ENV_ARG_PARAMETER),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (0);

  return poa;
}

void
TAO_Default_Servant_Dispatcher::pre_invoke_remote_request (TAO_POA &,
                                                           CORBA::Short,
                                                           TAO_ServerRequest &,
                                                           TAO_Object_Adapter::Servant_Upcall::Pre_Invoke_State &
                                                           ACE_ENV_ARG_DECL_NOT_USED)
{
}

void
TAO_Default_Servant_Dispatcher::pre_invoke_collocated_request (TAO_POA &,
                                                               CORBA::Short,
                                                               TAO_Object_Adapter::Servant_Upcall::Pre_Invoke_State &
                                                               ACE_ENV_ARG_DECL_NOT_USED)
{
}

void
TAO_Default_Servant_Dispatcher::post_invoke (TAO_POA &,
                                             TAO_Object_Adapter::Servant_Upcall::Pre_Invoke_State &)

{
}
