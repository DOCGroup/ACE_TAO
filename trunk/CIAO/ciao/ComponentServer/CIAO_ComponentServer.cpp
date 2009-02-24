/**
 * @file CIAO_ComponentServer.cpp
 * @author William R. Otte
 * // $Id$
 * Implementation and main for CIAO_ComponentServer.
 */

#include "CIAO_ComponentServer.h"

#include "ace/OS_NS_string.h"
#include "ace/Log_Msg.h"
#include "ace/Get_Opt.h"
#include "ace/Sched_Params.h"
#include "ace/Trace.h"
#include "ace/Env_Value_T.h"
#include "tao/ORB.h"
#include "tao/Object.h"
#include "tao/CORBA_methods.h"
#include "tao/PortableServer/PortableServer.h"
#include "tao/ORB_Core.h"
#include "ciao/CIAO_common.h"
#include "ciao/Logger/Logger_Service.h"
#include "ciao/Logger/Log_Macros.h"
#include "ciao/Server_init.h"


#include "CIAO_ComponentServer_Impl.h"
#include "CIAO_CS_ClientC.h"
#include "Configurator_Factory.h"
#include "Configurators/Server_Configurator.h"

#ifdef CIAO_BUILD_COMPONENTSERVER_EXE

int ACE_TMAIN (int argc, ACE_TCHAR **argv)
{
  // Tracing disabled by default
  CIAO_DISABLE_TRACE ();

  CIAO_TRACE ("CIAO_ComponentServer::ACE_TMAIN");

  try
    {
      CIAO::Deployment::ComponentServer_Task cs (argc, argv);
      cs.run ();
      return 0;
    }
  catch (CIAO::Deployment::ComponentServer_Task::Error &e)
    {
      CIAO_DEBUG ((LM_ALERT, CLINFO "CIAO_ComponentServer main: Caught ComponentServer exception: %C\n",
                  e.err_.c_str ()));
    }
  catch (...)
    {
      CIAO_DEBUG ((LM_ALERT, CLINFO "CIAO_ComponentServer main: Caught unknown exception.\n"));
    }

  return -1;
}

#endif /* CIAO_BUILD_COMPONENTSERVER_EXE */

bool
write_IOR (const ACE_TCHAR * ior_file_name, const char* ior)
{
  FILE* ior_output_file_ =
    ACE_OS::fopen (ior_file_name, "w");

  if (ior_output_file_)
    {
      ACE_OS::fprintf (ior_output_file_,
                       "%s",
                       ior);
      ACE_OS::fclose (ior_output_file_);
      return true;
    }
  return false;
}

namespace CIAO
{
  namespace Deployment
  {
    ComponentServer_Task::ComponentServer_Task (int argc, ACE_TCHAR **argv)
      : orb_ (0),
        uuid_ (""),
        callback_ior_str_ ("")
    {
      CIAO_TRACE ("CIAO_ComponentServer_Task::CIAO_ComponentServer_Task ()");

      Logger_Service
        *clf = ACE_Dynamic_Service<Logger_Service>::instance ("CIAO_Logger_Backend_Factory");

      if (!clf)
        clf = new Logger_Service;

      this->logger_.reset (clf);

      this->logger_->init (argc, argv);

      CIAO_DEBUG ((LM_TRACE, CLINFO "CIAO_ComponentServer_Task::CIAO_ComponentServer_Task - "
                   "Creating server object\n"));
      Configurator_Factory cf;
      this->configurator_.reset (cf (argc, argv));

      if (!this->configurator_->create_config_managers ())
        {
          CIAO_ERROR ((LM_ERROR, CLINFO
                       "ComponentServer_Task::ComponentServer_Task - "
                       "Error configuring ComponentServer configurator, exiting.\n"));
          throw Error ("Unable to load ComponentServer configurator.");
        }


      this->configurator_->pre_orb_initialize ();

      CIAO_DEBUG ((LM_TRACE, CLINFO "CIAO_ComponentServer_Task::CIAO_ComponentServer_Task - "
                   "Creating ORB\n"));
      this->orb_ = CORBA::ORB_init (argc, argv);

      this->configurator_->post_orb_initialize (this->orb_.in ());

      this->parse_args (argc, argv);
      this->configure_logging_backend ();

      CIAO::Server_init (this->orb_.in ());

      CIAO_DEBUG ((LM_TRACE, CLINFO "CIAO_ComponentServer_Task::CIAO_ComponentServer_Task - "
                   "CIAO_ComponentServer object created.\n"));
    }

