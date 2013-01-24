// $Id$

#include "ace/ARGV.h"
#include "ace/Service_Config.h"
#include "ace/Log_Msg.h"

#include "tao/corba.h"
#include "tao/ORB.h"
#include "tao/PortableServer/PortableServer.h"
#include "orbsvcs/CosNotifyChannelAdminC.h"
#include "orbsvcs/Notify/CosNotify_Service.h"

#include "DllOrb.h"

ACE_TCHAR const * const scpc_loadOrb = ACE_DYNAMIC_VERSIONED_SERVICE_DIRECTIVE(
  "testDllOrb",
  "Bug_3252",
  TAO_VERSION,
  "_make_DllOrb",
  "testDllOrb -ORBDebugLevel 0 -ORBId testDllOrb -NumThreads 2 -ORBDottedDecimalAddresses 1 -ORBCollocationStrategy thru_poa"
);

ACE_TCHAR const * const scpc_unloadOrb = ACE_REMOVE_SERVICE_DIRECTIVE("testDllOrb");

ACE_TCHAR const * const scpc_loadNotifyService = ACE_DYNAMIC_VERSIONED_SERVICE_DIRECTIVE(
  "testNotifyService",
  "TAO_CosNotification_Serv",
  TAO_VERSION,
  "_make_TAO_CosNotify_Service",
  "-UseSeparateDispatchingORB 1"
);

ACE_TCHAR const * const scpc_unloadNotifyService = ACE_REMOVE_SERVICE_DIRECTIVE("testNotifyService");

int
unloadNotify(ACE_Service_Config & r_serviceConfig)
{
  ACE_DEBUG ((LM_INFO, ACE_TEXT ("Unloading NotifyService ...\n")));
  if(0 != r_serviceConfig.process_directive(scpc_unloadNotifyService))
  {
    ACE_DEBUG ((LM_ERROR, ACE_TEXT ("Unloading NotifyService failed\n")));
    return -1;
  }
  ACE_DEBUG ((LM_INFO, ACE_TEXT ("unloading NotifyService done.\n")));
  return 0;
}

int
loadNotify(ACE_Service_Config & r_serviceConfig)
{
  try
  {
    DllOrb * p_orb = ACE_Dynamic_Service<DllOrb>::instance ("testDllOrb");
    CORBA::ORB_var v_orb = p_orb->orb();

    CORBA::Object_var v_poa = v_orb->resolve_initial_references("RootPOA");
    PortableServer::POA_var v_rootPOA = PortableServer::POA::_narrow(v_poa.in ());
    ACE_DEBUG ((LM_INFO, ACE_TEXT ("RootPOA OK.\n")));

    ACE_DEBUG ((LM_INFO, ACE_TEXT ("Loading NotifyService ...\n")));
    if(0 != r_serviceConfig.process_directive(scpc_loadNotifyService))
    {
      ACE_DEBUG ((LM_ERROR, ACE_TEXT ("Loading NotifyService failed\n")));
      return -1;
    }
    ACE_DEBUG ((LM_INFO, ACE_TEXT ("Loading NotifyService done.\n")));

    TAO_CosNotify_Service * p_notifyService =
      ACE_Dynamic_Service<TAO_CosNotify_Service>::instance("testNotifyService");

    p_notifyService->init_service(v_orb.in());

    CosNotifyChannelAdmin::EventChannelFactory_var v_notifyFactory =
      p_notifyService->create(
        v_rootPOA.in()
      );
    ACE_DEBUG ((LM_INFO, ACE_TEXT ("got EventChannelFactory\n")));

    // create an event channel
    CosNotifyChannelAdmin::ChannelID id;

    CosNotification::QoSProperties initialQos;
    CosNotification::AdminProperties initialAdmin;

    CosNotifyChannelAdmin::EventChannel_var v_eventChannel =
      v_notifyFactory->create_channel(
        initialQos,
        initialAdmin,
        id
      );
    ACE_DEBUG ((LM_INFO, ACE_TEXT ("got EventChannel\n")));

    CosNotifyFilter::FilterFactory_var v_filterFactory =
      v_eventChannel->default_filter_factory();
    ACE_DEBUG ((LM_INFO, ACE_TEXT ("got FilterFactory\n")));

    CosNotifyFilter::Filter_var v_filter =
      v_filterFactory->create_filter("ETCL");
    ACE_DEBUG ((LM_INFO, ACE_TEXT ("got Filter\n")));
  }
  catch(CORBA::Exception const & rc_ex)
  {
    rc_ex._tao_print_exception ("Unexpected CORBA Exception: ");
    return -1;
  }
  catch(...)
  {
    ACE_DEBUG ((LM_ERROR, ACE_TEXT ("Unexpected C++ Exception\n")));
    return -1;
  }

  return 0;
}


