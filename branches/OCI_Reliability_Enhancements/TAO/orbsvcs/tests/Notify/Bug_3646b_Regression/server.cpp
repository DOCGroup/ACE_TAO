// $Id$

#include "ace/Service_Config.h"
#include "orbsvcs/Notify_Service/Notify_Service.h"
#include "orbsvcs/CosNotifyChannelAdminC.h"
#include "orbsvcs/CosNamingC.h"

#include "DllORB.h"

char const * const scpc_orbId = "testDllOrb";

ACE_TCHAR const * const scpc_loadOrb = ACE_DYNAMIC_VERSIONED_SERVICE_DIRECTIVE(
  "testDllOrb",
  "bug3646b",
  TAO_VERSION,
  "_make_DllORB",
  "testDllOrb -ORBDebugLevel 0 -ORBId testDllOrb -ORBDottedDecimalAddresses 1"
);

ACE_TCHAR const * const scpc_unloadOrb = ACE_REMOVE_SERVICE_DIRECTIVE("testDllOrb");

ACE_TCHAR const * const scpc_loadNotifyService = ACE_DYNAMIC_VERSIONED_SERVICE_DIRECTIVE(
  "testNotifyService",
  "TAO_Notify_Service",
  TAO_VERSION,
  "_make_TAO_Notify_Service_Driver",
  "-NoNameSvc -RunThreads 0"
);

ACE_TCHAR const * const scpc_unloadNotifyService = ACE_REMOVE_SERVICE_DIRECTIVE("testNotifyService");

void loadunloadcycle()
{
  ACE_DEBUG((
    LM_DEBUG,
    ACE_TEXT ("(%P|%t) loadunloadcycle - loading\n")
  ));

  int result = ACE_Service_Config::process_directive(scpc_loadOrb);
  ACE_DEBUG((
    LM_DEBUG,
    ACE_TEXT ("(%P|%t) loadunloadcycle - loading ORB done. Result: <%d>\n"),
    result
  ));

  DllORB * p_orb =
    ACE_Dynamic_Service<DllORB>::instance("testDllOrb");

  CORBA::ORB_var v_orb = p_orb->orb();
    ACE_DEBUG((
    LM_DEBUG,
    ACE_TEXT ("(%P|%t) loadunloadcycle - v_orb OK\n")
  ));

  CORBA::Object_var v_poa =
    v_orb->resolve_initial_references("RootPOA");
  ACE_DEBUG((
    LM_DEBUG,
    ACE_TEXT ("(%P|%t) loadunloadcycle - v_poa OK\n")
  ));

  PortableServer::POA_var v_rootPOA =
    PortableServer::POA::_narrow(v_poa.in ());
  ACE_DEBUG((
    LM_DEBUG,
    ACE_TEXT ("(%P|%t) loadunloadcycle - v_rootPOA OK\n")
  ));

  result = ACE_Service_Config::process_directive(scpc_loadNotifyService);
  ACE_DEBUG((
    LM_DEBUG,
    ACE_TEXT ("(%P|%t) loadunloadcycle - loading NotifyService done. Result: <%d>\n"),
    result
  ));

  TAO_Notify_Service_Driver * p_notifyService =
    ACE_Dynamic_Service<TAO_Notify_Service_Driver>::instance("testNotifyService");

  ACE_UNUSED_ARG (p_notifyService);

  ACE_DEBUG((
    LM_DEBUG,
    ACE_TEXT ("(%P|%t) loadunloadcycle - unloading\n")
  ));

  ACE_DEBUG((
    LM_DEBUG,
    ACE_TEXT ("(%P|%t) loadunloadcycle - unloading NotifyService ...\n")
  ));
  result = ACE_Service_Config::process_directive(scpc_unloadNotifyService);
  ACE_DEBUG((
    LM_DEBUG,
    ACE_TEXT ("(%P|%t) loadunloadcycle - unloading NotifyService done. Result: <%d>\n"),
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

int ACE_TMAIN (int , ACE_TCHAR **)
{
  ACE_DEBUG((
    LM_DEBUG,
    ACE_TEXT ("(%P|%t) main - entered\n")
  ));

  for (int cnt = 0, max = 2; cnt < max; ++cnt)
    {
      ACE_DEBUG((
        LM_DEBUG,
        ACE_TEXT ("(%P|%t) main - cycle %d ...\n"), cnt
      ));
      loadunloadcycle();
      ACE_DEBUG((
        LM_DEBUG,
        ACE_TEXT ("(%P|%t) main - cycle %d done\n"), cnt
      ));
    }

  ACE_DEBUG((
    LM_DEBUG,
    ACE_TEXT ("(%P|%t) main - leaving\n")
  ));

  return 0;
}
