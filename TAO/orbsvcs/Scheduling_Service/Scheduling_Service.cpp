// $Id$

#include "Scheduling_Service.h"

#include "ace/Get_Opt.h"
#include "ace/Auto_Ptr.h"
#include "ace/Argv_Type_Converter.h"
#include "orbsvcs/CosNamingC.h"
#include "ace/OS_main.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_strings.h"

// Default Constructor.

TAO_Scheduling_Service::TAO_Scheduling_Service (void)
  : scheduler_impl_ (0),
    service_name_ ("ScheduleService"),
    scheduler_type_ (CONFIG)
{
}


// Constructor taking the command-line arguments.

TAO_Scheduling_Service::TAO_Scheduling_Service (int argc, ACE_TCHAR* argv[])
  : scheduler_impl_ (0),
    service_name_ ("ScheduleService"),
    scheduler_type_ (CONFIG)
{
  this->init (argc, argv);
}

// Destructor.

TAO_Scheduling_Service::~TAO_Scheduling_Service (void)
{
}


// Initialize the Scheduling Service with the arguments.

int
TAO_Scheduling_Service::init (int argc, ACE_TCHAR* argv[])
{
  int result;
  CORBA::ORB_var orb;
  PortableServer::POAManager_ptr poa_manager;

  try
    {
      // Copy command line parameter.
      ACE_Argv_Type_Converter command_line(argc, argv);

      // Initialize ORB manager.
      this->orb_manager_.init (command_line.get_argc(), command_line.get_TCHAR_argv());

      orb = this->orb_manager_.orb ();

      poa_manager = this->orb_manager_.poa_manager ();

      poa_manager->activate ();

      // Check the non-ORB arguments.  this needs to come before we
      // initialize the scheduler implementation so that we know which
      // type of scheduler to use.

      result = this->parse_args (command_line.get_argc(), command_line.get_TCHAR_argv());
      if (result < 0)
        return result;

      // Construct a scheduler implementation of the specified type.
      switch (this->scheduler_type_)
        {
          case RECONFIG:
            ACE_NEW_THROW_EX (scheduler_impl_,
                              RECONFIG_SCHED_TYPE,
                              CORBA::NO_MEMORY ());
            break;

          case CONFIG:
            ACE_NEW_THROW_EX (scheduler_impl_,
                              CONFIG_SCHED_TYPE,
                              CORBA::NO_MEMORY ());
            break;

          default:
            ACE_ERROR_RETURN ((LM_ERROR,
                               "TAO_Scheduling_Service::init: "
                               "unrecognized Scheduler_Type"), -1);
        }

      // Locate the naming service.
      CORBA::Object_var naming_obj =
        orb->resolve_initial_references ("NameService");

      if (CORBA::is_nil (naming_obj.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to locate the Naming Service.\n"),
                          -1);
      CosNaming::NamingContext_var naming_context =
        CosNaming::NamingContext::_narrow (naming_obj.in ());

      RtecScheduler::Scheduler_var scheduler =
        this->scheduler_impl_->_this ();

      CORBA::String_var scheduler_ior_string =
        orb->object_to_string (scheduler.in ());

      ACE_DEBUG ((LM_DEBUG, ACE_TEXT("The scheduler IOR is <%C>\n"),
                            scheduler_ior_string.in ()));

      // Register the servant with the Naming Context....
      CosNaming::Name schedule_name (1);
      schedule_name.length (1);
      schedule_name[0].id = CORBA::string_dup (this->service_name_.rep());
      naming_context->rebind (schedule_name, scheduler.in ());

      if (this->ior_file_name_.rep() != 0)
        {
          FILE *iorf = ACE_OS::fopen (this->ior_file_name_.rep(), "w");
          if (iorf != 0)
            {
              ACE_OS::fprintf (iorf,
                               "%s\n",
                               scheduler_ior_string.in ());
              ACE_OS::fclose (iorf);
            }
        }

      if (this->pid_file_name_.rep() != 0)
        {
          FILE *pidf = ACE_OS::fopen (this->pid_file_name_.rep(), "w");
          if (pidf != 0)
            {
              ACE_OS::fprintf (pidf,
                               ACE_TEXT("%ld\n"),
                               static_cast<long> (ACE_OS::getpid ()));
              ACE_OS::fclose (pidf);
            }
        }
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("TAO_Scheduling_Service::init");
      return -1;
    }

  return 0;
}


// Runs the TAO_Scheduling_Service.

int
TAO_Scheduling_Service::run (void)
{
  // Run the ORB manager.
  return this->orb_manager_.run ();
}


// Parses the command line arguments.

int
TAO_Scheduling_Service::parse_args (int argc, ACE_TCHAR* argv[])
{
  ACE_Get_Opt get_opt (argc, argv, ACE_TEXT("n:p:o:s:"));
  int opt;

  while ((opt = get_opt ()) != EOF)
    {
      switch (opt)
        {
        case 'n':
          this->service_name_ = ACE_TEXT_ALWAYS_CHAR(get_opt.opt_arg ());
          break;

        case 'p':
          this->pid_file_name_ = ACE_TEXT_ALWAYS_CHAR(get_opt.opt_arg ());
          break;

        case 'o':
          this->ior_file_name_ = ACE_TEXT_ALWAYS_CHAR(get_opt.opt_arg ());
          break;

        case 's':
          if (ACE_OS::strcasecmp (ACE_TEXT("CONFIG"), get_opt.optarg) == 0)
            {
              this->scheduler_type_ = CONFIG;
            }
          else if (ACE_OS::strcasecmp (ACE_TEXT("RECONFIG"), get_opt.optarg) == 0)
            {
              this->scheduler_type_ = RECONFIG;
            }
          else
            {
              ACE_DEBUG ((LM_DEBUG,
                          "Usage: %s "
                          "[-n service_name] "
                          "[-p pid_file_name] "
                          "[-o ior_file_name] "
                          "[-s <CONFIG | reconfig>]"
                          "\n",
                          argv[0]));

              return -1;
            }
          break;

        case '?':
        default:

          ACE_DEBUG ((LM_DEBUG,
                      "Usage: %s "
                      "[-n service_name] "
                      "[-p pid_file_name] "
                      "[-o ior_file_name] "
                      "[-s <CONFIG | reconfig>]"
                      "\n",
                      argv[0]));

          return -1;
        }
    }

  return 0;
}

int ACE_TMAIN (int argc, ACE_TCHAR* argv[])
{
  try
    {
      TAO_Scheduling_Service scheduling_service;

      ACE_DEBUG ((LM_DEBUG,
                  "%s; initializing scheduling service\n", __FILE__));

      if (scheduling_service.init (argc, argv) < 0)
        ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "init"), 1);

      ACE_DEBUG ((LM_DEBUG,
                  "%s; running scheduling service\n", __FILE__));

      scheduling_service.run ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("schedule_service");
          return 1;
    }

  return 0;
}
