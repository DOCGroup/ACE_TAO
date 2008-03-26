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
      
      CORBA::Object_var obj =
        orb->string_to_object (monitor_input_file);
      Monitor::MC_var monitor = Monitor::MC::_narrow (obj.in ());
      
      for (int i = 0; i < 8; ++i)
        {
          Monitor::MC::Data data =
            monitor->get_statistic ("Output CDR Buffer");
            
          cout << data.value << endl;
            
          ACE_OS::sleep (1);
        }
      
      orb->destroy ();
    }
  catch (CORBA::Exception &ex)
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "monitor_lient: exception caught - ");
      return 1;
    }

  return 0;
}
