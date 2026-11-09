#include "tao/Dynamic_TP/DTP_Config.h"
#include "ace/OS_NS_stdio.h"
#include "ace/Service_Config.h"
#include "ace/Dynamic_Service.h"

#include <array>
#include <cstddef>
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
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("  Stack Size: %d:\n"), entry.stack_size_));
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("  Request queue max depth: %d\n"), entry.queue_depth_));
}

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
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Unable to load configuration\n")));
      return -1;
    }

  TAO_DTP_Definition entry {};

  std::array<ACE_TCHAR const *, 11> const name_list {{
    ACE_TEXT ("ORB"),
    ACE_TEXT ("bogus"),
    ACE_TEXT ("POA1"),
    ACE_TEXT ("defaults"),
    ACE_TEXT ("m1"),
    ACE_TEXT ("m2"),
    ACE_TEXT ("m3"),
    ACE_TEXT ("m4"),
    ACE_TEXT ("m5"),
    ACE_TEXT ("m6"),
    ACE_TEXT ("m7")
  }};

  for (std::size_t i {}; i < name_list.size (); ++i)
    {
      ACE_TCHAR const * const name = name_list[i];
      std::string const name_string (ACE_TEXT_ALWAYS_CHAR (name));
      bool const found = registry->find (name_string, entry);
      bool const should_find = i != 1;

      if (found != should_find)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("Unexpected lookup result for TP Config definition %C\n"),
                      name));
          return -1;
        }

      if (found)
        {
          show_tp_config (name_string, entry);
        }
    }
  return 0;
}
