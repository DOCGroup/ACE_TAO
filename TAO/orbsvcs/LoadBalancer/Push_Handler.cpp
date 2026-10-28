#include "Push_Handler.h"
#include "tao/debug.h"

TAO_LB_Push_Handler::TAO_LB_Push_Handler (
  CosLoadBalancing::LoadMonitor_ptr monitor,
  PortableGroup::Location const & location,
  CosLoadBalancing::LoadManager_ptr manager)
  : monitor_ (CosLoadBalancing::LoadMonitor::_duplicate (monitor)),
    location_ (location),
    manager_ (CosLoadBalancing::LoadManager::_duplicate (manager))
{
}

int
TAO_LB_Push_Handler::handle_timeout (
  ACE_Time_Value const & /* current_time */,
  void const * /* arg */)
{
  try
    {
      CosLoadBalancing::LoadList_var loads =
        this->monitor_->loads ();

//       ORBSVCS_DEBUG ((LM_DEBUG,
//                   "PUSHING LOAD:\n"
//                   "  id:    %u\n"
//                   "  value: %f\n",
//                   loads[0].id,
//                   loads[0].value));

      this->manager_->push_loads (this->location_,
                                  loads.in ());
    }
  catch (const CORBA::Exception& ex)
    {
      // Catch the exception and ignore it.
      //  @@ Yah?

      if (TAO_debug_level > 0)
        ex._tao_print_exception ("(%P|%t) Push_Handler::handle_timeout()\n");
    }

  return 0;
}
