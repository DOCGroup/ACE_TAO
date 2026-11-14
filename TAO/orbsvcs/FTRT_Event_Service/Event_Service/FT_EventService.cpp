#include "FT_EventService.h"
#include "ace/INET_Addr.h"
#include "ace/Dynamic_Service.h"
#include "ace/Get_Opt.h"
#include "ace/OS_main.h"
#include "ace/OS_NS_strings.h"
#include "ace/SOCK_Connector.h"
#include "ace/SOCK_Stream.h"
#include "ace/Thread_Manager.h"
#include "orbsvcs/Sched/Config_Scheduler.h"
#include "orbsvcs/Scheduler_Factory.h"
#include "orbsvcs/FtRtEvent/EventChannel/FTRTEC_ServiceActivate.h"
#include "orbsvcs/FtRtEvent/Utils/Log.h"
#include "orbsvcs/Log_Macros.h"

#include <cstdlib>
#include <cstring>

int ACE_TMAIN (int argc, ACE_TCHAR *argv [])
{
  FT_EventService event_service;
  return event_service.run (argc, argv);
}

FT_EventService::FT_EventService ()
  : task_ (orb_)
{
}

FT_EventService::~FT_EventService ()
{
  // Keep the scheduler alive until before the ORB and task reference are released.
  this->sched_impl_.reset ();
}

int
FT_EventService::run (int argc, ACE_TCHAR *argv [])
{
  int result {};
  try
    {
      this->orb_ = CORBA::ORB_init (argc, argv);

      if (this->parse_args (argc, argv) == -1)
        result = 1;

      if (result == 0)
        {
          CORBA::Object_var const root_poa_object =
            this->orb_->resolve_initial_references ("RootPOA");
          if (CORBA::is_nil (root_poa_object.in ()))
            {
              ORBSVCS_ERROR ((LM_ERROR,
                              " (%P|%t) Unable to initialize the root POA.\n"));
              result = 1;
            }

          if (result == 0)
            {
              PortableServer::POA_var const root_poa =
                PortableServer::POA::_narrow (root_poa_object.in ());

              PortableServer::POAManager_var const poa_manager = root_poa->the_POAManager ();
              poa_manager->activate ();

              CORBA::Object_var const naming_obj =
                this->orb_->resolve_initial_references ("NameService");
              if (CORBA::is_nil (naming_obj.in ()))
                {
                  ORBSVCS_ERROR ((LM_ERROR,
                                  " (%P|%t) Unable to initialize the Naming Service.\n"));
                  result = 1;
                }

              if (result == 0)
                {
                  CosNaming::NamingContext_var const naming_context =
                    CosNaming::NamingContext::_narrow (naming_obj.in ());

                  this->setup_scheduler (naming_context.in ());

                  TAO_FTEC_Event_Channel ec (this->orb_, root_poa);
                  FtRtecEventChannelAdmin::EventChannel_var const ec_ior =
                    ec.activate (this->membership_);

                  if (this->report_factory (this->orb_.in (), ec_ior.in ()) == -1)
                    result = -1;
                  else
                    this->orb_->run ();
                }
            }
        }
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("A CORBA Exception occurred.");
      result = -1;
    }

  FTRTEC::Fault_Detector_Loader * const detector_loader =
    ACE_Dynamic_Service<FTRTEC::Fault_Detector_Loader>::instance ("FTRTEC_Fault_Detector");
  if (detector_loader != nullptr)
    detector_loader->stop ();

  if (!CORBA::is_nil (this->orb_.in ()))
    this->orb_->destroy ();

  ACE_Thread_Manager::instance ()->wait ();
  return result;
}

