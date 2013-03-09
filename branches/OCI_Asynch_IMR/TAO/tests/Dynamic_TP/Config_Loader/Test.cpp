// $Id$

#include "tao/Dynamic_TP/DTP_Config.h"
#include "ace/OS_NS_stdio.h"
#include "ace/Service_Config.h"
#include "ace/Dynamic_Service.h"

void
show_tp_config (const ACE_CString &name, const TAO_DTP_Definition &entry)
{
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Config set for %C:\n"), name.c_str()));
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("  Initial threads: %d:\n"), entry.init_threads_));
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("  Min threads: %d:\n"), entry.min_threads_));
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("  Max threads: %d:\n"), entry.max_threads_));
  if (entry.min_threads_ > -1)
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("  Idle Timeout: %d (sec)\n"), entry.timeout_.sec()));
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("  Stack Size: %d:\n"), entry.stack_size_));
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("  Request queue max depth: %d\n"), entry.queue_depth_));
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  ACE_Service_Config::open (argc, argv);

  ACE_Service_Gestalt *current = ACE_Service_Config::current();
  TAO_DTP_Config_Registry* registry =
    ACE_Dynamic_Service<TAO_DTP_Config_Registry>::instance
    (current, "DTP_Config_Registry", true);

  if (registry == 0)
    {
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT("Unable to load configuration\n")));
      return -1;
    }

  TAO_DTP_Definition entry;

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
      bool found = registry->find (ACE_TEXT_ALWAYS_CHAR (name_list[i]), entry);
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
          show_tp_config (ACE_TEXT_ALWAYS_CHAR (name_list[i]), entry);
        }
    }
  return 0;
}
