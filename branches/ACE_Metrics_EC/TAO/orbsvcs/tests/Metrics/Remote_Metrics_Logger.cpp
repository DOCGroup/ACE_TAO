// $Id$
#include "ace/OS.h"
#include "ace/Arg_Shifter.h"
#include "ace/ARGV.h"

#include "tao/corba.h"
#include "tao/TAO_Internal.h"

#include "Remote_Metrics_Logger.h"

int enable_debug = 0;
// Used to enable LM_DEBUG messages

int enable_trace = 0;
  // Used to enable LM_TRACE messages

int logger_host_name_set = 0;
// Trap missing logger hostname

int logger_host_ip_set = 0;
// Trap missing logger ip

ACE_RCSID(Remote_Metrics_Logger, Remote_Metrics_Logger, "$Id$")

void CatchError(void)
{
     cout <<  "This is a real problem. " << endl;
}

////////////////////////////////////////////
// class TAO_Remote_Metrics_Logger_Driver //
////////////////////////////////////////////

// Default Constructor.
TAO_Remote_Metrics_Logger_Driver::TAO_Remote_Metrics_Logger_Driver (void)
  : logger_impl_ (0),
    logger_ior_file_name_ (0),
    log_file_name_ (0),
    logger_host_name_(0),
    logger_host_ip_(0),
    log_output_ (1),
    generate_tabbed_output_(1),
    visualize_ (0)
{
   log_file_name_ = CORBA::string_dup("remote_logger.txt");
   logger_ior_file_name_ = CORBA::string_dup("remote_logger.ior");
}

// Destructor.
TAO_Remote_Metrics_Logger_Driver::~TAO_Remote_Metrics_Logger_Driver (void)
{
   delete logger_impl_;
   delete log_file_name_;
   delete logger_host_name_;
   delete logger_ior_file_name_;
}

