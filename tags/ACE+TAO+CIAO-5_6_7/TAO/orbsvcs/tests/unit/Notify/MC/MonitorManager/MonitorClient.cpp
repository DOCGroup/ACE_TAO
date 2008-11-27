// $Id$

#include "orbsvcs/orbsvcs/Notify/MonitorControl/NotificationServiceMCC.h"

#if defined (TAO_HAS_MONITOR_FRAMEWORK) && (TAO_HAS_MONITOR_FRAMEWORK == 1)

void
error (const char* msg)
{
  ACE_ERROR ((LM_ERROR, "%s\n", msg));
  ACE_OS::exit (1);
}

#endif /* TAO_HAS_MONITOR_FRAMEWORK==1 */

int
ACE_TMAIN (int argc, ACE_TCHAR* argv[])
{
#if defined (TAO_HAS_MONITOR_FRAMEWORK) && (TAO_HAS_MONITOR_FRAMEWORK == 1)

  try
    {
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);
      CORBA::Object_var obj =
        orb->string_to_object (
          "corbaloc:iiop:127.0.0.1:32101/TAO_MonitorAndControl");

      CosNotification::NotificationServiceMonitorControl_var monitor =
        CosNotification::NotificationServiceMonitorControl::_narrow (
          obj.in ());

      if (CORBA::is_nil (monitor.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Unable to resolve the TAO_MonitorAndControl\n"),
                            -1);
        }

      monitor->shutdown ();
      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("MonitorClient: ");
    }
  catch (...)
    {
      error ("Caught an unexpected exception type");
    }

#else /* TAO_HAS_MONITOR_FRAMEWORK==1 */
  ACE_UNUSED_ARG (argc);
  ACE_UNUSED_ARG (argv);
#endif /* TAO_HAS_MONITOR_FRAMEWORK==1 */

  return 0;
}

