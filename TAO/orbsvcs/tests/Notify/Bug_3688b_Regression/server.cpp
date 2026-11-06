#include <cstddef>

#include "ace/Service_Config.h"
#include "orbsvcs/Notify_Service/Notify_Service.h"
#include "orbsvcs/CosNotifyChannelAdminC.h"
#include "orbsvcs/CosNamingC.h"

#include "DllORB.h"
#include "TestListener.h"
#include "TestBroadcaster.h"

static ACE_TCHAR const* const load_orb = ACE_DYNAMIC_VERSIONED_SERVICE_DIRECTIVE (
  "testDllOrb",
  "bug3688",
  TAO_VERSION,
  "_make_DllORB",
  "testDllOrb -ORBDebugLevel 0 -ORBId testDllOrb -ORBDottedDecimalAddresses 1 "
  "-ORBInitRef NameService=file://naming.ior");

static ACE_TCHAR const* const unload_orb = ACE_REMOVE_SERVICE_DIRECTIVE ("testDllOrb");

static ACE_TCHAR const* const load_name_service = ACE_DYNAMIC_VERSIONED_SERVICE_DIRECTIVE (
  "testNamingService",
  "TAO_CosNaming_Serv",
  TAO_VERSION,
  "_make_TAO_Naming_Loader",
  "testNameService testNameService -ORBId testDllOrb -m 0 -o naming.ior");

static ACE_TCHAR const* const unload_name_service = ACE_REMOVE_SERVICE_DIRECTIVE ("testNamingService");

static ACE_TCHAR const* const load_notify_service = ACE_DYNAMIC_VERSIONED_SERVICE_DIRECTIVE (
  "testNotifyService",
  "TAO_Notify_Service",
  TAO_VERSION,
  "_make_TAO_Notify_Service_Driver",
  "-Channel -ChannelName Channel1 -ChannelName Channel2 -RunThreads 1 "
  "-ORBInitRef NameService=file://naming.ior -IORoutput notify.ior");

static ACE_TCHAR const* const unload_notify_service = ACE_REMOVE_SERVICE_DIRECTIVE ("testNotifyService");

static int
publish ()
{
  ACE_DEBUG ((LM_INFO, ACE_TEXT ("(%P|%t) publish ...\n")));

  DllORB* const orb_service = ACE_Dynamic_Service<DllORB>::instance ("testDllOrb");
  CORBA::ORB_var const orb = orb_service->orb ();

  TestBroadcaster broadcaster;
  broadcaster.connect (orb.in (), "corbaname:rir:#Channel1");

  TestListener listener (
    orb.in (),
    "corbaname:rir:#Channel1",
    "$data == 1 or $data == 2 or $data == 4");

  broadcaster.send_data ();

  std::size_t const received_count = listener.received_data.size ();
  ACE_DEBUG ((LM_INFO,
              ACE_TEXT ("(%P|%t) listener.received_data.size()=%B\n"),
              received_count));

  ACE_DEBUG ((LM_INFO, ACE_TEXT ("(%P|%t) publish done\n")));
  return received_count == 3 ? 0 : static_cast<int> (received_count);
}

static int
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

  result = ACE_Service_Config::process_directive (load_name_service);
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%P|%t) loadunloadcycle - loading NamingService done. Result: <%d>\n"),
              result));

  result = ACE_Service_Config::process_directive (load_notify_service);
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%P|%t) loadunloadcycle - loading NotifyService done. Result: <%d>\n"),
              result));

  int const publish_result = publish ();

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%P|%t) loadunloadcycle - unloading\n")));
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%P|%t) loadunloadcycle - unloading NotifyService ...\n")));
  result = ACE_Service_Config::process_directive (unload_notify_service);
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%P|%t) loadunloadcycle - unloading NotifyService done. Result: <%d>\n"),
              result));

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%P|%t) loadunloadcycle - unloading NameService ...\n")));
  result = ACE_Service_Config::process_directive (unload_name_service);
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%P|%t) loadunloadcycle - unloading NameService done. Result: <%d>\n"),
              result));

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%P|%t) loadunloadcycle - unloading ORB ...\n")));
  result = ACE_Service_Config::process_directive (unload_orb);
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%P|%t) loadunloadcycle - unloading ORB done. Result: <%d>\n"),
              result));

  return publish_result;
}

int
ACE_TMAIN (int, ACE_TCHAR**)
{
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%P|%t) main - entered\n")));
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%P|%t) main - cycle ...\n")));

  int const result = load_unload_cycle ();
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%P|%t) main - cycle done\n")));
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%P|%t) main - leaving\n")));
  return result;
}
