// $Id$

#include "orbsvcs/FtRtEvent/EventChannel/create_persistent_poa.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

PortableServer::POA_var create_persistent_poa(PortableServer::POA_var root_poa,
                                              PortableServer::POAManager_var mgr,
                                              const char* name,
                                              CORBA::PolicyList& policy_list)
{
  PortableServer::POA_var result;
    PortableServer::LifespanPolicy_var lifespan =
    root_poa->create_lifespan_policy(PortableServer::PERSISTENT);

  // create a USER_ID IdAssignmentPolicy object
  PortableServer::IdAssignmentPolicy_var assign =
    root_poa->create_id_assignment_policy(PortableServer::USER_ID);

  // create PolicyList.
  size_t orig_len =  policy_list.length();
  policy_list.length(orig_len+2);
  policy_list[orig_len+0]=
    PortableServer::LifespanPolicy::_duplicate(lifespan.in());
  policy_list[orig_len+1]=
    PortableServer::IdAssignmentPolicy::_duplicate(assign.in());

  // create the child POA
  result = root_poa->create_POA(name, mgr.in(), policy_list);


  return result;
}

TAO_END_VERSIONED_NAMESPACE_DECL
