#include "Thread_Task.h"
#include "../Scheduler.h"
#include "tao/RTScheduling/RTScheduler_Manager.h"
#include "tao/RTScheduling/Current.h"
#include "tao/ORB_Core.h"

#include <memory>

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc,
                         argv);

      CORBA::Object_var manager_obj = orb->resolve_initial_references ("RTSchedulerManager");

      TAO_RTScheduler_Manager_var manager = TAO_RTScheduler_Manager::_narrow (manager_obj.in ());

      TAO_Scheduler scheduler (orb.in ());

      manager->rtscheduler (std::addressof (scheduler));

      // Cancellation must not read past short GUIDs or overwrite its local
      // numeric buffer for GUIDs larger than size_t.
      DT_Hash_Map hash;
      CORBA::ULong const lengths[] = {0, 1, sizeof (size_t) + 1};
      for (CORBA::ULong const length : lengths)
        {
          RTScheduling::Current::IdType guid;
          guid.length (length);
          for (CORBA::ULong i {}; i < length; ++i)
            {
              guid[i] = 1;
            }
          TAO_RTScheduler_Current_i *test_current =
            new TAO_RTScheduler_Current_i (orb->orb_core (), std::addressof (hash));
          test_current->id (guid);
          bool cancelled = false;
          try
            {
              // cancel_thread deletes the current before throwing.
              test_current->cancel_thread ();
            }
          catch (CORBA::THREAD_CANCELLED const &)
            {
              cancelled = true;
            }
          if (!cancelled)
            {
              ACE_ERROR_RETURN ((LM_ERROR, "Cancellation did not throw\n"), 1);
            }
        }

      Thread_Task task (orb.in ());

      task.activate_task (4);

      orb->run ();

      // Wait until all threads are ready.
      ACE_Thread_Manager::instance ()->wait ();

      orb->destroy ();
    }
  catch (CORBA::Exception const & ex)
    {
      ex._tao_print_exception ("Caught exception:");
      return 1;
    }

  return 0;
}
