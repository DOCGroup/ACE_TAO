// $Id$

#include "FT_EventService.h"
#include "ace/Argv_Type_Converter.h"
#include "ace/Thread_Manager.h"
#include "ace/Get_Opt.h"
#include "ace/INET_Addr.h"
#include "ace/SOCK_Connector.h"
#include "ace/SOCK_Stream.h"
#include "orbsvcs/Sched/Config_Scheduler.h" 
#include "orbsvcs/Scheduler_Factory.h"
#include "orbsvcs/FtRtEvent/EventChannel/FTRTEC_ServiceActivate.h"

ACE_RCSID (FTRT_Event_Service,
           FT_EventService.cpp,
           "$Id$")


int ACE_TMAIN (int argc, ACE_TCHAR* argv[])
{
  FT_EventService event_service;
  return event_service.run (argc, argv);
}
 
FT_EventService::FT_EventService()
: global_scheduler_(0)
, sched_impl_(0)
, membership_(TAO_FTEC_Event_Channel::NONE)
, num_threads_(1)
{
}

FT_EventService::~FT_EventService()
{
  delete sched_impl_;
}

int
FT_EventService::run(int argc, ACE_TCHAR* argv[])
{
  ACE_TRY_NEW_ENV
  {
    // Make a copy of command line parameter.
    ACE_Argv_Type_Converter command(argc, argv);

    // Initialize ORB.
    CORBA::ORB_var orb =
      CORBA::ORB_init (command.get_argc(), command.get_ASCII_argv(), "" ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;

    if (this->parse_args (command.get_argc(), command.get_TCHAR_argv()) == -1)
      return 1;

    CORBA::Object_var root_poa_object =
      orb->resolve_initial_references("RootPOA"
      ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;
    if (CORBA::is_nil (root_poa_object.in ()))
      ACE_ERROR_RETURN ((LM_ERROR,
      " (%P|%t) Unable to initialize the root POA.\n"),
      1);

    PortableServer::POA_var root_poa =
      PortableServer::POA::_narrow (root_poa_object.in () ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;

    PortableServer::POAManager_var poa_manager =
      root_poa->the_POAManager (ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_TRY_CHECK;

    poa_manager->activate (ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_TRY_CHECK;

    CORBA::Object_var naming_obj =
      orb->resolve_initial_references ("NameService" ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;
    if (CORBA::is_nil (naming_obj.in ()))
      ACE_ERROR_RETURN ((LM_ERROR,
      " (%P|%t) Unable to initialize the Naming Service.\n"),
      1);

    CosNaming::NamingContext_var naming_context =
      CosNaming::NamingContext::_narrow (naming_obj.in () ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;

    setup_scheduler(naming_context.in() ACE_ENV_ARG_PARAMETER);
    ACE_CHECK_RETURN(-1);


    poa_manager->activate(ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_TRY_CHECK;

    // Activate the Event channel implementation

    TAO_FTEC_Event_Channel ec(orb, root_poa);

    FtRtecEventChannelAdmin::EventChannel_var ec_ior =
      ec.activate(membership_ 
        ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_TRY_CHECK;

    if (report_factory(orb.in(), ec_ior.in())==-1)
      return -1;

    orb->run(ACE_ENV_SINGLE_ARG_PARAMETER);
  }
  ACE_CATCHANY
  {
    ACE_PRINT_EXCEPTION(ACE_ANY_EXCEPTION, "A CORBA Exception occurred.");
  }
  ACE_ENDTRY;

  ACE_CHECK_RETURN(-1);

  ACE_Thread_Manager::instance()->wait();
  return 0;
}

int
FT_EventService::parse_args (int argc, ACE_TCHAR* argv [])
{
  char* member = ACE_OS::getenv("FTEC_MEMBERSHIP");

  membership_ = TAO_FTEC_Event_Channel::NONE;

  if (member) {
    if (ACE_OS::strcasecmp(member, "PRIMARY")==0) {
      membership_ = TAO_FTEC_Event_Channel::PRIMARY;
    }
    else if (ACE_OS::strcasecmp(member, "BACKUP")==0) {
      membership_ = TAO_FTEC_Event_Channel::BACKUP;
    }
  }

  char* n_threads = ACE_OS::getenv("FTEC_NUM_THREAD");

  this->num_threads_ = 1;
  if (n_threads) 
    this->num_threads_ = ACE_OS::atoi(n_threads); 

  ACE_Get_Opt get_opt (argc, argv, ACE_LIB_TEXT("jn:ps:"));
	int opt;

	while ((opt = get_opt ()) != EOF)
	{
		switch (opt)
		{
		case 'j':
      this->membership_ = TAO_FTEC_Event_Channel::BACKUP;
			break;
    case 'n':
      this->num_threads_ = ACE_OS::atoi(get_opt.opt_arg ()); 
      break;
		case 'p':
			this->membership_ = TAO_FTEC_Event_Channel::PRIMARY;
			break;
		case 's':
			// It could be just a flag (i.e. no "global" or "local"
			// argument, but this is consistent with the EC_Multiple
			// test and also allows for a runtime scheduling service.

			if (ACE_OS::strcasecmp (get_opt.opt_arg (), ACE_LIB_TEXT("global")) == 0)
			{
				this->global_scheduler_ = 1;
			}
			else if (ACE_OS::strcasecmp (get_opt.opt_arg (), ACE_LIB_TEXT("local")) == 0)
			{
				this->global_scheduler_ = 0;
			}
			else
			{
				ACE_DEBUG ((LM_DEBUG,
					ACE_LIB_TEXT("Unknown scheduling type <%s> ")
					ACE_LIB_TEXT("defaulting to local\n"),
					get_opt.opt_arg ()));
				this->global_scheduler_ = 0;
			}
			break;

		case '?':
		default:
			ACE_DEBUG ((LM_DEBUG,
				ACE_LIB_TEXT("Usage: %s \n")
				ACE_LIB_TEXT("  -j join the object group\n")
				ACE_LIB_TEXT("  -p set as primary\n")
				ACE_LIB_TEXT("  -s <global|local> \n")
				ACE_LIB_TEXT("\n"),
				argv[0]));
			return -1;
		}
	}

  if (this->num_threads_ < 1) 
    ACE_ERROR_RETURN((LM_ERROR, "Invalid number of threads specified\n"), -1);

	return 0;
}

void
FT_EventService::setup_scheduler(CosNaming::NamingContext_ptr naming_context
                                        ACE_ENV_ARG_DECL_WITH_DEFAULTS)
{
    RtecScheduler::Scheduler_var scheduler;
    if (CORBA::is_nil(naming_context)) {
        ACE_NEW_THROW_EX (this->sched_impl_,
            ACE_Config_Scheduler,
            CORBA::NO_MEMORY());
        
        scheduler = this->sched_impl_->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
        ACE_TRY_CHECK;
        
        if (ACE_Scheduler_Factory::server(scheduler.in()) == -1)
            ACE_ERROR((LM_ERROR,"Unable to install scheduler\n"));
    }
    else {
        // This is the name we (potentially) register the Scheduling
        // Service in the Naming Service.
        CosNaming::Name schedule_name (1);
        schedule_name.length (1);
        schedule_name[0].id = CORBA::string_dup ("ScheduleService");
        
        
        if (1) 
        {
            // We must find the scheduler object reference...
            
            if (this->global_scheduler_ == 0)
            {
                ACE_NEW_THROW_EX (this->sched_impl_,
                    ACE_Config_Scheduler,
                    CORBA::NO_MEMORY());
                
                scheduler = this->sched_impl_->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
                ACE_CHECK;
                
                // Register the servant with the Naming Context....
                naming_context->rebind (schedule_name, scheduler.in ()
                    ACE_ENV_ARG_PARAMETER);
                ACE_CHECK;
            }
            else
            {
                CORBA::Object_var tmp =
                    naming_context->resolve (schedule_name ACE_ENV_ARG_PARAMETER);
                ACE_CHECK;
                
                scheduler = RtecScheduler::Scheduler::_narrow (tmp.in ()
                    ACE_ENV_ARG_PARAMETER);
                ACE_CHECK;
            }
        }
    }
}

int 
FT_EventService::report_factory(CORBA::ORB_ptr orb,
                   FtRtecEventChannelAdmin::EventChannel_ptr ec)
{
    char* addr = ACE_OS::getenv("EventChannelFactoryAddr");

		if (addr != NULL) {
      // instaniated by object factory, report my ior back to the factory           
      ACE_INET_Addr factory_addr(addr);
      ACE_SOCK_Connector connector;
      ACE_SOCK_Stream stream;

      ACE_DEBUG((LM_DEBUG,"connecting to %s\n",addr));
      if (connector.connect(stream, factory_addr) == -1)
        ACE_ERROR_RETURN((LM_ERROR, "(%P|%t) Invalid Factory Address\n"), -1);

      ACE_DEBUG((LM_DEBUG,"Factory connected\n"));
      CORBA::String_var my_ior_string = orb->object_to_string(ec 
        ACE_ENV_ARG_PARAMETER);

      ACE_TRY_CHECK;
      int len = strlen(my_ior_string.in()) ;

      if (stream.send_n(my_ior_string.in(), len) != len)
        ACE_ERROR_RETURN((LM_ERROR, "(%P|%t) IOR Transmission Error\n"), -1);

      stream.close();
    }
    return 0;
}


