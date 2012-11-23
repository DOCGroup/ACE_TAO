// $Id$

#include "ace/Service_Config.h"
#include "orbsvcs/Notify/CosNotify_Service.h"
#include "orbsvcs/CosNotifyChannelAdminC.h"
#include "orbsvcs/CosNamingC.h"

#include "DllORB.h"

char const * const scpc_orbId = "testDllOrb";

ACE_TCHAR const * const scpc_loadOrb = ACE_DYNAMIC_VERSIONED_SERVICE_DIRECTIVE(
  "testDllOrb",
  "bug2926",
  TAO_VERSION
  "_make_DllORB",
  "testDllOrb -ORBDebugLevel 0 -ORBId testDllOrb -ORBDottedDecimalAddresses 1"
);

ACE_TCHAR const * const scpc_unloadOrb = ACE_REMOVE_SERVICE_DIRECTIVE("testDllOrb");

ACE_TCHAR const * const scpc_loadNotifyService = ACE_DYNAMIC_VERSIONED_SERVICE_DIRECTIVE(
  "testNotifyService",
  "TAO_CosNotification_Serv",
  TAO_VERSION,
  "_make_TAO_CosNotify_Service",
  ""
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

  CORBA::ORB_var v_orb;
  v_orb = p_orb->orb();
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

  TAO_CosNotify_Service * p_notifyService =
    ACE_Dynamic_Service<TAO_CosNotify_Service>::instance("testNotifyService");

  p_notifyService->init_service(v_orb.in());

  CosNotifyChannelAdmin::EventChannelFactory_var v_notifyFactory =
    p_notifyService->create(v_rootPOA.in(), "");

  // create an event channel
  CosNotifyChannelAdmin::ChannelID id;

  CosNotification::QoSProperties initialQos;
  CosNotification::AdminProperties initialAdmin;

  CosNotifyChannelAdmin::EventChannel_var  v_ec;
  v_ec = v_notifyFactory->create_channel(
    initialQos,
    initialAdmin,
    id
  );

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
