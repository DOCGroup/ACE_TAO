// $Id$

#include "orbsvcs/CosNamingC.h"
#include "orbsvcs/FtRtecEventChannelAdminC.h"
#include "orbsvcs/Event_Utilities.h"
#include "PushConsumer.h"
#include "ace/Get_Opt.h"
#include "orbsvcs/FtRtEvent/Utils/resolve_init.h"
#include "orbsvcs/FtRtEvent/Utils/FTEC_Gateway.h"
#include "orbsvcs/FtRtEvent/Utils/Log.h"

/// include this file to statically linked with FT ORB
#include "orbsvcs/FaultTolerance/FT_ClientService_Activate.h"

/// include this file to statically linked with Transaction Depth
#include "orbsvcs/FtRtEvent/ClientORB/FTRT_ClientORB_Loader.h"

ACE_RCSID (FtRtEvent,
           PushConsumer,
           "$Id$")

CORBA::ORB_var orb;
auto_ptr<TAO_FTRTEC::FTEC_Gateway> gateway;
int NUM_ITERATIONS = 100;

RtecEventChannelAdmin::EventChannel_ptr
get_event_channel(int argc, ACE_TCHAR** argv ACE_ENV_ARG_DECL)
{
    FtRtecEventChannelAdmin::EventChannel_var channel;
    ACE_Get_Opt get_opt (argc, argv, ACE_LIB_TEXT("d:hi:k:n"));
    int opt;
    int use_gateway = 1;

    while ((opt = get_opt ()) != EOF)
    {
      switch (opt)
      {
      case 'd':
        TAO_FTRTEC::Log::level(ACE_OS::atoi(get_opt.opt_arg ()));
        break;
      case 'i':
        {
          CORBA::Object_var obj = orb->string_to_object(get_opt.opt_arg ()
                                                        ACE_ENV_ARG_PARAMETER);
          ACE_CHECK_RETURN(0);
          channel = FtRtecEventChannelAdmin::EventChannel::_narrow(obj.in()
                                                                ACE_ENV_ARG_PARAMETER);
          ACE_CHECK_RETURN(0);
        }
        break;
      case 'k':
        NUM_ITERATIONS = atoi(get_opt.opt_arg ());
        break;
      case 'n':
         use_gateway = 0;
         break;
      case 'h':
      case '?':
        ACE_DEBUG((LM_DEBUG,
                   ACE_LIB_TEXT("Usage: %s ")
                   ACE_LIB_TEXT("-i ftrt_eventchannel_ior\n")
                   ACE_LIB_TEXT("-n       do not use gateway\n")
                   ACE_LIB_TEXT("\n"),
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
        resolve_init<CosNaming::NamingContext>(orb.in(), "NameService"
        ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN(0);

      channel  = resolve<FtRtecEventChannelAdmin::EventChannel>(naming_context.in(),
        name
        ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN(0);
    }

    if (use_gateway)
    {
      gateway.reset(new TAO_FTRTEC::FTEC_Gateway(orb.in(), channel.in()));
      return gateway->_this(ACE_ENV_SINGLE_ARG_PARAMETER);
    }
    else
      return channel._retn();
}

int main(int argc, ACE_TCHAR** argv)
{
  ACE_TRY_NEW_ENV {
    orb = CORBA::ORB_init(argc, argv, ""
      ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;

    RtecEventChannelAdmin::EventChannel_var channel
      = get_event_channel(argc, argv ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;


    if (CORBA::is_nil(channel.in()))
      ACE_ERROR_RETURN((LM_ERROR, "Cannot Find FT_EventService\n"), -1);

    PortableServer::POA_var poa =
      resolve_init<PortableServer::POA>(orb.in(), "RootPOA"
                            ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;

    PortableServer::POAManager_var mgr = poa->the_POAManager(ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_TRY_CHECK;

    mgr->activate(ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_TRY_CHECK;

    PushConsumer_impl push_consumer_impl(orb.in());

    RtecEventChannelAdmin::ConsumerQOS qos;
    qos.is_gateway = 1;
    qos.dependencies.length(1);

    RtecEventComm::EventHeader& h0 =
      qos.dependencies[0].event.header;
    h0.type   = ACE_ES_EVENT_UNDEFINED; // first free event type
    h0.source = ACE_ES_EVENT_SOURCE_ANY;

    RtecEventComm::PushConsumer_var push_consumer =
      push_consumer_impl._this();

    ACE_Time_Value time_val = ACE_OS::gettimeofday ();

    RtecEventChannelAdmin::ConsumerAdmin_var consumer_admin =
      channel->for_consumers(ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_TRY_CHECK;

    push_consumer_impl.supplier_ =
      consumer_admin->obtain_push_supplier(ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_TRY_CHECK;
    push_consumer_impl.supplier_->connect_push_consumer(push_consumer.in(),
      qos   ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;

    time_val = ACE_OS::gettimeofday () - time_val;

    ACE_DEBUG((LM_DEBUG, "connected to proxy_push_supplier, subscription latency = %d\n", time_val.sec () * 10000000 + time_val.usec ()* 10));

    orb->run(ACE_ENV_SINGLE_ARG_PARAMETER);

  }
  ACE_CATCHANY {
    ACE_PRINT_EXCEPTION(ACE_ANY_EXCEPTION, "A CORBA Exception occurred.");
    return -1;
  }
  ACE_ENDTRY;

  return 0;
}