int
FT_EventService::parse_args (int argc, ACE_TCHAR *argv [])
{
  this->membership_ = TAO_FTEC_Event_Channel::UNSPECIFIED;
  this->num_threads_ = 1;

  char const * const member = std::getenv ("FTEC_MEMBERSHIP");
  if (member != nullptr)
    {
      if (ACE_OS::strcasecmp (member, "PRIMARY") == 0)
        this->membership_ = TAO_FTEC_Event_Channel::PRIMARY;
      else if (ACE_OS::strcasecmp (member, "BACKUP") == 0)
        this->membership_ = TAO_FTEC_Event_Channel::BACKUP;
    }

  char const * const n_threads = std::getenv ("FTEC_NUM_THREAD");
  if (n_threads != nullptr)
    this->num_threads_ = std::atoi (n_threads);

  ACE_Get_Opt get_opt (argc, argv, ACE_TEXT ("d:jn:ps:"));
  int opt {};

  while ((opt = get_opt ()) != EOF)
    {
      switch (opt)
        {
        case 'd':
          TAO_FTRTEC::Log::level (ACE_OS::atoi (get_opt.opt_arg ()));
          break;
        case 'j':
          this->membership_ = TAO_FTEC_Event_Channel::BACKUP;
          break;
        case 'n':
          this->num_threads_ = ACE_OS::atoi (get_opt.opt_arg ());
          break;
        case 'p':
          this->membership_ = TAO_FTEC_Event_Channel::PRIMARY;
          break;
        case 's':
          if (ACE_OS::strcasecmp (get_opt.opt_arg (), ACE_TEXT ("global")) == 0)
            this->global_scheduler_ = true;
          else if (ACE_OS::strcasecmp (get_opt.opt_arg (), ACE_TEXT ("local")) == 0)
            this->global_scheduler_ = false;
          else
            {
              ORBSVCS_DEBUG ((LM_DEBUG,
                              ACE_TEXT ("Unknown scheduling type <%s> ")
                              ACE_TEXT ("defaulting to local\n"),
                              get_opt.opt_arg ()));
              this->global_scheduler_ = false;
            }
          break;

        case '?':
        default:
          ORBSVCS_DEBUG ((LM_DEBUG,
                          ACE_TEXT ("Usage: %s\n")
                          ACE_TEXT ("  -j join the object group\n")
                          ACE_TEXT ("  -p set as primary\n")
                          ACE_TEXT ("  -s <global|local>\n\n"),
                          argv[0]));
          return -1;
        }
    }

  if (this->num_threads_ < 1)
    ORBSVCS_ERROR_RETURN ((LM_ERROR, "Invalid number of threads specified\n"), -1);

  return 0;
}

void
FT_EventService::setup_scheduler (CosNaming::NamingContext_ptr naming_context)
{
  RtecScheduler::Scheduler_var scheduler;
  if (CORBA::is_nil (naming_context))
    {
      ACE_Config_Scheduler *scheduler_impl {};
      ACE_NEW_THROW_EX (scheduler_impl,
                        ACE_Config_Scheduler,
                        CORBA::NO_MEMORY ());
      this->sched_impl_.reset (scheduler_impl);

      scheduler = scheduler_impl->_this ();

      if (ACE_Scheduler_Factory::server (scheduler.in ()) == -1)
        ORBSVCS_ERROR ((LM_ERROR, "Unable to install scheduler\n"));
    }
  else
    {
      CosNaming::Name schedule_name {};
      schedule_name.length (1);
      schedule_name[0].id = CORBA::string_dup ("ScheduleService");

      if (!this->global_scheduler_)
        {
          ACE_Config_Scheduler *scheduler_impl {};
          ACE_NEW_THROW_EX (scheduler_impl,
                            ACE_Config_Scheduler,
                            CORBA::NO_MEMORY ());
          this->sched_impl_.reset (scheduler_impl);

          scheduler = scheduler_impl->_this ();
          naming_context->rebind (schedule_name, scheduler.in ());
        }
      else
        {
          CORBA::Object_var const scheduler_object =
            naming_context->resolve (schedule_name);
          scheduler = RtecScheduler::Scheduler::_narrow (scheduler_object.in ());
        }
    }
}

int
FT_EventService::report_factory (CORBA::ORB_ptr orb,
                                 FtRtecEventChannelAdmin::EventChannel_ptr ec)
{
  try
    {
      char const * const addr = std::getenv ("EventChannelFactoryAddr");
      if (addr != nullptr)
        {
          ACE_INET_Addr const factory_addr (addr);
          ACE_SOCK_Connector connector;
          ACE_SOCK_Stream stream;

          ORBSVCS_DEBUG ((LM_DEBUG, "connecting to %s\n", addr));
          if (connector.connect (stream, factory_addr) == -1)
            ORBSVCS_ERROR_RETURN ((LM_ERROR, "(%P|%t) Invalid Factory Address\n"), -1);

          ORBSVCS_DEBUG ((LM_DEBUG, "Factory connected\n"));
          CORBA::String_var const my_ior_string = orb->object_to_string (ec);

          size_t const len = std::strlen (my_ior_string.in ());
          ssize_t const bytes_sent = stream.send_n (my_ior_string.in (), len);
          if (bytes_sent < 0 || static_cast<size_t> (bytes_sent) != len)
            ORBSVCS_ERROR_RETURN ((LM_ERROR, "(%P|%t) IOR Transmission Error\n"), -1);

          stream.close ();
        }
    }
  catch (...)
    {
      return -1;
    }
  return 0;
}

void
FT_EventService::become_primary ()
{
  if (this->num_threads_ > 1)
    this->task_.activate (THR_NEW_LWP | THR_JOINABLE, this->num_threads_ - 1);
}

