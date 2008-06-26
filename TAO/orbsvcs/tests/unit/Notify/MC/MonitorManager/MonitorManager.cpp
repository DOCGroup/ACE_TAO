// $Id$

#include "ace/Service_Types.h"
#include "ace/Service_Config.h"
#include "ace/Service_Repository.h"
#include "ace/Thread_Manager.h"

#if defined (ACE_HAS_MONITOR_FRAMEWORK) && (ACE_HAS_MONITOR_FRAMEWORK == 1)

void
error (const char* msg)
{
  ACE_ERROR ((LM_ERROR, "%s\n", msg));
  ACE_OS::exit (1);
}

#endif /* ACE_HAS_MONITOR_FRAMEWORK==1 */

int
ACE_TMAIN (int, ACE_TCHAR* argv[])
{
#if defined (ACE_HAS_MONITOR_FRAMEWORK) && (ACE_HAS_MONITOR_FRAMEWORK == 1)

  try
    {
      if (ACE_Service_Config::open (argv[0]) != 0)
        {
          error ("Unable to load the TAO_MonitorAndControl");
        }

      // Run the service.
      const ACE_Service_Type* st = 0;
      ACE_Service_Repository::instance ()->find ("TAO_MonitorAndControl",
                                                 &st);
      if (st == 0)
        {
          error ("Failed to find the TAO_MonitorAndControl instance");
        }

      ACE_Service_Object* obj =
        (ACE_Service_Object*) st->type ()->object ();
      obj->resume ();

      // Wait for an external entity to stop it.
      ACE_Thread_Manager::instance ()->wait ();
    }
  catch (...)
    {
      error ("Caught an unexpected exception type");
    }

#endif /* ACE_HAS_MONITOR_FRAMEWORK==1 */

  return 0;
}

