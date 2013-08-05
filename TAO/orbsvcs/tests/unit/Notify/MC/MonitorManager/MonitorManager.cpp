// $Id$

#include "ace/Service_Types.h"
#include "ace/Service_Config.h"
#include "ace/Service_Repository.h"
#include "ace/Thread_Manager.h"

int
ACE_TMAIN (int, ACE_TCHAR* argv[])
{
#if defined (ACE_HAS_MONITOR_FRAMEWORK) && (ACE_HAS_MONITOR_FRAMEWORK == 1)

  try
    {
      if (ACE_Service_Config::open (argv[0]) != 0)
        {
          ACE_ERROR_RETURN ((LM_ERROR, "Unable to load the TAO_MonitorAndControl\n"), 1);
        }

      // Run the service.
      const ACE_Service_Type* st = 0;
      ACE_Service_Repository::instance ()->find (ACE_TEXT("TAO_MonitorAndControl"),
                                                 &st);
      if (st == 0)
        {
          ACE_ERROR_RETURN ((LM_ERROR, "Failed to find the TAO_MonitorAndControl instance\n"), 1);
        }

      ACE_Service_Object* obj =
        (ACE_Service_Object*) st->type ()->object ();
      obj->resume ();

      // Wait for an external entity to stop it.
      ACE_Thread_Manager::instance ()->wait ();
    }
  catch (...)
    {
      ACE_ERROR_RETURN ((LM_ERROR, "Caught an unexpected exception type\n"), 1);
    }
#else /* ACE_HAS_MONITOR_FRAMEWORK==1 */
  ACE_UNUSED_ARG (argv);
#endif /* ACE_HAS_MONITOR_FRAMEWORK==1 */

  return 0;
}

