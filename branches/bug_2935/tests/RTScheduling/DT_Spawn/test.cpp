//$Id$
#include "../Scheduler.h"
#include "tao/RTScheduling/RTScheduler_Manager.h"
#include "tao/ORB.h"
#include "Thread_Action.h"
#include "ace/Thread_Manager.h"
#include "ace/SString.h"

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  CORBA::ORB_var orb;
  RTScheduling::Current_var current;

  const char * name = 0;
  CORBA::Policy_ptr sched_param = 0;
  CORBA::Policy_ptr implicit_sched_param = 0;

  Test_Thread_Action thread_action;

  try
    {
      orb = CORBA::ORB_init (argc,
			     argv,
			     "");

      CORBA::Object_var manager_obj = orb->resolve_initial_references ("RTSchedulerManager");

      TAO_RTScheduler_Manager_var manager = TAO_RTScheduler_Manager::_narrow (manager_obj.in ());

      TAO_Scheduler* scheduler;
      ACE_NEW_RETURN (scheduler,
		      TAO_Scheduler (orb.in ()),
		      -1);
      RTScheduling::Scheduler_var safe_scheduler = scheduler;

      manager->rtscheduler (scheduler);

      CORBA::Object_var current_obj = orb->resolve_initial_references ("RTScheduler_Current");

      current = RTScheduling::Current::_narrow (current_obj.in ());

      try
	{

	  ACE_DEBUG ((LM_DEBUG,
		      "Invoking DT spawn without calling begin_scheduling_segment...\n"));

	  ACE_CString data ("Harry Potter");
	  current->spawn (&thread_action,
			  const_cast<char *> (data.c_str ()),
			  name,
			  sched_param,
			  implicit_sched_param,
			  0,
			  0);
	}
      catch (const CORBA::BAD_INV_ORDER& )
	{
	  ACE_DEBUG ((LM_DEBUG,
		      "Spawn should be in the context of a Scheduling Segment - Expected Exception\n"));
	}
      catch (const CORBA::Exception& ex)
	{
	  ex._tao_print_exception ("\n");
	}

      ACE_DEBUG ((LM_DEBUG,
		  "Start - Scheduling Segment...\n"));

      current->begin_scheduling_segment ("Potter",
					 sched_param,
					 implicit_sched_param);

      size_t count = 0;
      ACE_OS::memcpy (&count,
		      current->id ()->get_buffer (),
		      current->id ()->length ());

      ACE_DEBUG ((LM_DEBUG,
		  "The Current DT Guid is %d\n",
		  count));

      //Initialize data to be passed to the Thread_Action::do method
      Data spawn_data;
      spawn_data.data = "Harry Potter";
      spawn_data.current = RTScheduling::Current::_duplicate (current.in ());

      ACE_DEBUG ((LM_DEBUG,
		  "Spawning a new DT...\n"));
      RTScheduling::DistributableThread_var dt =
      current->spawn (&thread_action,
		      &spawn_data,
		      "Chamber of Secrets",
		      sched_param,
		      implicit_sched_param,
		      0,
		      0);

      current->end_scheduling_segment (name);
      ACE_DEBUG ((LM_DEBUG,
		  "End - Scheduling Segment %d\n",
		  count));

    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Caught Exception\n");
    }

  ACE_Thread_Manager::instance ()->wait ();

  return 0;
}

