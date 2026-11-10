#include "tao/Dynamic_TP/DTP_POA_Strategy.h"
#include "tao/CSD_Framework/CSD_Strategy_Repository.h"

#include "ace/Dynamic_Service.h"
#include "ace/Service_Config.h"
#include "tao/debug.h"

#include <array>
#include <string>

struct Expected_POA_Config
{
  std::string name;
  bool expected;
};

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  TAO_debug_level = 1;

  ACE_Service_Config::open (argc, argv);

  // Dump the POA to Strategy map
  TAO_CSD_Strategy_Repository * const dtp_poa_strat_repo =
    ACE_Dynamic_Service<TAO_CSD_Strategy_Repository>::instance
    ("TAO_CSD_Strategy_Repository");

  if (dtp_poa_strat_repo == nullptr)
    {
      ACE_DEBUG ((LM_ERROR, ACE_TEXT ("Unable to load configuration\n")));
      return -1;
    }

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("TAO_CSD_Strategy_Repository found\n")));

  // Check that configured POA names exist and unconfigured names do not.
  std::array<Expected_POA_Config, 4> const poa_list {{
    {"MyPOA", true},
    {"POA2", false},
    {"MyPOA3", true},
    {"MyPOA1", false}
  }};

  for (Expected_POA_Config const &config : poa_list)
    {
      ACE_DEBUG ((LM_INFO, ACE_TEXT ("Config definition for %C\n"), config.name.c_str ()));
      TAO_DTP_POA_Strategy const * const found_strategy =
        dynamic_cast<TAO_DTP_POA_Strategy const *> (dtp_poa_strat_repo->find (config.name));
      if ((found_strategy != nullptr) != config.expected)
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("Unexpected lookup result for POA configuration %C\n"),
                      config.name.c_str ()));
          return -1;
        }
    }

  return 0;
}
