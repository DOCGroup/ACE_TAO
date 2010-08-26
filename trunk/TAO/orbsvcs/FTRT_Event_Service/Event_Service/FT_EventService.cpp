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
#include "orbsvcs/FtRtEvent/Utils/Log.h"
#include "ace/OS_main.h"

ACE_RCSID (Event_Service,
           FT_EventService,
           "$Id$")

int ACE_TMAIN (int argc, ACE_TCHAR* argv[])
{
  FT_EventService event_service;
  return event_service.run (argc, argv);
}

FT_EventService::FT_EventService()
: global_scheduler_(0)
, sched_impl_(0)
, membership_(TAO_FTEC_Event_Channel::UNSPECIFIED)
, num_threads_(1)
, task_(orb_)
{
}

FT_EventService::~FT_EventService()
{
  delete sched_impl_;
}

int
FT_EventService::run(int argc, ACE_TCHAR* argv[])
{
  try
  {
    // Initialize ORB.
    orb_ = CORBA::ORB_init (argc, argv);

    if (this->parse_args (argc, argv) == -1)
      return 1;

    CORBA::Object_var root_poa_object =
      orb_->resolve_initial_references("RootPOA");
    if (CORBA::is_nil (root_poa_object.in ()))
      ACE_ERROR_RETURN ((LM_ERROR,
      " (%P|%t) Unable to initialize the root POA.\n"),
      1);

    PortableServer::POA_var root_poa =
      PortableServer::POA::_narrow (root_poa_object.in ());

    PortableServer::POAManager_var poa_manager =
      root_poa->the_POAManager ();

    poa_manager->activate ();

    CORBA::Object_var naming_obj =
      orb_->resolve_initial_references ("NameService");
    if (CORBA::is_nil (naming_obj.in ()))
      ACE_ERROR_RETURN ((LM_ERROR,
      " (%P|%t) Unable to initialize the Naming Service.\n"),
      1);

    CosNaming::NamingContext_var naming_context =
      CosNaming::NamingContext::_narrow (naming_obj.in ());

    setup_scheduler(naming_context.in());

    poa_manager->activate();

    // Activate the Event channel implementation

    TAO_FTEC_Event_Channel ec(orb_, root_poa);

    FtRtecEventChannelAdmin::EventChannel_var ec_ior =
      ec.activate(membership_);

    if (report_factory(orb_.in(), ec_ior.in() )==-1)
      return -1;

    orb_->run();
  }
  catch (const CORBA::Exception& ex)
  {
    ex._tao_print_exception ("A CORBA Exception occurred.");
    return -1;
  }


  ACE_Thread_Manager::instance()->wait();
  return 0;
}

int
FT_EventService::parse_args (int argc, ACE_TCHAR* argv [])
{
  /// get the membership from the environment variable
  char* member = ACE_OS::getenv("FTEC_MEMBERSHIP");

  membership_ = TAO_FTEC_Event_Channel::UNSPECIFIED;

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

  ACE_Get_Opt get_opt (argc, argv, ACE_TEXT("d:jn:ps:"));
  int opt;

  while ((opt = get_opt ()) != EOF)
  {
    switch (opt)
    {
    case 'd':
      TAO_FTRTEC::Log::level(ACE_OS::atoi(get_opt.opt_arg ()));
      break;
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

      if (ACE_OS::strcasecmp (get_opt.opt_arg (), ACE_TEXT("global")) == 0)
      {
        this->global_scheduler_ = 1;
      }
      else if (ACE_OS::strcasecmp (get_opt.opt_arg (), ACE_TEXT("local")) == 0)
      {
        this->global_scheduler_ = 0;
      }
      else
      {
        ACE_DEBUG ((LM_DEBUG,
          ACE_TEXT("Unknown scheduling type <%s> ")
          ACE_TEXT("defaulting to local\n"),
          get_opt.opt_arg ()));
        this->global_scheduler_ = 0;
      }
      break;

    case '?':
    default:
      ACE_DEBUG ((LM_DEBUG,
        ACE_TEXT("Usage: %s\n")
        ACE_TEXT("  -j join the object group\n")
        ACE_TEXT("  -p set as primary\n")
        ACE_TEXT("  -s <global|local>\n")
        ACE_TEXT("\n"),
        argv[0]));
      return -1;
    }
  }

  if (this->num_threads_ < 1)
    ACE_ERROR_RETURN((LM_ERROR, "Invalid number of threads specified\n"), -1);

  return 0;
}

void
FT_EventService::setup_scheduler(CosNaming::NamingContext_ptr naming_context)
{
    RtecScheduler::Scheduler_var scheduler;
    if (CORBA::is_nil(naming_context)) {
        ACE_NEW_THROW_EX (this->sched_impl_,
            ACE_Config_Scheduler,
            CORBA::NO_MEMORY());

        scheduler = this->sched_impl_->_this ();

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

                scheduler = this->sched_impl_->_this ();

                // Register the servant with the Naming Context....
                naming_context->rebind (schedule_name, scheduler.in ());
            }
            else
            {
                CORBA::Object_var tmp =
                    naming_context->resolve (schedule_name);

                scheduler = RtecScheduler::Scheduler::_narrow (tmp.in ());
            }
        }
    }
}

int
FT_EventService::report_factory(CORBA::ORB_ptr orb,
                   FtRtecEventChannelAdmin::EventChannel_ptr ec)
{
  try{
    char* addr = ACE_OS::getenv("EventChannelFactoryAddr");

    if (addr != 0) {
      // instaniated by object factory, report my ior back to the factory
      ACE_INET_Addr factory_addr(addr);
      ACE_SOCK_Connector connector;
      ACE_SOCK_Stream stream;

      ACE_DEBUG((LM_DEBUG,"connecting to %s\n",addr));
      if (connector.connect(stream, factory_addr) == -1)
        ACE_ERROR_RETURN((LM_ERROR, "(%P|%t) Invalid Factory Address\n"), -1);

      ACE_DEBUG((LM_DEBUG,"Factory connected\n"));
      CORBA::String_var my_ior_string = orb->object_to_string(ec);

      int len = ACE_OS::strlen(my_ior_string.in()) ;

      if (stream.send_n(my_ior_string.in(), len) != len)
        ACE_ERROR_RETURN((LM_ERROR, "(%P|%t) IOR Transmission Error\n"), -1);

      stream.close();
    }
  }
  catch (...){
    return -1;
  }
  return 0;
}

void FT_EventService::become_primary()
{
  if (this->num_threads_ > 1) {
    task_.activate(THR_NEW_LWP | THR_JOINABLE, num_threads_-1);
  }
}

