// $Id$
#include "orbsvcs/CosNamingC.h"
#include "orbsvcs/FtRtecEventChannelAdminC.h"
#include "PushSupplier.h"
#include "ace/Get_Opt.h"
#include "orbsvcs/FtRtEvent/Utils/resolve_init.h"
#include "orbsvcs/FtRtEvent/Utils/FTEC_Gateway.h"

/// include this file to statically linked with FT ORB
#include "orbsvcs/FaultTolerance/FT_ClientService_Activate.h"

/// include this file to statically linked with Transaction Depth
#include "orbsvcs/FtRtEvent/ClientORB/FTRT_ClientORB_Loader.h"

ACE_RCSID (supplier,
           supplier.cpp,
           "$Id$")


ACE_Time_Value timer_interval(1,0);
CORBA::ORB_var orb;
auto_ptr<TAO_FTRTEC::FTEC_Gateway> gateway;

RtecEventChannelAdmin::EventChannel_ptr
get_event_channel(int argc, ACE_TCHAR** argv ACE_ENV_ARG_DECL)
{
    FtRtecEventChannelAdmin::EventChannel_var channel;
	  ACE_Get_Opt get_opt (argc, argv, ACE_LIB_TEXT("gi:"));
	  int opt;
    int use_gateway = 0;

    while ((opt = get_opt ()) != EOF)
    {
      switch (opt)
      {
      case 'g':
         use_gateway = 1;
         break;
      case 'i':
        {
          CORBA::Object_var obj = orb->string_to_object(get_opt.opt_arg ()
                                                        ACE_ENV_ARG_PARAMETER);
          ACE_CHECK;
          channel = FtRtecEventChannelAdmin::EventChannel::_narrow(obj.in()
                                                                ACE_ENV_ARG_PARAMETER);
          ACE_CHECK;
        }
        break;
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
      ACE_CHECK;

      channel	= resolve<FtRtecEventChannelAdmin::EventChannel>(naming_context.in(),
        name
        ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }

    if (use_gateway)
    {
      gateway.reset(new TAO_FTRTEC::FTEC_Gateway(orb.in(), channel.in()));
      return gateway->_this();
    }
    else
      return channel._retn();
}


int main(int argc, ACE_TCHAR** argv)
{
	ACE_DECLARE_NEW_CORBA_ENV;
	ACE_TRY {
      orb = CORBA::ORB_init(argc, argv
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


		if (argc == 2) {
			timer_interval.set(atof(argv[1]));
		}

		PushSupplier_impl push_supplier(orb.in());
		if (push_supplier.init(channel.in() ACE_ENV_ARG_PARAMETER) == -1)
			return -1;

		RtecEventComm::PushSupplier_var
			supplier = push_supplier._this();


		orb->run(ACE_ENV_SINGLE_ARG_PARAMETER);

	}
	ACE_CATCHANY {
			ACE_PRINT_EXCEPTION(ACE_ANY_EXCEPTION, "A CORBA Exception occurred.");
	}
	ACE_ENDTRY;

    ACE_CHECK_RETURN(-1);

	return 0;
}
