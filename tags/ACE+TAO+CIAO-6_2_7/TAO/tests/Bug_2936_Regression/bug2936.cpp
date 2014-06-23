// $Id$

#include "ace/Service_Config.h"
#include "ace/Log_Msg.h"
#include "ace/OS_NS_unistd.h"

char const * const scpc_orbId = "testDllOrb";

ACE_TCHAR const * const scpc_loadOrb = ACE_DYNAMIC_SERVICE_DIRECTIVE(
  "testDllOrb",
  "bug2936",
  "_make_DllORB",
  "testDllOrb -ORBDebugLevel 0 -ORBId testDllOrb -ORBDottedDecimalAddresses 1"
);

ACE_TCHAR const * const scpc_unloadOrb = ACE_REMOVE_SERVICE_DIRECTIVE("testDllOrb");


ACE_TCHAR const * const scpc_loadPersistentPoa = ACE_DYNAMIC_SERVICE_DIRECTIVE(
  "testPersistentPoa",
  "bug2936",
  "_make_PersistentPoa",
  "testPersistentPoa testDllOrb"
);

ACE_TCHAR const * const scpc_unloadPersistentPoa = ACE_REMOVE_SERVICE_DIRECTIVE("testPersistentPoa");

void loadunloadcycle()
{
  int result = 0;

  ACE_DEBUG((
    LM_DEBUG,
    ACE_TEXT ("(%P|%t) loadunloadcycle - loading\n")
  ));

  result = ACE_Service_Config::process_directive(scpc_loadOrb);
  ACE_DEBUG((
    LM_DEBUG,
    ACE_TEXT ("(%P|%t) loadunloadcycle - loading ORB done\n")
  ));

  result = ACE_Service_Config::process_directive(scpc_loadPersistentPoa);
  ACE_DEBUG((
    LM_DEBUG,
    ACE_TEXT ("(%P|%t) loadunloadcycle - loading PersistentPoa done. Result: <%d>\n"),
    result
  ));

  ACE_DEBUG((
    LM_DEBUG,
    ACE_TEXT ("(%P|%t) loadunloadcycle - unloading\n")
  ));

  ACE_OS::sleep(2);

  ACE_DEBUG((
    LM_DEBUG,
    ACE_TEXT ("(%P|%t) loadunloadcycle - unloading PersistentPoa ...\n")
  ));
  result = ACE_Service_Config::process_directive(scpc_unloadPersistentPoa);
  ACE_DEBUG((
    LM_DEBUG,
    ACE_TEXT ("(%P|%t) loadunloadcycle - unloading PersistentPoa done. Result: <%d>\n"),
    result
  ));

  ACE_DEBUG((
    LM_DEBUG,
    ACE_TEXT ("(%P|%t) loadunloadcycle - unloading ORB ...\n")
  ));
  result = ACE_Service_Config::process_directive(scpc_unloadOrb);
  ACE_DEBUG((
    LM_DEBUG,
    ACE_TEXT ("(%P|%t) loadunloadcycle - unloading ORB done. Result: <%d>\n"),
    result
  ));
}

int ACE_TMAIN(int, ACE_TCHAR ** argv)
{
  int result = 0;

  result = ACE_Service_Config::open (
      argv[0],
      ACE_DEFAULT_LOGGER_KEY,
      1, // ignore_static_svcs = 1,
      1, // ignore_default_svc_conf_file = 0,
      0  // ignore_debug_flag = 0
    );
  if(result != 0)
  {
    ACE_DEBUG((
      LM_DEBUG,
      ACE_TEXT ("(%P|%t) main - ACE_Service_Config::open failed\n")
    ));
    return 1;
  } /* end of if */


  ACE_DEBUG((
    LM_DEBUG,
    ACE_TEXT ("(%P|%t) main - entered\n")
  ));

  ACE_DEBUG((
    LM_DEBUG,
    ACE_TEXT ("(%P|%t) main - cycle 1 ...\n")
  ));
  loadunloadcycle();
  ACE_DEBUG((
    LM_DEBUG,
    ACE_TEXT ("(%P|%t) main - cycle 1 done\n")
  ));

  ACE_DEBUG((
    LM_DEBUG,
    ACE_TEXT ("(%P|%t) main - cycle 2 ...\n")
  ));
  loadunloadcycle();
  ACE_DEBUG((
    LM_DEBUG,
    ACE_TEXT ("(%P|%t) main - cycle 2 done\n")
  ));

  ACE_DEBUG((
    LM_DEBUG,
    ACE_TEXT ("(%P|%t) main - leaving\n")
  ));

  return 0;
}
