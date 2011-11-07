// $Id$

#include "ace/Service_Config.h"
#include "ace/Log_Msg.h"

int
ACE_TMAIN(int, ACE_TCHAR *[])
{
  ACE_TCHAR const * const scpc_loadOrb =
    ACE_DYNAMIC_SERVICE_DIRECTIVE ("testDllORB",
                                   "Bug_2677_Regression",
                                   "_make_DllORB",
                                   "testDllORB -ORBId testORB");

  ACE_TCHAR const * const scpc_unloadOrb =
    ACE_REMOVE_SERVICE_DIRECTIVE ("testDllORB");

  // Do not use the default svc.conf file
  ACE_Service_Config::open (ACE_TEXT ("server"),
                            ACE_DEFAULT_LOGGER_KEY, 1, 1, 0);

  ACE_Service_Config::process_directive (scpc_loadOrb);
  ACE_DEBUG ((LM_INFO, ACE_TEXT ("loading ORB (1) done\n")));

  ACE_DEBUG ((LM_INFO, ACE_TEXT ("unloading ORB (1) ...\n")));
  ACE_Service_Config::process_directive (scpc_unloadOrb);
  ACE_DEBUG ((LM_INFO, ACE_TEXT ("unloading ORB (1) done\n")));

  ACE_Service_Config::process_directive (scpc_loadOrb);
  ACE_DEBUG ((LM_INFO, ACE_TEXT ("loading ORB (2) done\n")));

  ACE_DEBUG ((LM_INFO, ACE_TEXT ("unloading ORB (2) ...\n")));
  ACE_Service_Config::process_directive (scpc_unloadOrb);
  ACE_DEBUG ((LM_INFO, ACE_TEXT ("unloading ORB (2) done\n")));

  return 0;
}
