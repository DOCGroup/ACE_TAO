// $Id$

#include "Scheduling_Service.h"

#include "ace/Get_Opt.h"
#include "ace/Auto_Ptr.h"
#include "ace/Argv_Type_Converter.h"
#include "orbsvcs/CosNamingC.h"

ACE_RCSID(Scheduling_Service, Scheduling_Service, "$Id$")

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

  ACE_TRY_NEW_ENV
    {
      // Copy command line parameter.
      ACE_Argv_Type_Converter command_line(argc, argv);

      // Initialize ORB manager.
      this->orb_manager_.init (command_line.get_argc(), command_line.get_ASCII_argv() ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      orb = this->orb_manager_.orb ();
      ACE_TRY_CHECK;

      poa_manager = this->orb_manager_.poa_manager ();
      ACE_TRY_CHECK;

      poa_manager->activate (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Check the non-ORB arguments.  this needs to come before we
      // initialize the scheduler implementation so that we know which
      // type of scheduler to use.

      result = this->parse_args (command_line.get_argc(), command_line.get_TCHAR_argv());
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
                               "TAO_Scheduling_Service::init: "
                               "unrecognized Scheduler_Type"), -1);
        }

      // Locate the naming service.
      CORBA::Object_var naming_obj =
        orb->resolve_initial_references ("NameService" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (naming_obj.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to locate the Naming Service.\n"),
                          -1);
      CosNaming::NamingContext_var naming_context =
        CosNaming::NamingContext::_narrow (naming_obj.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      RtecScheduler::Scheduler_var scheduler =
        this->scheduler_impl_->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::String_var scheduler_ior_string =
        orb->object_to_string (scheduler.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG, ACE_LIB_TEXT("The scheduler IOR is <%s>\n"),
                            ACE_TEXT_CHAR_TO_TCHAR(scheduler_ior_string.in ())));

      // Register the servant with the Naming Context....
      CosNaming::Name schedule_name (1);
      schedule_name.length (1);
      schedule_name[0].id = CORBA::string_dup (this->service_name_.rep());
      naming_context->rebind (schedule_name, scheduler.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (this->ior_file_name_.rep() != 0)
        {
          FILE *iorf = fopen (this->ior_file_name_.rep(), "w");
          if (iorf != 0)
            {
              ACE_OS::fprintf (iorf,
                               ACE_LIB_TEXT("%s\n"),
                               ACE_TEXT_CHAR_TO_TCHAR(scheduler_ior_string.in ()));
              ACE_OS::fclose (iorf);
            }
        }

      if (this->pid_file_name_.rep() != 0)
        {
          FILE *pidf = fopen (this->pid_file_name_.rep(), "w");
          if (pidf != 0)
            {
              ACE_OS::fprintf (pidf,
                               ACE_LIB_TEXT("%ld\n"),
                               ACE_static_cast (long, ACE_OS::getpid ()));
              ACE_OS::fclose (pidf);
            }
        }
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "TAO_Scheduling_Service::init");
      return -1;
    }
  ACE_ENDTRY;

  return 0;
}


// Runs the TAO_Scheduling_Service.

int
TAO_Scheduling_Service::run (ACE_ENV_SINGLE_ARG_DECL)
{
  // Run the ORB manager.
  return this->orb_manager_.run (ACE_ENV_SINGLE_ARG_PARAMETER);
}


// Parses the command line arguments.

int
TAO_Scheduling_Service::parse_args (int argc, ACE_TCHAR* argv[])
{
  ACE_Get_Opt get_opt (argc, argv, ACE_LIB_TEXT("n:p:o:s:"));
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

// The templatized method parameters needed by the reconfig scheduler
// class template are hopelessly broken on pre-2.8 versions of g++.
#if (! defined (__GNUC__)) || (__GNUC__ > 2) || \
(__GNUC__ == 2 && defined (__GNUC_MINOR__) && __GNUC_MINOR__ >= 8)

        case 's':
          if (ACE_OS::strcasecmp (ACE_LIB_TEXT("CONFIG"), get_opt.optarg) == 0)
            {
              this->scheduler_type_ = CONFIG;
            }
          else if (ACE_OS::strcasecmp (ACE_LIB_TEXT("RECONFIG"), get_opt.optarg) == 0)
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

int ACE_TMAIN (int argc, ACE_TCHAR* argv[])
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

      scheduling_service.run (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "schedule_service");
          return 1;
    }
  ACE_ENDTRY;

  return 0;
}