// Initialize the logger driver with the command line arguments.
int
TAO_Remote_Metrics_Logger_Driver::init ()
{
  ACE_TRY_NEW_ENV
    {
      TAO_Internal::default_svc_conf_entries
         ("static Resource_Factory \"-ORBresources tss -ORBReactorType select_st -ORBPriorityMapping multi -ORBPriorityNativeBase 50 -ORBPriorityCORBABase 50 -ORBPrioritiesContiguous %d -ORBSchedPolicy SCHED_FIFO -ORBInputCDRAllocator null \"",
         TAO_DEFAULT_SERVER_STRATEGY_FACTORY_ARGS,
         TAO_DEFAULT_CLIENT_STRATEGY_FACTORY_ARGS);

      char *orb_init_string = 0;
      this->generate_orb_args (orb_init_string);

      ACE_ARGV * the_orb_args = new ACE_ARGV (orb_init_string);

      int orb_argc = the_orb_args->argc ();
      char ** orb_argv = the_orb_args->argv ();

      // Initialize the ORB
      this->orb_ = CORBA::ORB_init (orb_argc, orb_argv, 0, ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // Delete the orb_init_string since we're finished using it.
      delete orb_init_string;

      // Connect to the RootPOA.
      CORBA::Object_var poa_object =
         this->orb_->resolve_initial_references(TAO_OBJID_ROOTPOA,
                                                ACE_TRY_ENV);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (poa_object.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                          " (%P|%t) Unable to initialize the Root POA.\n"),
                          1);

      this->root_poa_ = PortableServer::POA::_narrow (poa_object.in (), ACE_TRY_ENV);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (poa_manager_.in ()))
      {
         this->poa_manager_ =
            this->root_poa_->the_POAManager (ACE_TRY_ENV);
         ACE_TRY_CHECK;

         // Activate the POA manager.
         this->poa_manager_->activate (ACE_TRY_ENV);
         ACE_TRY_CHECK;
      }

      // Create a child poa with USER_ID and PERSISTENT policies.
      CORBA::PolicyList policies;
      policies.length (2);

      policies[0] = this->root_poa_->create_id_assignment_policy (PortableServer::USER_ID,
                                                                  ACE_TRY_ENV);
      ACE_TRY_CHECK;

      policies[1] = this->root_poa_->create_lifespan_policy(PortableServer::PERSISTENT,
                                                                ACE_TRY_ENV);
      ACE_TRY_CHECK;


      this->persistent_poa_ = this->root_poa_->create_POA("TheChildPOA",
                                                           this->poa_manager_.in (),
                                                           policies,
                                                           ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // Cleanup the POA Policies
      policies[0]->destroy (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      policies[1]->destroy (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG,
                  "Constructing Logger with visualize_=%d, log_output_=%d\n",
                  visualize_, log_output_));

      // Now construct the logger implementation, using the settings.
      ACE_NEW_THROW_EX (this->logger_impl_,
                        TAO_Metrics_Logger (this->visualize_,
                                            this->log_output_,
                                            this->generate_tabbed_output_,
                                            this->log_file_name_),
                        CORBA::NO_MEMORY ());
      ACE_TRY_CHECK;

      // Register and activate the logger servant object in the
      // persistent child POA, under a specific name.
      PortableServer::ObjectId_var rml_id =
        PortableServer::string_to_ObjectId ("remote_metrics_logger");

      this->persistent_poa_->
         activate_object_with_id (rml_id.in (),
                                  this->logger_impl_,
                                  ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // Get and store a reference to the logger servant.
      CORBA::Object_var logger_ref =
         this->persistent_poa_->id_to_reference(rml_id.in (), ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // Output and log the logger IOR.
      ACE_DEBUG ((LM_DEBUG,
                  "\n\nThe TAO_Metrics logger IOR is:\n\n%s\n\n",
                  this->orb_->object_to_string (logger_ref)));

      if (this->logger_ior_file_name_ != 0)
      {
         FILE *liorf = fopen (this->logger_ior_file_name_, "w");
         if (liorf != 0)
         {
            ACE_OS::fprintf (liorf,
                             "%s\n",
                             this->orb_->object_to_string (logger_ref));
            ACE_OS::fclose (liorf);
            ACE_DEBUG ((LM_DEBUG,
                        "\n\nThe TAO_Metrics logger IOR was stored in: %s\n\n",
                        this->logger_ior_file_name_));
         }
      }
    }

    ACE_CATCHANY
    {
       ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "Unhandled Exception Generated by TAO_Remote_Metrics_Logger_Driver::init");
       ACE_ERROR_RETURN((LM_ERROR, "(%t) File: %N, Line: %l, Fatal Exception Enountered During TAO_Remote_Metrics_Logger_Driver::init\n"), -1);
    }
    ACE_ENDTRY;

    return 0;
}

// Runs the TAO_Remote_Metrics_Logger_Driver.
int
TAO_Remote_Metrics_Logger_Driver::run (CORBA::Environment& ACE_TRY_ENV)
{
  int result = 0;
  ACE_Thread_Manager orb_thread_manager;

  ACE_TRY
  {
     /////////////////////////////
     // Initialization sequence //
     /////////////////////////////

     // Spawn a thread that runs the orb event loop
     orb_thread_manager.spawn (TAO_Remote_Metrics_Logger_Driver::run_orb, this);

     ////////////////////////
     // Operation sequence //
     ////////////////////////

     // Run the logger's service task: pulls messages of the log
     // queue, processes them by logging to a file and/or generating
     // visualization events.

     this->logger_impl_->svc ();

     //////////////////////////
     // Termination sequence //
     //////////////////////////

     ACE_DEBUG ((LM_DEBUG, "about to shut down orb\n"));

     // Shut down orb, then wait for the ORB thread to exit.
     this->orb_->shutdown ();
     ACE_TRY_CHECK;
  }

  ACE_CATCHANY
  {
     ACE_DEBUG ((LM_DEBUG, "caught an exception\n"));
     ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "TAO_Remote_Metrics_Logger_Driver::run");
     result = -1;
  }

  ACE_ENDTRY;
  ACE_DEBUG ((LM_DEBUG, "about to wait on orb thread\n"));

  orb_thread_manager.wait ();
  ACE_DEBUG ((LM_DEBUG, "all done with run\n"));
  return result;

}

