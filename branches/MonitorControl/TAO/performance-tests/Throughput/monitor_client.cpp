#include "ace/OS_NS_unistd.h"

#include "tao/Strategies/advanced_resource.h"
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
        
      Monitor::MC::NameList namelist;
      namelist.length (1UL);
      namelist[0] = "Output CDR Buffer";
        
      /// Access the monitor's value a few times and watch it grow.
      for (int i = 0; i < 8; ++i)
        {
          Monitor::MC::DataListList_var data =
            monitor->get_statistics (namelist);
            
          for (CORBA::ULong i = 0; i < data->length(); ++i)
            {
              ACE_DEBUG ((LM_DEBUG, "MP <%s>:\n", data[i].itemname));
              Monitor::MC::DataItem dlist = data[i];
              
              for (CORBA::ULong j = 0; j < dlist.dlist.length (); ++j)
                {
                  Monitor::MC::Data d = dlist.dlist[j];
                  ACE_DEBUG ((LM_DEBUG,
                              "\t value <%u>:\n",
                              static_cast<unsigned int> (d.value)));
                }
            }

          ACE_OS::sleep (1);
        }
      
      orb->destroy ();
    }
  catch (CORBA::Exception &ex)
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Monitor_client: exception caught - ");
      return 1;
    }

  return 0;
}
