// $Id$

#include "ace/ARGV.h"
#include "ace/Reactor.h"
#include "ace/Signal.h"
#include "ace/Service_Config.h"
#include "ace/Log_Msg.h"

ACE_TCHAR const * const scpc_loadOrb = ACE_DYNAMIC_SERVICE_DIRECTIVE(
  "testDllOrb",
  "Bug_3542",
  "_make_DllOrb",
  "testDllOrb -ORBDebugLevel 0 -ORBId testDllOrb -NumThreads 2 -ORBDottedDecimalAddresses 1 -ORBCollocationStrategy thru_poa"
);

ACE_TCHAR const * const scpc_unloadOrb = ACE_REMOVE_SERVICE_DIRECTIVE("testDllOrb");

int
ACE_TMAIN(int, ACE_TCHAR **)
{
  int result = 0;
#if 0
  ACE_Service_Config serviceConfig;

  char signum[64];
  ACE_OS::sprintf(signum, "%d", SIGUSR1);

  ACE_ARGV args;
  args.add(argv[0]);
  args.add("-s");
  args.add(signum);

  result = serviceConfig.open (
    args.argc(),
    args.argv(),
    ACE_DEFAULT_LOGGER_KEY,
    1, // ignore_static_svcs = 1,
    1, // ignore_default_svc_conf_file = 0,
    0  // ignore_debug_flag = 0
  );
  if(0 != result)
  {
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("(%P|%t) Error: serviceConfig.open failed\n")));
    return result;
  }
  ACE_DEBUG ((LM_INFO, ACE_TEXT ("(%P|%t) serviceConfig.open done\n")));
#endif

  for(int cnt = 0; cnt < 1000; ++cnt)
  {
    ACE_DEBUG ((LM_INFO, ACE_TEXT ("(%P|%t) Loading ORB cnt = %d\n"), cnt));
    result = ACE_Service_Config::process_directive(scpc_loadOrb);
    if(0 != result)
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("(%P|%t) Error loading ORB failed (%d)\n"), result));
      return result;
    }
    ACE_DEBUG ((LM_INFO, ACE_TEXT ("(%P|%t) Loading ORB done\n")));

    result = ACE_Service_Config::process_directive(scpc_unloadOrb);
    if(0 != result)
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("(%P|%t) Error unloading ORB failed (%d)\n"), result));
      return result;
    }
    ACE_DEBUG ((LM_INFO, ACE_TEXT ("(%P|%t) Unloading ORB done\n")));
  }

  return result;
}