#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
#if (! defined (__GNUC__)) || (__GNUC__ > 2) || \
(__GNUC__ == 2 && defined (__GNUC_MINOR__) && __GNUC_MINOR__ >= 8)
template class auto_ptr<RtecScheduler::Config_Info>;
template class auto_ptr<RtecScheduler::RT_Info>;
template class auto_ptr<TAO_Reconfig_Scheduler_Entry>;
template class ACE_Auto_Basic_Ptr<RtecScheduler::Config_Info>;
template class ACE_Auto_Basic_Ptr<RtecScheduler::RT_Info>;
template class ACE_Auto_Basic_Ptr<TAO_Reconfig_Scheduler_Entry>;
template class ACE_Hash_Map_Manager_Ex<int, RtecScheduler::Config_Info *, ACE_Hash<int>, ACE_Equal_To<int>, TAO_SYNCH_MUTEX>;
template class ACE_Hash_Map_Manager_Ex<int, RtecScheduler::Dependency_Set *, ACE_Hash<int>, ACE_Equal_To<int>, TAO_SYNCH_MUTEX>;
template class ACE_Hash_Map_Manager_Ex<int, RtecScheduler::RT_Info *, ACE_Hash<int>, ACE_Equal_To<int>, TAO_SYNCH_MUTEX>;
template class ACE_Hash_Map_Iterator_Base_Ex<int, RtecScheduler::Config_Info *, ACE_Hash<int>, ACE_Equal_To<int>, TAO_SYNCH_MUTEX>;
template class ACE_Hash_Map_Iterator_Base_Ex<int, RtecScheduler::Dependency_Set *, ACE_Hash<int>, ACE_Equal_To<int>, TAO_SYNCH_MUTEX>;
template class ACE_Hash_Map_Iterator_Base_Ex<int, RtecScheduler::RT_Info *, ACE_Hash<int>, ACE_Equal_To<int>, TAO_SYNCH_MUTEX>;
template class ACE_Hash_Map_Iterator_Ex<int,RtecScheduler::Config_Info*,ACE_Hash<int>,ACE_Equal_To<int>,TAO_SYNCH_MUTEX>;
template class ACE_Hash_Map_Iterator_Ex<int,RtecScheduler::Dependency_Set*,ACE_Hash<int>,ACE_Equal_To<int>,TAO_SYNCH_MUTEX>;
template class ACE_Hash_Map_Iterator_Ex<int,RtecScheduler::RT_Info*,ACE_Hash<int>,ACE_Equal_To<int>,TAO_SYNCH_MUTEX>;
template class ACE_Hash_Map_Reverse_Iterator_Ex<int,RtecScheduler::Config_Info*,ACE_Hash<int>,ACE_Equal_To<int>,TAO_SYNCH_MUTEX>;
template class ACE_Hash_Map_Reverse_Iterator_Ex<int,RtecScheduler::Dependency_Set*,ACE_Hash<int>,ACE_Equal_To<int>,TAO_SYNCH_MUTEX>;
template class ACE_Hash_Map_Reverse_Iterator_Ex<int,RtecScheduler::RT_Info*,ACE_Hash<int>,ACE_Equal_To<int>,TAO_SYNCH_MUTEX>;
template class ACE_Hash_Map_Entry<int, RtecScheduler::Config_Info *>;
template class ACE_Hash_Map_Entry<int, RtecScheduler::Dependency_Set *>;
template class ACE_Hash_Map_Entry<int, RtecScheduler::RT_Info *>;
template class ACE_RB_Tree<const char *, RtecScheduler::RT_Info *, ACE_Less_Than<const char *>, TAO_SYNCH_MUTEX>;
template class ACE_RB_Tree_Node<const char *, RtecScheduler::RT_Info *>;
template class ACE_RB_Tree_Iterator<const char *, RtecScheduler::RT_Info *, ACE_Less_Than<const char *>, TAO_SYNCH_MUTEX>;
template class ACE_RB_Tree_Iterator_Base<char const *, RtecScheduler::RT_Info *, ACE_Less_Than<char const *>, TAO_SYNCH_MUTEX>;
template class ACE_RB_Tree_Reverse_Iterator<const char *, RtecScheduler::RT_Info *, ACE_Less_Than<const char *>, TAO_SYNCH_MUTEX>;
template class TAO_Reconfig_Scheduler<TAO_MUF_Reconfig_Sched_Strategy, TAO_SYNCH_MUTEX>;
template class TAO_RSE_Dependency_Visitor<TAO_MUF_Reconfig_Sched_Strategy, TAO_SYNCH_MUTEX>;
template class TAO_RSE_DFS_Visitor<TAO_MUF_Reconfig_Sched_Strategy, TAO_SYNCH_MUTEX>;
template class TAO_RSE_Priority_Visitor<TAO_MUF_Reconfig_Sched_Strategy>;
template class TAO_RSE_Propagation_Visitor<TAO_MUF_Reconfig_Sched_Strategy, TAO_SYNCH_MUTEX>;
template class TAO_RSE_SCC_Visitor<TAO_MUF_Reconfig_Sched_Strategy, TAO_SYNCH_MUTEX>;
template class TAO_RSE_Utilization_Visitor<TAO_MUF_Reconfig_Sched_Strategy>;
#  endif /* __GNUC__ */
#elif defined(ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#if (! defined (__GNUC__)) || (__GNUC__ > 2) || \
(__GNUC__ == 2 && defined (__GNUC_MINOR__) && __GNUC_MINOR__ >= 8)
#pragma instantiate auto_ptr<RtecScheduler::Config_Info>
#pragma instantiate auto_ptr<RtecScheduler::RT_Info>
#pragma instantiate auto_ptr<TAO_Reconfig_Scheduler_Entry>
#pragma instantiate ACE_Auto_Basic_Ptr<RtecScheduler::Config_Info>
#pragma instantiate ACE_Auto_Basic_Ptr<RtecScheduler::RT_Info>
#pragma instantiate ACE_Auto_Basic_Ptr<TAO_Reconfig_Scheduler_Entry>
#pragma instantiate ACE_Hash_Map_Manager_Ex<int, RtecScheduler::Config_Info *, ACE_Hash<int>, ACE_Equal_To<int>, TAO_SYNCH_MUTEX>
#pragma instantiate ACE_Hash_Map_Manager_Ex<int, RtecScheduler::Dependency_Set *, ACE_Hash<int>, ACE_Equal_To<int>, TAO_SYNCH_MUTEX>
#pragma instantiate ACE_Hash_Map_Manager_Ex<int, RtecScheduler::RT_Info *, ACE_Hash<int>, ACE_Equal_To<int>, TAO_SYNCH_MUTEX>
#pragma instantiate ACE_Hash_Map_Iterator_Base_Ex<int, RtecScheduler::Config_Info *, ACE_Hash<int>, ACE_Equal_To<int>, TAO_SYNCH_MUTEX>
#pragma instantiate ACE_Hash_Map_Iterator_Base_Ex<int, RtecScheduler::Dependency_Set *, ACE_Hash<int>, ACE_Equal_To<int>, TAO_SYNCH_MUTEX>
#pragma instantiate ACE_Hash_Map_Iterator_Base_Ex<int, RtecScheduler::RT_Info *, ACE_Hash<int>, ACE_Equal_To<int>, TAO_SYNCH_MUTEX>
#pragma instantiate ACE_Hash_Map_Iterator_Ex<int,RtecScheduler::Config_Info*,ACE_Hash<int>,ACE_Equal_To<int>,TAO_SYNCH_MUTEX>
#pragma instantiate ACE_Hash_Map_Iterator_Ex<int,RtecScheduler::Dependency_Set*,ACE_Hash<int>,ACE_Equal_To<int>,TAO_SYNCH_MUTEX>
#pragma instantiate ACE_Hash_Map_Iterator_Ex<int,RtecScheduler::RT_Info*,ACE_Hash<int>,ACE_Equal_To<int>,TAO_SYNCH_MUTEX>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator_Ex<int,RtecScheduler::Config_Info*,ACE_Hash<int>,ACE_Equal_To<int>,TAO_SYNCH_MUTEX>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator_Ex<int,RtecScheduler::Dependency_Set*,ACE_Hash<int>,ACE_Equal_To<int>,TAO_SYNCH_MUTEX>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator_Ex<int,RtecScheduler::RT_Info*,ACE_Hash<int>,ACE_Equal_To<int>,TAO_SYNCH_MUTEX>
#pragma instantiate ACE_Hash_Map_Entry<int, RtecScheduler::Config_Info *>
#pragma instantiate ACE_Hash_Map_Entry<int, RtecScheduler::Dependency_Set *>
#pragma instantiate ACE_Hash_Map_Entry<int, RtecScheduler::RT_Info *>
#pragma instantiate ACE_RB_Tree<const char *, RtecScheduler::RT_Info *, ACE_Less_Than<const char *>, TAO_SYNCH_MUTEX>
#pragma instantiate ACE_RB_Tree_Node<const char *, RtecScheduler::RT_Info *>
#pragma instantiate ACE_RB_Tree_Iterator<const char *, RtecScheduler::RT_Info *, ACE_Less_Than<const char *>, TAO_SYNCH_MUTEX>
#pragma instantiate ACE_RB_Tree_Iterator_Base<char const *, RtecScheduler::RT_Info *, ACE_Less_Than<char const *>, TAO_SYNCH_MUTEX>
#pragma instantiate ACE_RB_Tree_Reverse_Iterator<const char *, RtecScheduler::RT_Info *, ACE_Less_Than<const char *>, TAO_SYNCH_MUTEX>
#pragma instantiate TAO_Reconfig_Scheduler<TAO_MUF_Reconfig_Sched_Strategy, TAO_SYNCH_MUTEX>
#pragma instantiate TAO_RSE_Dependency_Visitor<TAO_MUF_Reconfig_Sched_Strategy, TAO_SYNCH_MUTEX>
#pragma instantiate TAO_RSE_DFS_Visitor<TAO_MUF_Reconfig_Sched_Strategy, TAO_SYNCH_MUTEX>
#pragma instantiate TAO_RSE_Priority_Visitor<TAO_MUF_Reconfig_Sched_Strategy>
#pragma instantiate TAO_RSE_Propagation_Visitor<TAO_MUF_Reconfig_Sched_Strategy, TAO_SYNCH_MUTEX>
#pragma instantiate TAO_RSE_SCC_Visitor<TAO_MUF_Reconfig_Sched_Strategy, TAO_SYNCH_MUTEX>
#pragma instantiate TAO_RSE_Utilization_Visitor<TAO_MUF_Reconfig_Sched_Strategy>
#  endif /* __GNUC__ */
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