// Parses the command line arguments.
int
TAO_Remote_Metrics_Logger_Driver::parse_args (int argc, char *argv[])
{
   ACE_Arg_Shifter arg_shifter (argc, argv);

   // Ignore the first argument.  Program Name
   arg_shifter.ignore_arg ();

   while (arg_shifter.is_anything_left ())
   {
      const ACE_TCHAR *current_arg;

      if ( (current_arg = arg_shifter.get_the_parameter("-log_name")) )
      {
         // Activate logging, using passed log file name.
         log_output_ = 1;
         delete log_file_name_;
         this->log_file_name_ = CORBA::string_dup(current_arg);
         arg_shifter.consume_arg();
      }
      else if ( (current_arg = arg_shifter.get_the_parameter("-logger_ior")) )
      {
         delete logger_ior_file_name_;
         this->logger_ior_file_name_ = CORBA::string_dup(current_arg);;
         arg_shifter.consume_arg();
      }
      else if ((current_arg = arg_shifter.get_the_parameter("-logger_host")))
      {
         logger_host_name_set = 1;

         delete logger_host_name_;
         this->logger_host_name_ = CORBA::string_dup(current_arg);;
         arg_shifter.consume_arg();
      }
      else if (( current_arg = arg_shifter.get_the_parameter("-logger_ip")) )
      {
         logger_host_ip_set = 1;

         delete logger_host_ip_;
         this->logger_host_ip_ = CORBA::string_dup(current_arg);;
         arg_shifter.consume_arg();
      }
      else if (  arg_shifter.cur_arg_strncasecmp("-no_log_export") >= 0 )
      {
         // Activate logging, using passed log file name.
         this->generate_tabbed_output_ = 0;
         arg_shifter.consume_arg();
      }
      else if (  arg_shifter.cur_arg_strncasecmp("-debug") >= 0 )
      {
         enable_debug = 1;
         arg_shifter.consume_arg();
      }
      else if (  arg_shifter.cur_arg_strncasecmp("-trace") >= 0 )
      {
         enable_trace = 1;
         arg_shifter.consume_arg();
      }
      else if (  arg_shifter.cur_arg_strncasecmp("-log") >= 0 )
      {
         log_output_ = 1;
         arg_shifter.consume_arg();
      }
      else if (  arg_shifter.cur_arg_strncasecmp("-visualize") >= 0 )
      {
         visualize_ = 1;
         arg_shifter.consume_arg();
      }
      else if (  arg_shifter.cur_arg_strncasecmp("?") >= 0 )
      {
         ACE_DEBUG ((LM_DEBUG,
                     "Usage: %s "
                     "[-log_name log_file_name] "
                     "[-ior_name logger_ior_file_name] "
                     "[-log] (log to stdout)"
                     "[-debug] (Enable LM_DEBUG log priority)"
                     "[-trace] (Enable LM_TRACE log priority)"
                     "[-visualize] (Turn on Visualization)"
                     "\n",
                     argv[0]));
         return 0;
      }
      else
      {
         ACE_DEBUG((LM_DEBUG, "Unrecognized Command Line Arg: %s.  Assuming user-defined parameter\n", arg_shifter.get_current()));
         arg_shifter.ignore_arg ();
      }
   }

   if (!logger_host_name_set  && !logger_host_ip_set)
      return -1;
   else
      return 1;
}

// Run the ORB event loop.
void *
TAO_Remote_Metrics_Logger_Driver::run_orb (void *that)
{
   ACE_TRY_NEW_ENV
   {
      ACE_static_cast (TAO_Remote_Metrics_Logger_Driver *, that)->
         orb_->run (ACE_TRY_ENV);
      ACE_TRY_CHECK;
   }
   ACE_CATCHANY
   {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "TAO_Remote_Metrics_Logger_Driver::run_orb");
   }
   ACE_ENDTRY;

   return 0;
}

void
TAO_Remote_Metrics_Logger_Driver::generate_orb_args (char *& orb_init_str)
{
   u_int init_str_len = 0;

   if (logger_host_name_set)
   {
      init_str_len = 21 + // " -ORBendpoint iiop://"
                     ACE_OS::strlen(this->logger_host_name_) +
                     6 +  // ":10030"
                     1 /* Null Terminator */;
   }
   else
   {
      init_str_len = 21 + // " -ORBendpoint iiop://"
                     ACE_OS::strlen(this->logger_host_ip_) +
                     6 +  // ":10030"
                     1 /* Null Terminator */;
   }


   orb_init_str = CORBA::string_alloc (init_str_len);
   *orb_init_str =0;

   if (logger_host_name_set)
   {
      ACE_OS::sprintf(orb_init_str,
                      " -ORBendpoint iiop://%s:10030",
                      this->logger_host_name_);
   }
   else
   {
      ACE_OS::sprintf(orb_init_str,
                      " -ORBendpoint iiop://%s:10030",
                      this->logger_host_ip_);
   }

   ACE_DEBUG((LM_DEBUG, "ORB _init string is %s\n",orb_init_str ));
}

int
main (int argc, char *argv[])
{

   ACE_TRY_NEW_ENV
   {
      TAO_Remote_Metrics_Logger_Driver logger_driver;

      int result = logger_driver.parse_args (argc, argv);
      if (result < 0)
      {
         ACE_ERROR ((LM_ERROR,
                     "You must specify a logger hostname- Usage: %s "
                     "Remote_Logger -logger_host <log_host_name>\n",
                     argv[0]));
         return 0;
      }

      if (enable_debug)
      {
         ACE_Log_Msg::enable_debug_messages();
      }
      else
      {
         ACE_Log_Msg::disable_debug_messages();
      }

      if (enable_trace)
      {
         ACE_Log_Msg::enable_debug_messages(LM_TRACE);
      }
      else
      {
         ACE_Log_Msg::disable_debug_messages(LM_TRACE);
      }

      ACE_DEBUG ((LM_DEBUG,
                 "%s; initializing TAO_Metrics logger driver\n", __FILE__));

      if (logger_driver.init () < 0)
      {
         ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "TAO_Metrics logger driver init failed."), 1);
      }

      ACE_DEBUG ((LM_DEBUG,
                  "%s; running TAO_Metrics logger driver\n", __FILE__));

      logger_driver.run (ACE_TRY_ENV);
      ACE_TRY_CHECK;
   }
   ACE_CATCHANY
   {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "TAO_Metrics logger test driver exception in main.");
      return 1;
   }
   ACE_ENDTRY;

   return 0;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
#elif defined(ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

