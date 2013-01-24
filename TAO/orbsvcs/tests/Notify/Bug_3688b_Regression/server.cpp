// $Id$

#include "ace/Service_Config.h"
#include "orbsvcs/Notify_Service/Notify_Service.h"
#include "orbsvcs/CosNotifyChannelAdminC.h"
#include "orbsvcs/CosNamingC.h"


#include "DllORB.h"
#include "TestListener.h"
#include "TestBroadcaster.h"


char const * const scpc_orbId = "testDllOrb";

ACE_TCHAR const * const scpc_loadOrb = ACE_DYNAMIC_VERSIONED_SERVICE_DIRECTIVE(
  "testDllOrb",
  "bug3688",
  TAO_VERSION,
  "_make_DllORB",
  "testDllOrb -ORBDebugLevel 0 -ORBId testDllOrb -ORBDottedDecimalAddresses 1 -ORBInitRef NameService=file://naming.ior"
);

ACE_TCHAR const * const scpc_unloadOrb = ACE_REMOVE_SERVICE_DIRECTIVE("testDllOrb");

ACE_TCHAR const * const scpc_loadNameService = ACE_DYNAMIC_VERSIONED_SERVICE_DIRECTIVE(
  "testNamingService",
  "TAO_CosNaming_Serv",
  TAO_VERSION,
  "_make_TAO_Naming_Loader",
  "testNameService testNameService -ORBId testDllOrb -m 0 -o naming.ior"
);

ACE_TCHAR const * const scpc_unloadNameService = ACE_REMOVE_SERVICE_DIRECTIVE("testNamingService");


ACE_TCHAR const * const scpc_loadNotifyService = ACE_DYNAMIC_VERSIONED_SERVICE_DIRECTIVE(
  "testNotifyService",
  "TAO_Notify_Service",
  TAO_VERSION,
  "_make_TAO_Notify_Service_Driver",
  "-Channel -ChannelName Channel1 -ChannelName Channel2 -RunThreads 1 -ORBInitRef NameService=file://naming.ior -IORoutput notify.ior"
);

ACE_TCHAR const * const scpc_unloadNotifyService = ACE_REMOVE_SERVICE_DIRECTIVE("testNotifyService");



int publish()
{
  int result = 0;

  ACE_DEBUG((
    LM_INFO,
    ACE_TEXT ("(%P|%t) publish ...\n")
  ));

  DllORB * p_orb = ACE_Dynamic_Service<DllORB>::instance ("testDllOrb");
  CORBA::ORB_var v_orb = p_orb->orb();

  TestBroadcaster testBroadcaster;

  testBroadcaster.connect(
    v_orb.in(),
    "corbaname:rir:#Channel1"
  );

  TestListener testListener(
    v_orb.in(),
    "corbaname:rir:#Channel1",
    "$data == 1 or "
    "$data == 2 or "
    "$data == 4"
  );

  testBroadcaster.sendData();


  ACE_DEBUG((
    LM_INFO,
    ACE_TEXT ("(%P|%t) testListener.m_receivedData.size()=%d\n"),
    testListener.m_receivedData.size()
  ));

  result = testListener.m_receivedData.size() == 3 ? 0 : testListener.m_receivedData.size();

  ACE_DEBUG((
    LM_INFO,
    ACE_TEXT ("(%P|%t) publish done\n")
  ));

  return result;
}

int loadunloadcycle()
{
  int publish_result = 0;

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

  result = ACE_Service_Config::process_directive(scpc_loadNameService);
  ACE_DEBUG((
    LM_DEBUG,
    ACE_TEXT ("(%P|%t) loadunloadcycle - loading NamingService done. Result: <%d>\n"),
    result
  ));

  result = ACE_Service_Config::process_directive(scpc_loadNotifyService);
  ACE_DEBUG((
    LM_DEBUG,
    ACE_TEXT ("(%P|%t) loadunloadcycle - loading NotifyService done. Result: <%d>\n"),
    result
  ));

  publish_result = publish();

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
    ACE_TEXT ("(%P|%t) loadunloadcycle - unloading NameService ...\n")
  ));
  result = ACE_Service_Config::process_directive(scpc_unloadNameService);
  ACE_DEBUG((
    LM_DEBUG,
    ACE_TEXT ("(%P|%t) loadunloadcycle - unloading NameService done. Result: <%d>\n"),
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

  return publish_result;
}

int ACE_TMAIN (int , ACE_TCHAR **)
{
  ACE_DEBUG((
    LM_DEBUG,
    ACE_TEXT ("(%P|%t) main - entered\n")
  ));

  ACE_DEBUG((
    LM_DEBUG,
    ACE_TEXT ("(%P|%t) main - cycle ...\n")
  ));

  int result = loadunloadcycle();

  ACE_DEBUG((
    LM_DEBUG,
    ACE_TEXT ("(%P|%t) main - cycle done\n")
  ));

  ACE_DEBUG((
    LM_DEBUG,
    ACE_TEXT ("(%P|%t) main - leaving\n")
  ));

  return result;
}