    int
    ComponentServer_Task::svc (void)
    {
      CIAO_TRACE ("ComponentServer_Task::svc");

      CIAO_DEBUG ((LM_TRACE, CLINFO "ComponentServer_Task::svc - "
                   "Activating the root POA\n"));
      CORBA::Object_var object =
        this->orb_->resolve_initial_references ("RootPOA");

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (object.in ());

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager ();

      poa_manager->activate ();

      CIAO_DEBUG ((LM_TRACE, CLINFO "ComponentServer_Task::svc - "
                   "Creating server implementation object\n"));
      CIAO::Deployment::CIAO_ComponentServer_i *ci_srv = 0;
      ACE_NEW_NORETURN (ci_srv, CIAO_ComponentServer_i (this->uuid_, this->orb_.in (), root_poa.in ()));

      if (ci_srv == 0)
        {
          CIAO_ERROR ((LM_CRITICAL, "ComponentServer_Task::run - "
                       "Out of memory error while allocating servant."));
          throw Error ("Out of memory whilst allocating servant.");
        }

      PortableServer::ServantBase_var safe (ci_srv);

      ComponentServer_var cs (ci_srv->_this ());

      if (this->output_file_ != ACE_TEXT(""))
        {
          CORBA::String_var ior = this->orb_->object_to_string (cs.in ());
          write_IOR (this->output_file_.c_str (), ior.in ());
        }


      if (this->callback_ior_str_ != "")
        {
          CIAO_DEBUG ((LM_TRACE, CLINFO " resolving callback IOR\n"));
          CORBA::Object_ptr obj = this->orb_->string_to_object (this->callback_ior_str_.c_str ());
          ServerActivator_var sa (ServerActivator::_narrow (obj));

          if (CORBA::is_nil (sa.in ()))
            {
              CIAO_DEBUG ((LM_ERROR, CLINFO "ComponentServer_Task::svc - "
                           "Failed to narrow callback IOR\n"));
              throw Error ("Faled to narrow callback IOR");
            }

          Components::ConfigValues_var config;
          {
            Components::ConfigValues *cf = 0;
            ACE_NEW_NORETURN (cf, Components::ConfigValues (0));

            if  (cf == 0)
              {
                CIAO_ERROR ((LM_CRITICAL, "ComponentServer_Task::run - "
                             "Out of memory error while allocating config values."));
              }
            else
              {
                config = cf;
              }
          }

          // Make callback.
          CIAO_DEBUG ((LM_TRACE, CLINFO "ComponentServer_Task::svc - "
                       "Making callback on my ServerActivator\n"));

          try
            {
              // Callback to NodeApplication to get configuration
              sa->component_server_callback (cs.in (),
                                             this->uuid_.c_str (),
                                             config.out ());

              CIAO_DEBUG ((LM_TRACE, CLINFO "ComponentServer_Task::svc - "
                           "Configuration received\n"));
              // @@WO: Probably need to do something with these config values.

              ci_srv->init (sa.in (), config._retn ());

              CIAO_DEBUG ((LM_NOTICE, CLINFO "ComponentServer_Task::svc - "
                           "Configuration complete for component server %C\n",
                           this->uuid_.c_str ()));

              sa->configuration_complete (this->uuid_.c_str ());
            }
          catch (const CORBA::BAD_PARAM &)
            {
              CIAO_ERROR ((LM_ERROR, CLINFO "ComponentServer_Task::svc - "
                           "The Callback IOR provided pointed to the wrong ServerActivator\n"));
              throw Error ("Bad callback IOR");
            }
          catch (...)
            {
              CIAO_ERROR ((LM_ERROR, CLINFO "ComponentServer_Task::svc - "
                           "Caught exception while calling back\n"));
              throw Error ("Caught exception while calling back");
            }

        }
      else
        {
          CIAO_DEBUG ((LM_TRACE, CLINFO "ComponentServer_Task::svc - "
                       "Initializing ComponentServer without ServantActivator callback\n"));
          ci_srv->init (0, 0);
        }

      this->orb_->run ();
      CIAO_DEBUG ((LM_TRACE, CLINFO "ComponentServer_Task::svc - "
                   "ORB Event loop completed.\n"));

      root_poa->destroy (1, 1);

      this->orb_->destroy ();

      return 0;
    }

