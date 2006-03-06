// $Id$

#include "CSD_Default_Servant_Dispatcher.h"
#include "CSD_POA.h"
#include "CSD_Strategy_Repository.h"
#include "ace/Dynamic_Service.h"

ACE_RCSID(CSD_Framework,
          CSD_Default_Servant_Dispatcher,
          "$Id$")


TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_CSD_Default_Servant_Dispatcher::~TAO_CSD_Default_Servant_Dispatcher (void)
{
}


TAO_Root_POA *
TAO_CSD_Default_Servant_Dispatcher::create_Root_POA (const ACE_CString &name,
                                                 TAO_POA_Manager &poa_manager,
                                                 const TAO_POA_Policy_Set &policies,
                                                 ACE_Lock &lock,
                                                 TAO_SYNCH_MUTEX &thread_lock,
                                                 TAO_ORB_Core &orb_core,
                                                 TAO_Object_Adapter *object_adapter
                                                 ACE_ENV_ARG_DECL)
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
                                 ACE_ENV_ARG_PARAMETER),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (0);


  TAO_CSD_Strategy_Repository *repo =
    ACE_Dynamic_Service<TAO_CSD_Strategy_Repository>::instance ("TAO_CSD_Strategy_Repository");

  CSD_Framework::Strategy_var strategy = repo->find (name);

  if (! ::CORBA::is_nil (strategy.in ()))
    {
      poa->set_csd_strategy (strategy.in () ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);
    }

  return poa;
}

TAO_END_VERSIONED_NAMESPACE_DECL
