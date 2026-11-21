#include "ace/ARGV.h"
#include "ace/Service_Config.h"
#include "ace/Log_Msg.h"

#include "tao/corba.h"
#include "tao/ORB.h"
#include "tao/PortableServer/PortableServer.h"
#include "orbsvcs/CosNotifyChannelAdminC.h"
#include "orbsvcs/Notify/CosNotify_Service.h"

#include "DllOrb.h"

static ACE_TCHAR const* const scpc_load_orb = ACE_DYNAMIC_VERSIONED_SERVICE_DIRECTIVE (
  "testDllOrb",
  "Bug_3252",
  TAO_VERSION,
  "_make_DllOrb",
  "testDllOrb -ORBDebugLevel 0 -ORBId testDllOrb -NumThreads 2 -ORBDottedDecimalAddresses 1 "
  "-ORBCollocationStrategy thru_poa");

static ACE_TCHAR const* const scpc_unload_orb = ACE_REMOVE_SERVICE_DIRECTIVE ("testDllOrb");

static ACE_TCHAR const* const scpc_load_notify_service = ACE_DYNAMIC_VERSIONED_SERVICE_DIRECTIVE (
  "testNotifyService",
  "TAO_CosNotification_Serv",
  TAO_VERSION,
  "_make_TAO_CosNotify_Service",
  "-UseSeparateDispatchingORB 1");

static ACE_TCHAR const* const scpc_unload_notify_service =
  ACE_REMOVE_SERVICE_DIRECTIVE ("testNotifyService");

static int
unload_notify (ACE_Service_Config& service_config)
{
  ACE_DEBUG ((LM_INFO, ACE_TEXT ("Unloading NotifyService ...\n")));
  if (service_config.process_directive (scpc_unload_notify_service) != 0)
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("Unloading NotifyService failed\n")));
      return -1;
    }

  ACE_DEBUG ((LM_INFO, ACE_TEXT ("unloading NotifyService done.\n")));
  return 0;
}

static int
load_notify (ACE_Service_Config& service_config)
{
  try
    {
      DllOrb* const p_orb = ACE_Dynamic_Service<DllOrb>::instance ("testDllOrb");
      CORBA::ORB_var const v_orb = p_orb->orb ();

      CORBA::Object_var const v_poa = v_orb->resolve_initial_references ("RootPOA");
      PortableServer::POA_var const v_root_poa = PortableServer::POA::_narrow (v_poa.in ());
      ACE_DEBUG ((LM_INFO, ACE_TEXT ("RootPOA OK.\n")));

      ACE_DEBUG ((LM_INFO, ACE_TEXT ("Loading NotifyService ...\n")));
      if (service_config.process_directive (scpc_load_notify_service) != 0)
        {
          ACE_ERROR ((LM_ERROR, ACE_TEXT ("Loading NotifyService failed\n")));
          return -1;
        }
      ACE_DEBUG ((LM_INFO, ACE_TEXT ("Loading NotifyService done.\n")));

      TAO_CosNotify_Service* const notify_service =
        ACE_Dynamic_Service<TAO_CosNotify_Service>::instance ("testNotifyService");
      notify_service->init_service (v_orb.in ());

      CosNotifyChannelAdmin::EventChannelFactory_var const notify_factory =
        notify_service->create (v_root_poa.in ());
      ACE_DEBUG ((LM_INFO, ACE_TEXT ("got EventChannelFactory\n")));

      CosNotifyChannelAdmin::ChannelID id {};
      CosNotification::QoSProperties initial_qos;
      CosNotification::AdminProperties initial_admin;
      CosNotifyChannelAdmin::EventChannel_var const event_channel =
        notify_factory->create_channel (initial_qos, initial_admin, id);
      ACE_DEBUG ((LM_INFO, ACE_TEXT ("got EventChannel\n")));

      CosNotifyFilter::FilterFactory_var const filter_factory =
        event_channel->default_filter_factory ();
      ACE_DEBUG ((LM_INFO, ACE_TEXT ("got FilterFactory\n")));

      CosNotifyFilter::Filter_var const filter = filter_factory->create_filter ("ETCL");
      ACE_DEBUG ((LM_INFO, ACE_TEXT ("got Filter\n")));
    }
  catch (CORBA::Exception const& ex)
    {
      ex._tao_print_exception ("Unexpected CORBA Exception: ");
      return -1;
    }
  catch (...)
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("Unexpected C++ Exception\n")));
      return -1;
    }

  return 0;
}

