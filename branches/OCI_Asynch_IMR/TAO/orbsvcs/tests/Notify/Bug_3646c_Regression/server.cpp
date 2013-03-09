// $Id$

#include "ace/Service_Config.h"
#include "orbsvcs/Notify_Service/Notify_Service.h"
#include "orbsvcs/CosNotifyChannelAdminC.h"
#include "orbsvcs/CosNamingC.h"
#include "ace/Get_Opt.h"

#include "DllORB.h"

char const * const scpc_orbId = "testDllOrb";

const int max_length = 1000;

ACE_TCHAR scpc_loadOrb[max_length] = ACE_DYNAMIC_VERSIONED_SERVICE_DIRECTIVE(
  "testDllOrb",
  "bug3646c",
  TAO_VERSION,
  "_make_DllORB",
  "testDllOrb -ORBDebugLevel 0 -ORBId testDllOrb -ORBDottedDecimalAddresses 1 -ORBInitRef NameService=iioploc://%s:%s/NameService"
);

ACE_TCHAR const * const scpc_unloadOrb = ACE_REMOVE_SERVICE_DIRECTIVE("testDllOrb");

ACE_TCHAR scpc_loadNotifyService[max_length] = ACE_DYNAMIC_VERSIONED_SERVICE_DIRECTIVE(
  "testNotifyService",
  "TAO_Notify_Service",
  TAO_VERSION,
  "_make_TAO_Notify_Service_Driver",
  "-Channel -ChannelName Channel1 -ChannelName Channel2 -RunThreads 0 -ORBInitRef NameService=iioploc://%s:%s/NameService -IORoutput %s"
);

ACE_TCHAR const * const scpc_unloadNotifyService = ACE_REMOVE_SERVICE_DIRECTIVE("testNotifyService");

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("h:p:o:"));
  int c;

  ACE_TCHAR *hostname = 0;
  ACE_TCHAR *port = 0;
  ACE_TCHAR *ior_file = 0;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'h':
         hostname = get_opts.opt_arg ();
        break;
      case 'p':
        port = get_opts.opt_arg ();
        break;
      case 'o':
        ior_file = get_opts.opt_arg ();
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
           "usage:  %s "
           "-h <hostname> "
           "-p <port> "
           "-o <ior> "
           "\n",
           argv [0]),-1);
      }
// Indicates successful parsing of the command line
if ( hostname == 0 || port == 0 || ior_file == 0){
    ACE_ERROR_RETURN ((LM_ERROR,
           "usage:  %s "
           "-h <hostname> "
           "-p <port> "
           "-o <ior> "
           "\n",
           argv [0]),-1);
}

ACE_TCHAR str[max_length];

ACE_OS::strcpy(str, scpc_loadNotifyService);
ACE_OS::sprintf(scpc_loadNotifyService, str,  hostname, port, ior_file);

ACE_OS::strcpy(str, scpc_loadOrb);
ACE_OS::sprintf(scpc_loadOrb, str,  hostname, port);

return 0;
}



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

  p_notifyService->run ();

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

int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  ACE_DEBUG((
    LM_DEBUG,
    ACE_TEXT ("(%P|%t) main - entered\n")
  ));

  if (parse_args (argc, argv) != 0)
        return 1;

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
