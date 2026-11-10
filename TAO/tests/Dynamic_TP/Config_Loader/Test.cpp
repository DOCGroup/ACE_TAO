#include "tao/Dynamic_TP/DTP_Config.h"
#include "ace/Dynamic_Service.h"
#include "ace/Service_Config.h"

#include <array>
#include <string>

void
show_tp_config (std::string const &name, TAO_DTP_Definition const &entry)
{
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Config set for %C:\n"), name.c_str()));
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("  Initial threads: %d:\n"), entry.init_threads_));
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("  Min threads: %d:\n"), entry.min_threads_));
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("  Max threads: %d:\n"), entry.max_threads_));
  if (entry.min_threads_ > -1)
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("  Idle Timeout: %d (sec)\n"), entry.timeout_.sec()));
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("  Stack Size: %B:\n"), entry.stack_size_));
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("  Request queue max depth: %d\n"), entry.queue_depth_));
}

struct Expected_DTP_Config
{
  std::string name;
  bool expected;
};

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  ACE_Service_Config::open (argc, argv);

  ACE_Service_Gestalt * const current = ACE_Service_Config::current ();
  TAO_DTP_Config_Registry * const registry =
    ACE_Dynamic_Service<TAO_DTP_Config_Registry>::instance
    (current, "DTP_Config_Registry", true);

  if (registry == nullptr)
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("Unable to load configuration\n")));
      return -1;
    }

  TAO_DTP_Definition entry {};

  std::array<Expected_DTP_Config, 11> const configs {{
    {"ORB", true},
    {"bogus", false},
    {"POA1", true},
    {"defaults", true},
    {"m1", true},
    {"m2", true},
    {"m3", true},
    {"m4", true},
    {"m5", true},
    {"m6", true},
    {"m7", true}
  }};

  for (Expected_DTP_Config const &config : configs)
    {
      bool const found = registry->find (config.name, entry);

      if (found != config.expected)
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("Unexpected lookup result for TP Config definition %C\n"),
                      config.name.c_str ()));
          return -1;
        }

      if (found)
        show_tp_config (config.name, entry);
    }
  return 0;
}
