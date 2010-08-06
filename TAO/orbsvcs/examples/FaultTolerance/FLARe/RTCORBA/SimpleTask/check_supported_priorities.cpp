// $Id$

#include "ace/Sched_Params.h"
#include "tao/ORB_Core.h"
#include "tao/ORB.h"
#include "tao/RTCORBA/Priority_Mapping_Manager.h"
#include "tao/Protocols_Hooks.h"

const char *
sched_policy_name (int sched_policy)
{
  const char *name = 0;

  switch (sched_policy)
    {
    case ACE_SCHED_OTHER:
      name = "SCHED_OTHER";
      break;
    case ACE_SCHED_RR:
      name = "SCHED_RR";
      break;
    case ACE_SCHED_FIFO:
      name = "SCHED_FIFO";
      break;
    }

  return name;
}

bool
check_supported_priorities (CORBA::ORB_ptr orb)
{
  int sched_policy =
    orb->orb_core ()->orb_params ()->ace_sched_policy ();

  // Check that we have sufficient priority range to run this test,
  // i.e., more than 1 priority level.
  int max_priority =
    ACE_Sched_Params::priority_max (sched_policy);
  int min_priority =
    ACE_Sched_Params::priority_min (sched_policy);

  if (max_priority == min_priority)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Not enough priority levels with the %C scheduling policy\n"
                  "on this platform to run the test, terminating program....\n"
                  "Check svc.conf options\n",
                  sched_policy_name (sched_policy)));

      return false;
    }

  return true;
}

CORBA::Short
get_implicit_thread_CORBA_priority (CORBA::ORB_ptr orb)
{
  CORBA::Object_var obj =
    orb->resolve_initial_references (TAO_OBJID_PRIORITYMAPPINGMANAGER);

  TAO_Priority_Mapping_Manager_var mapping_manager =
    TAO_Priority_Mapping_Manager::_narrow (obj.in ());

  if (CORBA::is_nil (mapping_manager.in ()))
    throw CORBA::INTERNAL ();

  RTCORBA::PriorityMapping *pm =
    mapping_manager.in ()->mapping ();

  TAO_Protocols_Hooks *tph =
    orb->orb_core ()->get_protocols_hooks ();

  CORBA::Short corba_priority;
  CORBA::Short native_priority;

  if (tph->get_thread_native_priority (native_priority) == 0 &&
      pm->to_CORBA (native_priority, corba_priority))
  {
    return corba_priority;
  }

  ACE_DEBUG ((LM_DEBUG, "get_implicit_thread_CORBA_priority - failed to access priority\n"));
  throw CORBA::DATA_CONVERSION (CORBA::OMGVMCID | 2, CORBA::COMPLETED_NO);
}

