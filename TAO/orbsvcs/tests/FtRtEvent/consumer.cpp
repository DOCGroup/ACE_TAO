// $Id$

#include "orbsvcs/CosNamingC.h"
#include "orbsvcs/FtRtecEventChannelAdminC.h"
#include "orbsvcs/Event_Utilities.h"
#include "PushConsumer.h"
#include "ace/Get_Opt.h"
#include "ace/Auto_Ptr.h"
#include "orbsvcs/FtRtEvent/Utils/resolve_init.h"
#include "orbsvcs/FtRtEvent/Utils/FTEC_Gateway.h"

/// include this file to statically linked with FT ORB
#include "orbsvcs/FaultTolerance/FT_ClientService_Activate.h"

/// include this file to statically linked with Transaction Depth
#include "orbsvcs/FtRtEvent/ClientORB/FTRT_ClientORB_Loader.h"

CORBA::ORB_var orb;
auto_ptr<TAO_FTRTEC::FTEC_Gateway> gateway;

RtecEventChannelAdmin::EventChannel_ptr
get_event_channel(int argc, ACE_TCHAR** argv)
{
    FtRtecEventChannelAdmin::EventChannel_var channel;
    ACE_Get_Opt get_opt (argc, argv, ACE_TEXT("hi:n"));
    int opt;
    int use_gateway = 1;

    while ((opt = get_opt ()) != EOF)
    {
      switch (opt)
      {
      case 'i':
        {
          CORBA::Object_var obj = orb->string_to_object(get_opt.opt_arg ());
          channel = FtRtecEventChannelAdmin::EventChannel::_narrow(obj.in());
        }
        break;
      case 'n':
         use_gateway = 0;
         break;
      case 'h':
      case '?':
        ACE_DEBUG((LM_DEBUG,
                   ACE_TEXT("Usage: %s ")
                   ACE_TEXT("-i ftrt_eventchannel_ior\n")
                   ACE_TEXT("-n       do not use gateway\n")
                   ACE_TEXT("\n"),
                      argv[0]));
        return 0;
      }
    }


    if (CORBA::is_nil(channel.in()))
    {
      CosNaming::Name name(1);
      name.length(1);
      name[0].id = CORBA::string_dup("FT_EventService");

      CosNaming::NamingContext_var naming_context =
        resolve_init<CosNaming::NamingContext>(orb.in(), "NameService");

      channel  = resolve<FtRtecEventChannelAdmin::EventChannel>(naming_context.in(),
        name);
    }

    if (use_gateway)
    {
      ACE_AUTO_PTR_RESET (gateway, new TAO_FTRTEC::FTEC_Gateway(orb.in(), channel.in()), TAO_FTRTEC::FTEC_Gateway);
      return gateway->_this();
    }
    else
      return channel._retn();
}

int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try{
    orb = CORBA::ORB_init (argc, argv);

    RtecEventChannelAdmin::EventChannel_var channel
      = get_event_channel (argc, argv);


    if (CORBA::is_nil(channel.in()))
      ACE_ERROR_RETURN ((LM_ERROR, "Cannot Find FT_EventService\n"), -1);

    PortableServer::POA_var poa =
      resolve_init<PortableServer::POA>(orb.in(), "RootPOA");

    PortableServer::POAManager_var mgr = poa->the_POAManager();

    mgr->activate();

    PushConsumer_impl push_consumer_impl(orb.in());
    RtecEventChannelAdmin::ConsumerAdmin_var consumer_admin =
      channel->for_consumers();

    RtecEventChannelAdmin::ProxyPushSupplier_var supplier =
      consumer_admin->obtain_push_supplier();

    RtecEventChannelAdmin::ConsumerQOS qos;
    qos.is_gateway = 1;
    qos.dependencies.length(1);

    RtecEventComm::EventHeader& h0 =
      qos.dependencies[0].event.header;
    h0.type   = ACE_ES_EVENT_UNDEFINED; // first free event type
    h0.source = ACE_ES_EVENT_SOURCE_ANY;

    RtecEventComm::PushConsumer_var push_consumer =
      push_consumer_impl._this();

    supplier->connect_push_consumer(push_consumer.in(),
      qos);

    orb->run();

  }
  catch (const CORBA::Exception& ex){
    ex._tao_print_exception ("A CORBA Exception occurred.");
    return -1;
  }

  return 0;
}
