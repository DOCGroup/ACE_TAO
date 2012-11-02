// $Id$

#include "tao/Dynamic_TP/Dynamic_TP_Config.h"
#include "tao/Dynamic_TP/Dynamic_TP_POA_Loader.h"
#include "ace/OS_NS_stdio.h"
#include "ace/Service_Config.h"
#include "ace/Dynamic_Service.h"
#include <iostream>
#include "tao/debug.h"

void
show_tp_config (const ACE_CString &name, const TAO_DTP_Definition &entry)
{
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Config set for %C:\n"), name.c_str()));
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("  Initial threads: %d:\n"), entry.init_threads_));
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("  Min threads: %d:\n"), entry.min_threads_));
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("  Max threads: %d:\n"), entry.max_threads_));
  if (entry.min_threads_ > -1)
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("  Idle Timeout: %d (sec)\n"), entry.timeout_));
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("  Stack Size: %d:\n"), entry.stack_size_));
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("  Request queue max depth: %d\n"), entry.queue_depth_));
}

void
show_poa_config (const ACE_CString &name, const TAO_DTP_POA_Config_Map &entry)
{
  ACE_DEBUG ((LM_INFO, ACE_TEXT ("Config set for [%C]\n"), name.c_str()));
  //ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("  Initial threads: %d:\n"), entry.init_threads_));
  //ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("  Min threads: %d:\n"), entry.min_threads_));
  //ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("  Max threads: %d:\n"), entry.max_threads_));
  //if (entry.min_threads_ > -1)
  //  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("  Idle Timeout: %d (sec)\n"), entry.timeout_));
  //ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("  Stack Size: %d:\n"), entry.stack_size_));
  //ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("  Request queue max depth: %d\n"), entry.queue_depth_));
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  TAO_debug_level = 1;
	
	ACE_Service_Config::open (argc, argv);



  ACE_Service_Gestalt *current = ACE_Service_Config::current();
  TAO_Dynamic_TP_Config_Registry* registry =
    ACE_Dynamic_Service<TAO_Dynamic_TP_Config_Registry>::instance
    (current, "Dynamic_TP_Config_Registry", true);

  if (registry == 0)
    {
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT("Unable to load configuration\n")));
      return -1;
    }

 /* TAO_DTP_Definition entry;

  const ACE_TCHAR *name_list [] =
    { ACE_TEXT ("ORB"),
      ACE_TEXT ("bogus"),
      ACE_TEXT ("POA1"),
      ACE_TEXT ("defaults"),
      ACE_TEXT ("m1"),
      ACE_TEXT ("m2"),
      ACE_TEXT ("m3"),
      ACE_TEXT ("m4"),
      ACE_TEXT ("m5"),
      ACE_TEXT ("m6"),
      ACE_TEXT ("m7"),
      0
    };

  for (int i = 0; name_list[i] != 0; i++)
    {
      bool found = registry->find (name_list[i], entry);
      if (!found && (i != 1))
        {
          ACE_DEBUG ((LM_DEBUG, ACE_TEXT("Cannot find TP Config definition for %C\n"), name_list[i]));
          return -1;
        }
      if (i == 1)
        {
          if (found)
            {
              ACE_DEBUG ((LM_DEBUG, ACE_TEXT("Found TP Config definition for %C which should have failed\n"), name_list[i]));
              return -1;
            }
          else
            {
              ACE_DEBUG ((LM_DEBUG, ACE_TEXT("TP definition for %C not found as expected\n"), name_list[i]));
            }
        }
      else
        {
          show_tp_config (name_list[i], entry);
        }
    }*/


  
  TAO_Dynamic_TP_POA_Config_Map_Registry* dtp_poa_map_registry =
    ACE_Dynamic_Service<TAO_Dynamic_TP_POA_Config_Map_Registry>::instance
    (current, "Dynamic_TP_POA_Config_Map_Registry", true);

  if (dtp_poa_map_registry == 0)
    {
	  ACE_DEBUG ((LM_DEBUG, ACE_TEXT("Unable to load configuration\n")));
      return -1;
    }
  else
  {
	  ACE_DEBUG ((LM_DEBUG, ACE_TEXT("Dynamic_TP_POA_Config_Map_Registry found\n")));
	  ACE_DEBUG ((LM_DEBUG, ACE_TEXT("There are %i elements in the registry.\n"),dtp_poa_map_registry->get_count()));
  }

  TAO_DTP_POA_Config_Map empty_TP;


  const ACE_TCHAR *poa_list [] =
    { ACE_TEXT ("POA1"),
      ACE_TEXT ("POA2"),
      ACE_TEXT ("POA3"),
      ACE_TEXT ("MyPOA"),
      0
    };

    for (int i = 0; poa_list[i] != 0; i++)
    {
	  ACE_DEBUG ((LM_INFO, ACE_TEXT("Config definition for %C\n"), poa_list[i]));
      bool found = dtp_poa_map_registry->find (poa_list[i], empty_TP);
      if (!found && (i != 1))
        {
          ACE_DEBUG ((LM_DEBUG, ACE_TEXT("Cannot find TP Config definition for %C\n"), poa_list[i]));
        }
      if (i == 1)
        {
          if (found)
            {
              ACE_DEBUG ((LM_DEBUG, ACE_TEXT("Found TP Config definition for %C which should have failed\n"), poa_list[i]));
            }
          else
            {
              ACE_DEBUG ((LM_DEBUG, ACE_TEXT("Config definition for %C not found as expected\n"), poa_list[i]));
            }
        }
      else
        {
          show_poa_config (poa_list[i], empty_TP);
        }
    }


  return 0;
}
