// $Id$

#include "tao/Dynamic_TP/DTP_POA_Strategy.h"
#include "tao/CSD_Framework/CSD_Strategy_Repository.h"

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
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("  Idle Timeout: %d (sec)\n"), entry.timeout_.sec()));
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("  Stack Size: %d:\n"), entry.stack_size_));
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("  Request queue max depth: %d\n"), entry.queue_depth_));
}

//void
//show_poa_config (const ACE_CString &name, TAO_DTP_POA_Strategy * strat)
//{
  //ACE_DEBUG ((LM_INFO, ACE_TEXT ("POA [%C] has the configuration key [%C]\n"), name.c_str(),strat->get_tp_config().c_str()));
  //ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("  Initial threads: %d:\n"), entry.init_threads_));
  //ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("  Min threads: %d:\n"), entry.min_threads_));
  //ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("  Max threads: %d:\n"), entry.max_threads_));
  //if (entry.min_threads_ > -1)
  //  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("  Idle Timeout: %d (sec)\n"), entry.timeout_));
  //ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("  Stack Size: %d:\n"), entry.stack_size_));
  //ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("  Request queue max depth: %d\n"), entry.queue_depth_));
//}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  TAO_debug_level = 1;

  ACE_Service_Config::open (argc, argv);

  // Dump the POA to Strategy map
  TAO_CSD_Strategy_Repository* dtp_poa_strat_repo =
    ACE_Dynamic_Service<TAO_CSD_Strategy_Repository>::instance
    ("TAO_CSD_Strategy_Repository");

  if (dtp_poa_strat_repo == 0)
    {
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT("Unable to load configuration\n")));
      return -1;
    }
  else
    {
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT("TAO_CSD_Strategy_Repository found\n")));
    }


  // Now iterate through the repository and attempt to find sample POA configurations

  const ACE_TCHAR *poa_list [] =
    { ACE_TEXT ("MyPOA"),  // should find this one
      ACE_TEXT ("POA2"),  // shouldn't find this one
      ACE_TEXT ("MyPOA3"),  // should find this one
      ACE_TEXT ("MyPOA1"), // should find this one
      0
    };

  TAO_DTP_POA_Strategy * mapped_strategy;

  for (int i = 0; poa_list[i] != 0; i++)
    {
      mapped_strategy = 0;
      ACE_DEBUG ((LM_INFO, ACE_TEXT("Config definition for %C\n"), poa_list[i]));
      mapped_strategy = dynamic_cast <TAO_DTP_POA_Strategy*> (dtp_poa_strat_repo->find (poa_list[i]));
      if ((mapped_strategy == 0) && (i != 1) && (i != 3))
        {
          ACE_DEBUG ((LM_DEBUG, ACE_TEXT("Cannot find TP Config definition for %C\n"), poa_list[i]));
          continue;
        }
      if (i == 1 || i == 3)
        {
          if (mapped_strategy)
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
          //show_poa_config (poa_list[i], mapped_strategy);
        }
    }


  return 0;
}