    void
    ComponentServer_Task::run (void)
    {
      CIAO_TRACE ("ComponentServer_Task::run");

      if (this->configurator_->rt_support ())
        {
          CIAO_DEBUG ((LM_DEBUG, CLINFO "ComponentServer_Task::run - Starting ORB with RT support\n"));

          this->check_supported_priorities ();

          // spawn a thread
          // Task activation flags.
          long flags =
            THR_NEW_LWP |
            THR_JOINABLE |
            this->orb_->orb_core ()->orb_params ()->thread_creation_flags ();

          // Activate task.
          int result = this->activate (flags);
          if (result == -1)
            {
              if (errno == EPERM)
                {
                  CIAO_ERROR ((LM_EMERGENCY, CLINFO
                              "ComponentServer_Task::run - Cannot create thread with scheduling policy %C\n"
                              "because the user does not have the appropriate privileges, terminating program. "
                              "Check svc.conf options and/or run as root\n",
                              sched_policy_name (this->orb_->orb_core ()->orb_params ()->ace_sched_policy ())));
                  throw Error ("Unable to start RT support due to permissions problem.");
                }
              else
                throw Error ("Unknown error while spawning ORB thread.");
            }

          // Wait for task to exit.
          result = this->wait ();

          if (result != -1)
            throw Error ("Unknown error waiting for ORB thread to complete");

          CIAO_DEBUG ((LM_INFO, CLINFO "ComponentServer_Task::run - ORB thread completed, terminating ComponentServer %C\n",
                      this->uuid_.c_str ()));
        }
      else
        {
          CIAO_DEBUG ((LM_DEBUG, CLINFO "ComponentServer_Task::run - Starting ORB without RT support\n"));
          this->svc ();
          CIAO_DEBUG ((LM_INFO, CLINFO "ComponentServer_Task::run - ORB has shutdown, terminating ComponentServer\n"));
        }
    }

    void
    ComponentServer_Task::parse_args (int argc, ACE_TCHAR **argv)
    {
      CIAO_TRACE ("ComponentServer_Task::parse_args");

      CIAO_DEBUG ((LM_TRACE, CLINFO "ComponentServer_Task::parse_args - parsing arguments...\n"));

      ACE_Get_Opt opts (argc, argv, ACE_TEXT("hu:c:"), 1, 0,
                        ACE_Get_Opt::RETURN_IN_ORDER);
      opts.long_option (ACE_TEXT("uuid"), 'u', ACE_Get_Opt::ARG_REQUIRED);
      opts.long_option (ACE_TEXT("callback-ior"), 'c', ACE_Get_Opt::ARG_REQUIRED);
      opts.long_option (ACE_TEXT("help"), 'h');
      opts.long_option (ACE_TEXT("log-level"),'l', ACE_Get_Opt::ARG_REQUIRED);
      opts.long_option (ACE_TEXT("trace"),'t', ACE_Get_Opt::NO_ARG);
      opts.long_option (ACE_TEXT("output-ior"),'o', ACE_Get_Opt::ARG_REQUIRED);

      //int j;
      char c;
      ACE_CString s;

      while ((c = opts ()) != -1)
        {
          CIAO_DEBUG ((LM_TRACE, CLINFO "ComponentServer_Task::parse_args - "
                       "Found option: \"%s\" with argument \"%s\"\n",
                       opts.last_option (), opts.opt_arg ()));

          switch (c)
            {
            case 'u':
              CIAO_DEBUG ((LM_DEBUG, CLINFO "ComponentServer_Task::parse_args - "
                           "uuid is %s\n",
                           opts.opt_arg ()));
              this->uuid_ = opts.opt_arg ();
              break;

            case 'c':
              CIAO_DEBUG ((LM_DEBUG, CLINFO "ComponentServer_Task::parse_args - "
                           "callback ior is %s\n",
                          opts.opt_arg ()));
              this->callback_ior_str_ = opts.opt_arg ();
              break;

            case 'l':
              {
                continue; // no-op, already taken care of
              }

            case 't':
              continue; // already taken care of

            case 'o':
              CIAO_DEBUG ((LM_DEBUG, CLINFO "ComponentServer_Task::parse_args - "
                           "IOR Output file: %s\n",
                           opts.opt_arg ()));
              this->output_file_ = opts.opt_arg ();
              break;

            case 'h':
              this->usage ();
              throw Error ("Command line help requested, bailing out....");

            default:
              CIAO_ERROR ((LM_ERROR, CLINFO " Unknown option: %s\n",
                          opts.last_option ()));
              this->usage ();
              ACE_TString err (ACE_TEXT("Unknown option "));
              err += opts.last_option ();
              throw Error (err);
            }
        }

      // check required options.
      if (this->uuid_ == "")
        throw Error ("Option required: -u|--uuid");
      if (this->callback_ior_str_ == "")
        CIAO_ERROR ((LM_WARNING, CLINFO
                     "ComponentServer_Task::parse_args - Starting ComponentServer without a callback IOR\n"));
    }

