#include "Monitor_Signal_Handler.h"


ACE_RCSID (LoadBalancer,
           Monitor_Signal_Handler,
           "$Id$")


TAO_LB_Monitor_Signal_Handler::TAO_LB_Monitor_Signal_Handler (
  CORBA::ORB_ptr orb,
  PortableServer::POA_ptr poa,
  CosLoadBalancing::LoadManager_ptr load_manager,
  const PortableGroup::Location & location)
  : TAO_LB_Signal_Handler (orb, poa),
    load_manager_ (CosLoadBalancing::LoadManager::_duplicate (load_manager)),
    location_ (location)
{
}


int
TAO_LB_Monitor_Signal_Handler::perform_cleanup (int signum)
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      // Deregister the LoadMonitor from the LoadManager in the PULL
      // load monitoring case.
      if (!CORBA::is_nil (this->load_manager_.in ()))
        {
          this->load_manager_->remove_load_monitor (this->location_
                                                    ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Caught exception");

      ACE_ERROR_RETURN ((LM_ERROR,
                         "Problem during LoadMonitor cleanup "
                         "initiated by signal %d.\n",
                         signum),
                        -1);
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (-1);

  return this->TAO_LB_Signal_Handler::perform_cleanup (signum);
}
