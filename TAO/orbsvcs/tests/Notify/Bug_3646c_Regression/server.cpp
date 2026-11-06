#include "ace/Service_Config.h"
#include "orbsvcs/Notify_Service/Notify_Service.h"
#include "orbsvcs/CosNotifyChannelAdminC.h"
#include "orbsvcs/CosNamingC.h"
#include "ace/Get_Opt.h"

#include "DllORB.h"

constexpr int max_length = 1000;

#if defined (ACE_USES_WCHAR)
# define LOAD_ORB_DIRECTIVE \
  "testDllOrb -ORBDebugLevel 0 -ORBId testDllOrb -ORBDottedDecimalAddresses 1 " \
  "-ORBInitRef NameService=iioploc://%ls:%ls/NameService"
#else
# define LOAD_ORB_DIRECTIVE \
  "testDllOrb -ORBDebugLevel 0 -ORBId testDllOrb -ORBDottedDecimalAddresses 1 " \
  "-ORBInitRef NameService=iioploc://%s:%s/NameService"
#endif

static ACE_TCHAR load_orb[max_length] = ACE_DYNAMIC_VERSIONED_SERVICE_DIRECTIVE (
  "testDllOrb",
  "bug3646c",
  TAO_VERSION,
  "_make_DllORB",
  LOAD_ORB_DIRECTIVE);

static ACE_TCHAR const* const unload_orb = ACE_REMOVE_SERVICE_DIRECTIVE ("testDllOrb");

#if defined (ACE_USES_WCHAR)
# define LOAD_NOTIFY_SERVICE_DIRECTIVE \
  "-Channel -ChannelName Channel1 -ChannelName Channel2 -RunThreads 0 " \
  "-ORBInitRef NameService=iioploc://%ls:%ls/NameService -IORoutput %ls"
#else
# define LOAD_NOTIFY_SERVICE_DIRECTIVE \
  "-Channel -ChannelName Channel1 -ChannelName Channel2 -RunThreads 0 " \
  "-ORBInitRef NameService=iioploc://%s:%s/NameService -IORoutput %s"
#endif

static ACE_TCHAR load_notify_service[max_length] = ACE_DYNAMIC_VERSIONED_SERVICE_DIRECTIVE (
  "testNotifyService",
  "TAO_Notify_Service",
  TAO_VERSION,
  "_make_TAO_Notify_Service_Driver",
  LOAD_NOTIFY_SERVICE_DIRECTIVE);

static ACE_TCHAR const* const unload_notify_service = ACE_REMOVE_SERVICE_DIRECTIVE ("testNotifyService");

static int
parse_args (int argc, ACE_TCHAR* argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("h:p:o:"));
  int option {};
  ACE_TCHAR const* hostname {};
  ACE_TCHAR const* port {};
  ACE_TCHAR const* ior_file {};

  while ((option = get_opts ()) != -1)
    {
      switch (option)
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
                             "usage: %s -h <hostname> -p <port> -o <ior>\n",
                             argv[0]),
                            -1);
        }
    }

  if (hostname == nullptr || port == nullptr || ior_file == nullptr)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "usage: %s -h <hostname> -p <port> -o <ior>\n",
                       argv[0]),
                      -1);

  ACE_TCHAR directive_format[max_length] {};
  ACE_OS::strcpy (directive_format, load_notify_service);
  ACE_OS::sprintf (load_notify_service, directive_format, hostname, port, ior_file);

  ACE_OS::strcpy (directive_format, load_orb);
  ACE_OS::sprintf (load_orb, directive_format, hostname, port);

  return 0;
}

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

  TAO_Notify_Service_Driver* const notify_service =
    ACE_Dynamic_Service<TAO_Notify_Service_Driver>::instance ("testNotifyService");
  if (notify_service == nullptr)
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%P|%t) Could not resolve testNotifyService\n")));
  else
    notify_service->run ();

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
ACE_TMAIN (int argc, ACE_TCHAR* argv[])
{
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%P|%t) main - entered\n")));

  if (parse_args (argc, argv) != 0)
    return 1;

  for (int cycle {}; cycle < 2; ++cycle)
    {
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%P|%t) main - cycle %d ...\n"), cycle));
      load_unload_cycle ();
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%P|%t) main - cycle %d done\n"), cycle));
    }

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%P|%t) main - leaving\n")));
  return 0;
}