static int
unload_orb (ACE_Service_Config& service_config)
{
  ACE_DEBUG ((LM_INFO, ACE_TEXT ("Unloading ORB ...\n")));
  if (service_config.process_directive (scpc_unload_orb) != 0)
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("Unloading ORB failed\n")));
      return -1;
    }

  ACE_DEBUG ((LM_INFO, ACE_TEXT ("unloading ORB done.\n")));
  return 0;
}

static int
load_orb (ACE_Service_Config& service_config)
{
  ACE_DEBUG ((LM_INFO, ACE_TEXT ("Loading ORB ...\n")));
  if (service_config.process_directive (scpc_load_orb) != 0)
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("Loading ORB failed\n")));
      return -1;
    }

  ACE_DEBUG ((LM_INFO, ACE_TEXT ("Loading ORB done.\n")));
  return 0;
}

int
ACE_TMAIN (int, ACE_TCHAR **argv)
{
  ACE_TCHAR signum[64] {};
  ACE_OS::sprintf (signum, ACE_TEXT ("%d"), SIGUSR1);

  ACE_ARGV args;
  args.add (argv[0]);
  args.add (ACE_TEXT ("-s"));
  args.add (signum);

  ACE_Service_Config& service_config = *ACE_Service_Config::singleton ();
  int result = service_config.open (args.argc (),
                                   args.argv (),
                                   ACE_DEFAULT_LOGGER_KEY,
                                   true, // ignore_static_svcs
                                   true, // ignore_default_svc_conf_file
                                   false); // ignore_debug_flag

  if (result != 0)
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("serviceConfig.open failed\n")));
      return result;
    }
  ACE_DEBUG ((LM_INFO, ACE_TEXT ("serviceConfig.open done\n")));

  for (int i {}; i < 5; ++i)
    {
      ACE_DEBUG ((LM_INFO, ACE_TEXT ("loadOrb ...\n")));
      result = load_orb (service_config);
      if (result != 0)
        {
          ACE_ERROR ((LM_ERROR, ACE_TEXT ("loadOrb failed\n")));
          return result;
        }
      ACE_DEBUG ((LM_INFO, ACE_TEXT ("loadOrb done\n")));

      ACE_DEBUG ((LM_INFO, ACE_TEXT ("loadNotify ...\n")));
      result = load_notify (service_config);
      if (result != 0)
        {
          ACE_ERROR ((LM_ERROR, ACE_TEXT ("loadNotify failed\n")));
          return result;
        }
      ACE_DEBUG ((LM_INFO, ACE_TEXT ("loadNotify done\n")));

      ACE_DEBUG ((LM_INFO, ACE_TEXT ("unloadNotify ...\n")));
      result = unload_notify (service_config);
      if (result != 0)
        {
          ACE_ERROR ((LM_ERROR, ACE_TEXT ("unloadNotify failed\n")));
          return result;
        }
      ACE_DEBUG ((LM_INFO, ACE_TEXT ("unloadNotify done\n")));

      ACE_DEBUG ((LM_INFO, ACE_TEXT ("unloadOrb ...\n")));
      result = unload_orb (service_config);
      if (result != 0)
        {
          ACE_ERROR ((LM_ERROR, ACE_TEXT ("unloadOrb failed\n")));
          return result;
        }
      ACE_DEBUG ((LM_INFO, ACE_TEXT ("unloadOrb done\n")));
    }

  ACE_DEBUG ((LM_INFO, ACE_TEXT ("serviceConfig.fini_svcs ...\n")));
  result = service_config.fini_svcs ();
  if (result != 0)
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("serviceConfig.fini_svcs failed\n")));
      return result;
    }
  ACE_DEBUG ((LM_INFO, ACE_TEXT ("serviceConfig.fini_svcs done\n")));

  ACE_DEBUG ((LM_INFO, ACE_TEXT ("serviceConfig.close ...\n")));
  result = service_config.close ();
  if (result != 0)
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("serviceConfig.close failed\n")));
      return result;
    }
  ACE_DEBUG ((LM_INFO, ACE_TEXT ("serviceConfig.close done\n")));
  return 0;
}