    void
    ComponentServer_Task::usage (void)
    {
      CIAO_TRACE ("ComponentServer_Task::usage");
      // Shouldn't be subject to CIAO's logging policy
      ACE_ERROR ((LM_EMERGENCY, "Usage: CIAO_ComponentServer <options>\n"
                   "Options:\n"
                   "\t-h|--help\t\t\t\tShow help\n"
                   "\t-l|--log-level <level>\t\t\tSets log level (default 5). 1 - most detailed.\n"
                   "\t-u|--uuid <uuid> \t\t\tSets UUID of spawned component server (required)\n"
                   "\t-c|--callback-ior <string ior>\t\tSets callback url for the spawning ServerActivator.\n"
                   "\t-o|--output-ior <filename>\t\tOutputs the IOR of the component server object to file\n"
                   ));

    }

    const char *
    ComponentServer_Task::sched_policy_name (int sched_policy)
    {
      const char *name = 0;

      switch (sched_policy)
        {
        case ACE_SCHED_OTHER:
          name = "SCHED_OTHER";
          break;
        case ACE_SCHED_RR:
          name = "SCHED_RR";
          break;
        case ACE_SCHED_FIFO:
          name = "SCHED_FIFO";
          break;
        }

      return name;
    }

    /// The following check is taken from $(TAO_ROOT)/tests/RTCORBA/
    void
    ComponentServer_Task::check_supported_priorities (void)
    {
      CIAO_TRACE ("ComponentServer_Task::check_supported_priorities");

      int const sched_policy =
        this->orb_->orb_core ()->orb_params ()->ace_sched_policy ();

      // Check that we have sufficient priority range to run,
      // i.e., more than 1 priority level.
      int const max_priority = ACE_Sched_Params::priority_max (sched_policy);
      int const min_priority = ACE_Sched_Params::priority_min (sched_policy);

      if (max_priority == min_priority)
        {
          CIAO_DEBUG ((LM_DEBUG, CLINFO "ComponentServer_Task::check_supported_priorities - "
                      " Not enough priority levels with the %C scheduling policy\n"
                      "on this platform to run, terminating ....\n"
                      "Check svc.conf options\n",
                      sched_policy_name (sched_policy)));

          throw Error ("Bad scheduling policy.");
        }
    }

    void
    ComponentServer_Task::configure_logging_backend (void)
    {
      Logger_Service
        *clf = ACE_Dynamic_Service<Logger_Service>::instance ("CIAO_Logger_Backend_Factory");
      if (clf)
        {
          CIAO_DEBUG ((LM_TRACE, CLINFO "ComponentServer_Task::configure_logging_backend - "
                       "Replacing logger backend\n"));
          ACE_Log_Msg_Backend * backend = clf->get_logger_backend(this->orb_);
          backend->open(0);
          ACE_Log_Msg::msg_backend (backend);
          ACE_Log_Msg * ace = ACE_Log_Msg::instance();
          ace->clr_flags(ace->flags());
          ace->set_flags(ACE_Log_Msg::CUSTOM);
        }
    }
  }
}


