#include "Push_Handler.h"

#include "tao/debug.h"


ACE_RCSID (LoadBalancing,
           Push_Handler,
           "$Id$")


TAO_LB_Push_Handler::TAO_LB_Push_Handler (
  CosLoadBalancing::LoadMonitor_ptr monitor,
  const PortableGroup::Location & location,
  CosLoadBalancing::LoadManager_ptr manager)
  : monitor_ (CosLoadBalancing::LoadMonitor::_duplicate (monitor)),
    location_ (location),
    manager_ (CosLoadBalancing::LoadManager::_duplicate (manager))
{
}

int
TAO_LB_Push_Handler::handle_timeout (
  const ACE_Time_Value & /* current_time */,
  const void * /* arg */)
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      CosLoadBalancing::LoadList_var loads =
        this->monitor_->loads (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

//       ACE_DEBUG ((LM_DEBUG,
//                   "PUSHING LOAD:\n"
//                   "  id:    %u\n"
//                   "  value: %f\n",
//                   loads[0].id,
//                   loads[0].value));

      this->manager_->push_loads (this->location_,
                                  loads.in ()
                                  ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      // Catch the exception and ignore it.
      //  @@ Yah?

      if (TAO_debug_level > 0)
        ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                             "(%P|%t) Push_Handler::handle_timeout()\n");
    }
  ACE_ENDTRY;

  return 0;
}
