#include "FtRtEvent_Test.h"
#include "orbsvcs/CosNamingC.h"
#include "orbsvcs/FtRtecEventChannelAdminC.h"
#include "ace/Get_Opt.h"
#include "orbsvcs/FtRtEvent/Utils/resolve_init.h"
#include "orbsvcs/FtRtEvent/Utils/FTEC_Gateway.h"
#include "orbsvcs/FtRtEvent/Utils/Log.h"
/// include this file to statically linked with FT ORB
#include "orbsvcs/FaultTolerance/FT_ClientService_Activate.h"

/// include this file to statically linked with Transaction Depth
#include "orbsvcs/FtRtEvent/ClientORB/FTRT_ClientORB_Loader.h"

ACE_RCSID (FtRtEvent,
           FtRtEvent_Test,
           "$Id$")

FtRtEvent_Test_Base::FtRtEvent_Test_Base()
: gateway_(0)
, timer_interval_(1,0)
, num_iterations_(100)
, use_gateway_(1)
{
}

FtRtEvent_Test_Base::~FtRtEvent_Test_Base()
{
  delete gateway_;
}

int 
FtRtEvent_Test_Base::parse_args(int argc, ACE_TCHAR** argv ACE_ENV_ARG_DECL)
{
  ACE_Get_Opt get_opt (argc, argv, ACE_LIB_TEXT("d:f:hi:k:n?"));
  int opt;

  while ((opt = get_opt ()) != EOF)
  {
    switch (opt)
    {
    case 'd':
      TAO_FTRTEC::Log::level(ACE_OS::atoi(get_opt.opt_arg ()));
      break;
    case 'i':
      {
        CORBA::Object_var obj = orb_->string_to_object(get_opt.opt_arg ()
          ACE_ENV_ARG_PARAMETER);
        ACE_CHECK_RETURN(0);
        channel_ = FtRtecEventChannelAdmin::EventChannel::_narrow(obj.in()
          ACE_ENV_ARG_PARAMETER);
        ACE_CHECK_RETURN(0);
      }
      break;
    case 'k':
      num_iterations_ = atoi(get_opt.opt_arg ());
      break;
    case 'n':
      use_gateway_ = 0;
      break;
    case 'f':
      timer_interval_.set(1.0/ACE_OS::atoi(get_opt.opt_arg ()));
      break;
    case 'h':
    case '?':
      ACE_DEBUG((LM_DEBUG,
        ACE_LIB_TEXT("Usage: %s ")
        ACE_LIB_TEXT("-i ftrt_eventchannel_ior\n")
        ACE_LIB_TEXT("-n       do not use gateway\n")
        ACE_LIB_TEXT("-f event_frequency  in HZ (default 1 HZ)\n")
        ACE_LIB_TEXT("\n"),
        argv[0]));
      return -1;

    }
  }
  return 0;
}

RtecEventChannelAdmin::EventChannel_ptr
FtRtEvent_Test_Base::get_event_channel(ACE_ENV_SINGLE_ARG_DECL)
{
  FtRtecEventChannelAdmin::EventChannel_var channel;
  if (CORBA::is_nil(channel.in()))
  {
    /// Find the FTRTEC from the Naming Service
    CosNaming::Name name(1);
    name.length(1);
    name[0].id = CORBA::string_dup("FT_EventService");

    CosNaming::NamingContext_var naming_context =
      resolve_init<CosNaming::NamingContext>(orb_.in(), "NameService"
      ACE_ENV_ARG_PARAMETER);
    ACE_CHECK_RETURN(0);

    channel  = resolve<FtRtecEventChannelAdmin::EventChannel>(naming_context.in(),
      name
      ACE_ENV_ARG_PARAMETER);
    ACE_CHECK_RETURN(0);
  }

  if (use_gateway_)
  {
    // use local gateway to communicate with FTRTEC
    gateway_ = new TAO_FTRTEC::FTEC_Gateway(orb_.in(), channel.in());
    return gateway_->_this(ACE_ENV_SINGLE_ARG_PARAMETER);
  }
  return channel._retn();
}


int 
FtRtEvent_Test_Base::run(int argc, ACE_TCHAR** argv)
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY {
    orb_ = CORBA::ORB_init(argc, argv, ""
      ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;

    if (parse_args(argc, argv ACE_ENV_ARG_PARAMETER) == -1) 
      return -1;
    ACE_TRY_CHECK;

    channel_ = get_event_channel(ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_TRY_CHECK;

    if (CORBA::is_nil(channel_.in()))
      return -1;

    PortableServer::POA_var poa =
      resolve_init<PortableServer::POA>(orb_.in(), "RootPOA"
      ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;

    PortableServer::POAManager_var mgr = poa->the_POAManager(ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_TRY_CHECK;

    mgr->activate(ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_TRY_CHECK;

    if (this->init(ACE_ENV_SINGLE_ARG_PARAMETER) == -1)
      return -1;
    ACE_TRY_CHECK;

    orb_->run(ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_TRY_CHECK;

  }
  ACE_CATCHANY {
    ACE_PRINT_EXCEPTION(ACE_ANY_EXCEPTION, "A CORBA Exception occurred.");
  }
  ACE_ENDTRY;

  ACE_CHECK_RETURN(-1);

  return 0;
}
