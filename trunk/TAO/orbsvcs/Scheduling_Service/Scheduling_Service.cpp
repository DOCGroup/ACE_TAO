// $Id$

#include "ace/Get_Opt.h"
#include "orbsvcs/CosNamingC.h"

#include "Scheduling_Service.h"

ACE_RCSID(Scheduling_Service, Scheduling_Service, "$Id$")

// Default Constructor.

TAO_Scheduling_Service::TAO_Scheduling_Service (void)
  : scheduler_impl_ (0),
    ior_file_name_ (0),
    pid_file_name_ (0),
    service_name_ ("ScheduleService"),
    scheduler_type_ (CONFIG)
{
}


// Constructor taking the command-line arguments.

TAO_Scheduling_Service::TAO_Scheduling_Service (int argc, char *argv[])
  : scheduler_impl_ (0),
    ior_file_name_ (0),
    pid_file_name_ (0),
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
TAO_Scheduling_Service::init (int argc, char *argv[])
{
  int result;
  CORBA::ORB_var orb;
  PortableServer::POA_ptr root_poa;
  PortableServer::POAManager_ptr poa_manager;

  ACE_TRY_NEW_ENV
    {
      // Initialize ORB manager.
      this->orb_manager_.init (argc, argv, ACE_TRY_ENV);
      ACE_TRY_CHECK;

      orb = this->orb_manager_.orb ();
      ACE_TRY_CHECK;

      root_poa = this->orb_manager_.root_poa ();
      ACE_TRY_CHECK;

      poa_manager = this->orb_manager_.poa_manager ();
      ACE_TRY_CHECK;

      poa_manager->activate (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // Check the non-ORB arguments.  this needs to come before we
      // initialize the scheduler implementation so that we know which
      // type of scheduler to use.

      result = this->parse_args (argc, argv);
      if (result < 0)
        return result;

      // Construct a scheduler implementation of the specified type. 
      switch (this->scheduler_type_)
	{

// The templatized method parameters needed by the reconfig scheduler
// class template are hopelessly broken on pre-2.8 versions of g++.
#if (! defined (__GNUC__)) || (__GNUC__ > 2) || \
(__GNUC__ == 2 && defined (__GNUC_MINOR__) && __GNUC_MINOR__ >= 8)

          case RECONFIG:
            ACE_NEW_THROW_EX (scheduler_impl_,
                              RECONFIG_SCHED_TYPE,
                              CORBA::NO_MEMORY ());
            ACE_TRY_CHECK;
            break;

#endif  /* __GNUC__ */

          case CONFIG:
            ACE_NEW_THROW_EX (scheduler_impl_,
                              CONFIG_SCHED_TYPE,
                              CORBA::NO_MEMORY ());
            ACE_TRY_CHECK;
            break;

          default:
            ACE_ERROR_RETURN ((LM_ERROR, 
                               "TAO_Naming_Service::init: "
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
        CosNaming::NamingContext::_narrow (naming_obj.in (), ACE_TRY_ENV);
      ACE_TRY_CHECK;

      RtecScheduler::Scheduler_var scheduler =
        this->scheduler_impl_->_this (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      CORBA::String_var scheduler_ior_string =
        orb->object_to_string (scheduler.in (), ACE_TRY_ENV);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG, "The scheduler IOR is <%s>\n",
                            scheduler_ior_string.in ()));

      // Register the servant with the Naming Context....
      CosNaming::Name schedule_name (1);
      schedule_name.length (1);
      schedule_name[0].id = CORBA::string_dup (this->service_name_);
      naming_context->rebind (schedule_name, scheduler.in (), ACE_TRY_ENV);
      ACE_TRY_CHECK;

      if (this->ior_file_name_ != 0)
        {
          FILE *iorf = fopen (this->ior_file_name_, "w");
          if (iorf != 0)
            {
              ACE_OS::fprintf (iorf,
                               "%s\n",
                               scheduler_ior_string.in ());
              ACE_OS::fclose (iorf);
            }
        }

      if (this->pid_file_name_ != 0)
        {
          FILE *pidf = fopen (this->pid_file_name_, "w");
          if (pidf != 0)
            {
              ACE_OS::fprintf (pidf,
                               "%d\n",
                               ACE_OS::getpid ());
              ACE_OS::fclose (pidf);
            }
        }
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "TAO_Naming_Service::init");
      return -1;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (-1);

  return 0;
}


// Runs the TAO_Scheduling_Service.

int
TAO_Scheduling_Service::run (CORBA_Environment& ACE_TRY_ENV)
{
  // Run the ORB manager.
  return this->orb_manager_.run (ACE_TRY_ENV);
}


// Parses the command line arguments.

int
TAO_Scheduling_Service::parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opt (argc, argv, "n:p:s:");
  int opt;

  while ((opt = get_opt ()) != EOF)
    {
      switch (opt)
        {
        case 'n':
          this->service_name_ = get_opt.optarg;
          break;

        case 'p':
          this->pid_file_name_ = get_opt.optarg;
          break;

        case 'o':
          this->ior_file_name_ = get_opt.optarg;
          break;

// The templatized method parameters needed by the reconfig scheduler
// class template are hopelessly broken on pre-2.8 versions of g++.
#if (! defined (__GNUC__)) || (__GNUC__ > 2) || \
(__GNUC__ == 2 && defined (__GNUC_MINOR__) && __GNUC_MINOR__ >= 8)

        case 's':
          if (ACE_OS::strcasecmp ("CONFIG", get_opt.optarg) == 0)
            {
              this->scheduler_type_ = CONFIG;
            }
          else if (ACE_OS::strcasecmp ("RECONFIG", get_opt.optarg) == 0)
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

#endif /* __GNUC__ */

        case '?':
        default:

// The templatized method parameters needed by the reconfig scheduler
// class template are hopelessly broken on pre-2.8 versions of g++.
#if (! defined (__GNUC__)) || (__GNUC__ > 2) || \
(__GNUC__ == 2 && defined (__GNUC_MINOR__) && __GNUC_MINOR__ >= 8)

          ACE_DEBUG ((LM_DEBUG,
                      "Usage: %s "
                      "[-n service_name] "
                      "[-p pid_file_name] "
                      "[-o ior_file_name] "
                      "[-s <CONFIG | reconfig>]"
                      "\n",
                      argv[0]));

#else /* __GNUC__ <= 2.8 */

          ACE_DEBUG ((LM_DEBUG,
                      "Usage: %s "
                      "[-n service_name] "
                      "[-p pid_file_name] "
                      "[-o ior_file_name] "
                      "\n",
                      argv[0]));

#endif /* __GNUC__ */

          return -1;
        }
    }

  return 0;
}

int main (int argc, char *argv[])
{
  ACE_TRY_NEW_ENV
    {
      TAO_Scheduling_Service scheduling_service;

      ACE_DEBUG ((LM_DEBUG,
                  "%s; initializing scheduling service\n", __FILE__));
      if (scheduling_service.init (argc, argv) < 0)
        ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "init"), 1);

      ACE_DEBUG ((LM_DEBUG,
                  "%s; running scheduling service\n", __FILE__));
      if (scheduling_service.run (ACE_TRY_ENV) < 0)
        ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "run"), 1);

    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "schedule_service");
    }
  ACE_ENDTRY;

  return 0;
}


#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
#if (! defined (__GNUC__)) || (__GNUC__ > 2) || \
(__GNUC__ == 2 && defined (__GNUC_MINOR__) && __GNUC_MINOR__ >= 8)
template class TAO_Reconfig_Scheduler<TAO_MUF_Reconfig_Sched_Strategy, ACE_SYNCH_MUTEX>;
#  endif /* __GNUC__ */
#elif defined(ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#if (! defined (__GNUC__)) || (__GNUC__ > 2) || \
(__GNUC__ == 2 && defined (__GNUC_MINOR__) && __GNUC_MINOR__ >= 8)
#pragma instantiate TAO_Reconfig_Scheduler<TAO_MUF_Reconfig_Sched_Strategy, ACE_SYNCH_MUTEX>
#  endif /* __GNUC__ */
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
