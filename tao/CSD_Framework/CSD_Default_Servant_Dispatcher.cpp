// -*- C++ -*-
// $Id$

#include "tao/CSD_Framework/CSD_Default_Servant_Dispatcher.h"
#include "tao/CSD_Framework/CSD_POA.h"
#include "tao/CSD_Framework/CSD_Strategy_Repository.h"
#include "ace/Dynamic_Service.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_CSD_Default_Servant_Dispatcher::~TAO_CSD_Default_Servant_Dispatcher (void)
{
}


TAO_Root_POA *
TAO_CSD_Default_Servant_Dispatcher::create_Root_POA (const ACE_CString &name,
                                                     PortableServer::POAManager_ptr poa_manager,
                                                 const TAO_POA_Policy_Set &policies,
                                                 ACE_Lock &lock,
                                                 TAO_SYNCH_MUTEX &thread_lock,
                                                 TAO_ORB_Core &orb_core,
                                                 TAO_Object_Adapter *object_adapter)
    {
  TAO_CSD_POA *poa = 0;

  ACE_NEW_THROW_EX (poa,
                    TAO_CSD_POA (name,
                                 poa_manager,
                                 policies,
                                 0,
                                 lock,
                                 thread_lock,
                                 orb_core,
                                 object_adapter
                                ),
                    CORBA::NO_MEMORY ());


  TAO_CSD_Strategy_Repository *repo =
    ACE_Dynamic_Service<TAO_CSD_Strategy_Repository>::instance ("TAO_CSD_Strategy_Repository");

  if (repo == 0)
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT("(%P|%t) CSD_Default_Servant_Dispatcher::")
                    ACE_TEXT("create_POA could not find ")
                    ACE_TEXT("TAO_CSD_Strategy_Repository\n")));
      return 0;
    }

  CSD_Framework::Strategy_var strategy = repo->find (name);

  if (! ::CORBA::is_nil (strategy.in ()))
    {
      poa->set_csd_strategy (strategy.in ());
    }

  return poa;
}

TAO_END_VERSIONED_NAMESPACE_DECL
