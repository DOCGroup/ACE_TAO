// $Id$

#include "orbsvcs/FtRtEvent/EventChannel/create_persistent_poa.h"

ACE_RCSID (EventChannel,
           create_persistent_poa,
           "$Id$")

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

PortableServer::POA_var create_persistent_poa(PortableServer::POA_var root_poa,
                                              PortableServer::POAManager_var mgr,
                                              const char* name,
                                              CORBA::PolicyList& policy_list
                                              ACE_ENV_ARG_DECL)
{
  PortableServer::POA_var result;
    PortableServer::LifespanPolicy_var lifespan =
    root_poa->create_lifespan_policy(PortableServer::PERSISTENT
    ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN(result);

  // create a USER_ID IdAssignmentPolicy object
  PortableServer::IdAssignmentPolicy_var assign =
    root_poa->create_id_assignment_policy(PortableServer::USER_ID
    ACE_ENV_ARG_PARAMETER);
   ACE_CHECK_RETURN(result);

  // create PolicyList.
  size_t orig_len =  policy_list.length();
  policy_list.length(orig_len+2);
   ACE_CHECK_RETURN(result);
  policy_list[orig_len+0]=
    PortableServer::LifespanPolicy::_duplicate(lifespan.in());
   ACE_CHECK_RETURN(result);
  policy_list[orig_len+1]=
    PortableServer::IdAssignmentPolicy::_duplicate(assign.in());
   ACE_CHECK_RETURN(result);

  // create the child POA
  result = root_poa->create_POA(name, mgr.in(), policy_list
    ACE_ENV_ARG_PARAMETER);

   ACE_CHECK_RETURN(result);

  return result;
}

TAO_END_VERSIONED_NAMESPACE_DECL
