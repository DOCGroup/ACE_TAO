// @(#) $Id$

#include "PG_Servant_Dispatcher.h"
#include "GOA.h"

ACE_RCSID(PortableGroup,
          PG_Servant_Dispatcher,
          "$Id$")

TAO_PG_Servant_Dispatcher::~TAO_PG_Servant_Dispatcher (void)
{
}

TAO_POA *
TAO_PG_Servant_Dispatcher::create_POA (const ACE_CString &name,
                                            TAO_POA_Manager &poa_manager,
                                            const TAO_POA_Policy_Set &policies,
                                            TAO_POA *parent,
                                            ACE_Lock &lock,
                                            TAO_SYNCH_MUTEX &thread_lock,
                                            TAO_ORB_Core &orb_core,
                                            TAO_Object_Adapter *object_adapter
                                            ACE_ENV_ARG_DECL)
{
  TAO_POA *poa = 0;

  ACE_NEW_THROW_EX (poa,
                    TAO_GOA (name,
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
TAO_PG_Servant_Dispatcher::pre_invoke_remote_request (TAO_POA &,
                                                      CORBA::Short,
                                                      TAO_ServerRequest &,
                                                      TAO::Portable_Server::Servant_Upcall::Pre_Invoke_State &
                                                      ACE_ENV_ARG_DECL_NOT_USED)
{
}

void
TAO_PG_Servant_Dispatcher::pre_invoke_collocated_request (TAO_POA &,
                                                          CORBA::Short,
                                                          TAO::Portable_Server::Servant_Upcall::Pre_Invoke_State &
                                                          ACE_ENV_ARG_DECL_NOT_USED)
{
}

void
TAO_PG_Servant_Dispatcher::post_invoke (TAO_POA &,
                                        TAO::Portable_Server::Servant_Upcall::Pre_Invoke_State &)
{
}