int unloadOrb(ACE_Service_Config & r_serviceConfig)
{
  ACE_DEBUG ((LM_INFO, ACE_TEXT ("Unloading ORB ...\n")));
  if(0 != r_serviceConfig.process_directive(scpc_unloadOrb))
  {
    ACE_DEBUG ((LM_ERROR, ACE_TEXT ("Unloading ORB failed\n")));
    return -1;
  }
  ACE_DEBUG ((LM_INFO, ACE_TEXT ("unloading ORB done.\n")));
  return 0;
}

int loadOrb(ACE_Service_Config & r_serviceConfig)
{
  ACE_DEBUG ((LM_INFO, ACE_TEXT ("Loading ORB ...\n")));
  if(0 != r_serviceConfig.process_directive(scpc_loadOrb))
  {
    ACE_DEBUG ((LM_ERROR, ACE_TEXT ("Loading ORB failed\n")));
    return -1;
  }
  ACE_DEBUG ((LM_INFO, ACE_TEXT ("Loading ORB done.\n")));
  return 0;
}



int
ACE_TMAIN(int, ACE_TCHAR **argv)
{
  int result = 0;

  ACE_TCHAR signum[64];
  ACE_OS::sprintf(signum, ACE_TEXT("%d"), SIGUSR1);

  ACE_ARGV args;
  args.add(argv[0]);
  args.add(ACE_TEXT("-s"));
  args.add(signum);

  ACE_Service_Config &serviceConfig = *ACE_Service_Config::singleton ();

  result = serviceConfig.open (args.argc(),
                               args.argv(),
                               ACE_DEFAULT_LOGGER_KEY,
                               true, // ignore_static_svcs = 1,
                               true, // ignore_default_svc_conf_file = 0,
                               false  // ignore_debug_flag = 0
                               );

  if(0 != result)
    {
      ACE_DEBUG ((LM_ERROR, ACE_TEXT ("serviceConfig.open failed\n")));
      return result;
    }
  ACE_DEBUG ((LM_INFO, ACE_TEXT ("serviceConfig.open done\n")));

  for(int i = 0; i < 5; ++i)
    {
      ACE_DEBUG ((LM_INFO, ACE_TEXT ("loadOrb ...\n")));
      result = loadOrb(serviceConfig);
      if(0 != result)
        {
          ACE_DEBUG ((LM_ERROR, ACE_TEXT ("loadOrb failed\n")));
          return result;
        }
      ACE_DEBUG ((LM_INFO, ACE_TEXT ("loadOrb done\n")));

      ACE_DEBUG ((LM_INFO, ACE_TEXT ("loadNotify ...\n")));
      result = loadNotify(serviceConfig);
      if(0 != result)
        {
          ACE_DEBUG ((LM_ERROR, ACE_TEXT ("loadNotify failed\n")));
        return result;
        }
      ACE_DEBUG ((LM_INFO, ACE_TEXT ("loadNotify done\n")));

      ACE_DEBUG ((LM_INFO, ACE_TEXT ("unloadNotify ...\n")));
      result = unloadNotify(serviceConfig);
      if(0 != result)
        {
          ACE_DEBUG ((LM_ERROR, ACE_TEXT ("unloadNotify failed\n")));
          return result;
        }
      ACE_DEBUG ((LM_INFO, ACE_TEXT ("unloadNotify done\n")));

      ACE_DEBUG ((LM_INFO, ACE_TEXT ("unloadOrb ...\n")));
      result = unloadOrb(serviceConfig);
      if(0 != result)
        {
          ACE_DEBUG ((LM_ERROR, ACE_TEXT ("unloadOrb failed\n")));
          return result;
        }
      ACE_DEBUG ((LM_INFO, ACE_TEXT ("unloadOrb done\n")));
    }

  ACE_DEBUG ((LM_INFO, ACE_TEXT ("serviceConfig.fini_svcs ...\n")));
  result = serviceConfig.fini_svcs();
  if(0 != result)
    {
      ACE_DEBUG ((LM_ERROR, ACE_TEXT ("serviceConfig.fini_svcs failed\n")));
      return result;
    }
  ACE_DEBUG ((LM_INFO, ACE_TEXT ("serviceConfig.fini_svcs done\n")));

  ACE_DEBUG ((LM_INFO, ACE_TEXT ("serviceConfig.close ...\n")));
  result = serviceConfig.close();
  if(0 != result)
    {
      ACE_DEBUG ((LM_ERROR, ACE_TEXT ("serviceConfig.close failed\n")));
      return result;
    }
  ACE_DEBUG ((LM_INFO, ACE_TEXT ("serviceConfig.close done\n")));

  return 0;
}

