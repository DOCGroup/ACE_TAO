#include "tao/Dynamic_TP/DTP_POA_Strategy.h"
#include "tao/CSD_Framework/CSD_Strategy_Repository.h"

#include "ace/OS_NS_stdio.h"
#include "ace/Service_Config.h"
#include "ace/Dynamic_Service.h"
#include "tao/debug.h"

#include <array>

struct Expected_POA_Config
{
  ACE_TCHAR const *name;
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
    {ACE_TEXT ("MyPOA"), true},
    {ACE_TEXT ("POA2"), false},
    {ACE_TEXT ("MyPOA3"), true},
    {ACE_TEXT ("MyPOA1"), false}
  }};

  for (Expected_POA_Config const &config : poa_list)
    {
      ACE_DEBUG ((LM_INFO, ACE_TEXT ("Config definition for %C\n"), config.name));
      TAO_DTP_POA_Strategy const * const found_strategy =
        dynamic_cast<TAO_DTP_POA_Strategy const *> (
          dtp_poa_strat_repo->find (ACE_TEXT_ALWAYS_CHAR (config.name)));
      if ((found_strategy != nullptr) != config.expected)
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("Unexpected lookup result for POA configuration %C\n"),
                      config.name));
          return -1;
        }
    }

  return 0;
}
