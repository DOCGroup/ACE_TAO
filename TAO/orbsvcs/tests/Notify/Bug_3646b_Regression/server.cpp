#include "ace/Service_Config.h"
#include "orbsvcs/Notify_Service/Notify_Service.h"
#include "orbsvcs/CosNotifyChannelAdminC.h"
#include "orbsvcs/CosNamingC.h"

#include "DllORB.h"

static ACE_TCHAR const* const load_orb = ACE_DYNAMIC_VERSIONED_SERVICE_DIRECTIVE (
  "testDllOrb",
  "bug3646b",
  TAO_VERSION,
  "_make_DllORB",
  "testDllOrb -ORBDebugLevel 0 -ORBId testDllOrb -ORBDottedDecimalAddresses 1");

static ACE_TCHAR const* const unload_orb = ACE_REMOVE_SERVICE_DIRECTIVE ("testDllOrb");

static ACE_TCHAR const* const load_notify_service = ACE_DYNAMIC_VERSIONED_SERVICE_DIRECTIVE (
  "testNotifyService",
  "TAO_Notify_Service",
  TAO_VERSION,
  "_make_TAO_Notify_Service_Driver",
  "-NoNameSvc -RunThreads 0");

static ACE_TCHAR const* const unload_notify_service = ACE_REMOVE_SERVICE_DIRECTIVE ("testNotifyService");

static void
load_unload_cycle ()
{
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%P|%t) loadunloadcycle - loading\n")));

  int result = ACE_Service_Config::process_directive (load_orb);
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%P|%t) loadunloadcycle - loading ORB done. Result: <%d>\n"),
              result));

  DllORB* const orb_service = ACE_Dynamic_Service<DllORB>::instance ("testDllOrb");
  CORBA::ORB_var const orb = orb_service->orb ();
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%P|%t) loadunloadcycle - v_orb OK\n")));

  CORBA::Object_var const poa_object = orb->resolve_initial_references ("RootPOA");
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%P|%t) loadunloadcycle - v_poa OK\n")));

  PortableServer::POA_var const root_poa = PortableServer::POA::_narrow (poa_object.in ());
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%P|%t) loadunloadcycle - v_rootPOA OK\n")));

  result = ACE_Service_Config::process_directive (load_notify_service);
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%P|%t) loadunloadcycle - loading NotifyService done. Result: <%d>\n"),
              result));

  ACE_Dynamic_Service<TAO_Notify_Service_Driver>::instance ("testNotifyService");

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%P|%t) loadunloadcycle - unloading\n")));
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%P|%t) loadunloadcycle - unloading NotifyService ...\n")));
  result = ACE_Service_Config::process_directive (unload_notify_service);
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%P|%t) loadunloadcycle - unloading NotifyService done. Result: <%d>\n"),
              result));

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%P|%t) loadunloadcycle - unloading ORB ...\n")));
  result = ACE_Service_Config::process_directive (unload_orb);
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%P|%t) loadunloadcycle - unloading ORB done. Result: <%d>\n"),
              result));
}

int
ACE_TMAIN (int, ACE_TCHAR**)
{
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%P|%t) main - entered\n")));

  for (int cycle {}; cycle < 2; ++cycle)
    {
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%P|%t) main - cycle %d ...\n"), cycle));
      load_unload_cycle ();
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%P|%t) main - cycle %d done\n"), cycle));
    }

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%P|%t) main - leaving\n")));
  return 0;
}
