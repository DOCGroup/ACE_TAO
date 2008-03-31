#include "ace/streams.h"
#include "ace/OS_NS_unistd.h"

#include "tao/Monitor/Monitor.h"

const char *monitor_input_file = "file://monitor.ior";

int 
main (int argc, char *argv[])
{
  try
    {
      CORBA::ORB_var orb = CORBA::ORB_init (argc, 
                                            argv, 
                                            "");
      
      /// Get the MC object reference that the client has exposed.
      CORBA::Object_var obj =
        orb->string_to_object (monitor_input_file);
        
      if (CORBA::is_nil (obj.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Monitor client - string_to_object failed\n"),
                             -1);
        }
        
      Monitor::MC_var monitor = Monitor::MC::_narrow (obj.in ());
      
      if (CORBA::is_nil (monitor.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Monitor client - narrow failed\n"),
                             -1);
        }
        
      /// Access the monitor's value a few times and watch it grow.
      for (int i = 0; i < 8; ++i)
        {
          Monitor::MC::Data data =
            monitor->get_statistic ("Output CDR Buffer");
            
          cout << "client marshal buffer size = " << data.value
               << " bytes" << endl;
            
          ACE_OS::sleep (1);
        }
      
      orb->destroy ();
    }
  catch (CORBA::Exception &ex)
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Monitor_vlient: exception caught - ");
      return 1;
    }

  return 0;
}
