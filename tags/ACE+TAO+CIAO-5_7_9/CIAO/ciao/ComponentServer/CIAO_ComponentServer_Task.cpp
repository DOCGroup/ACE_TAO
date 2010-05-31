/**
 * @file CIAO_ComponentServer_Task.cpp
 * @author William R. Otte
 * $Id$
 *
 * Implementation and main for CIAO_ComponentServer.
 */

#include "CIAO_ComponentServer_Task.h"

#include "ace/OS_NS_string.h"
#include "ace/Log_Msg.h"
#include "ace/Get_Opt.h"
#include "tao/ORB.h"
#include "tao/PortableServer/PortableServer.h"
#include "ciao/Logger/Logger_Service.h"
#include "ciao/Logger/Log_Macros.h"
#include "CIAO_ComponentServer_Impl.h"
#include "Server_init.h"

namespace CIAO
{
  namespace Deployment
  {
    ComponentServer_Task::Error::Error(const ACE_CString &err) : err_(err)
    {
    }

    ComponentServer_Task::ComponentServer_Task (int argc, ACE_TCHAR **argv)
    {
      CIAO_TRACE ("CIAO_ComponentServer_Task::CIAO_ComponentServer_Task ()");

      Logger_Service
        *clf = ACE_Dynamic_Service<Logger_Service>::instance ("CIAO_Logger");

      if (clf)
        {
          clf->init (argc, argv);
        }

      CIAO_DEBUG (9, (LM_TRACE, CLINFO
                   "CIAO_ComponentServer_Task::CIAO_ComponentServer_Task - "
                   "Creating ORB\n"));

      this->orb_ = CORBA::ORB_init (argc, argv);

      this->parse_args (argc, argv);

      CIAO::Server_init (this->orb_.in ());

      CIAO_DEBUG (9, (LM_TRACE, CLINFO
                   "CIAO_ComponentServer_Task::CIAO_ComponentServer_Task - "
                   "CIAO_ComponentServer object created.\n"));
    }

    int
    ComponentServer_Task::svc (void)
    {
      CIAO_TRACE ("ComponentServer_Task::svc");

      CIAO_DEBUG (9, (LM_TRACE, CLINFO
                   "ComponentServer_Task::svc - "
                   "Activating the root POA\n"));

      CORBA::Object_var object =
        this->orb_->resolve_initial_references ("RootPOA");

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (object.in ());

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager ();

      poa_manager->activate ();

      CIAO_DEBUG (9, (LM_TRACE, CLINFO
                   "ComponentServer_Task::svc - "
                   "Creating server implementation object\n"));

      CIAO::Deployment::CIAO_ComponentServer_i *ci_srv = 0;
      ACE_NEW_NORETURN (ci_srv,
                        CIAO_ComponentServer_i (this->uuid_,
                                                this->orb_.in (),
                                                root_poa.in ()));

      if (ci_srv == 0)
        {
          CIAO_ERROR (1, (LM_CRITICAL,
                       "ComponentServer_Task::run - "
                       "Out of memory error while allocating servant."));
          throw Error ("Out of memory whilst allocating servant.");
        }

      PortableServer::ServantBase_var safe_config = ci_srv;
      PortableServer::ObjectId_var id =
        root_poa->activate_object (ci_srv);
      CORBA::Object_var cs_object = root_poa->id_to_reference (id.in ());
      ComponentServer_var cs =
        ComponentServer::_narrow (cs_object.in ());

      if (this->output_file_ != ACE_TEXT(""))
        {
          CORBA::String_var ior = this->orb_->object_to_string (cs.in ());
          ::CIAO::Utility::write_IOR (this->output_file_.c_str (), ior.in ());
        }

      if (this->callback_ior_str_ != ACE_TEXT(""))
        {
          CIAO_DEBUG (9, (LM_TRACE, CLINFO "ComponentServer_Task::run - "
                      "Resolving callback IOR\n"));
          CORBA::Object_var obj =
            this->orb_->string_to_object (this->callback_ior_str_.c_str ());
          ServerActivator_var sa (ServerActivator::_narrow (obj));

          if (CORBA::is_nil (sa.in ()))
            {
              CIAO_DEBUG (6, (LM_ERROR, CLINFO
                           "ComponentServer_Task::svc - "
                           "Failed to narrow callback IOR [%s]\n",
                           this->callback_ior_str_.c_str ()));
              throw Error ("Failed to narrow callback IOR");
            }

          Components::ConfigValues_var config;
          {
            Components::ConfigValues *cf = 0;
            ACE_NEW_NORETURN (cf, Components::ConfigValues (0));

            if  (cf == 0)
              {
                CIAO_ERROR (1, (LM_CRITICAL, "ComponentServer_Task::run - "
                             "Out of memory error while allocating config values."));
              }
            else
              {
                config = cf;
              }
          }

          // Make callback.
          CIAO_DEBUG (9, (LM_TRACE, CLINFO
                       "ComponentServer_Task::svc - "
                       "Making callback on my ServerActivator\n"));

          try
            {
              // Callback to NodeApplication to get configuration
              sa->component_server_callback (cs.in (),
                                             this->uuid_.c_str (),
                                             config.out ());

              CIAO_DEBUG (9, (LM_TRACE, CLINFO "ComponentServer_Task::svc - "
                           "Configuration received\n"));
              // @@WO: Probably need to do something with these config values.

              ci_srv->init (sa.in (), config._retn ());

              CIAO_DEBUG (6, (LM_NOTICE, CLINFO "ComponentServer_Task::svc - "
                           "Configuration complete for component server %C\n",
                           this->uuid_.c_str ()));

              sa->configuration_complete (this->uuid_.c_str ());
            }
          catch (const ::CORBA::BAD_PARAM &)
            {
              CIAO_ERROR (1, (LM_ERROR, CLINFO "ComponentServer_Task::svc - "
                           "The Callback IOR provided pointed to the "
                           "wrong ServerActivator\n"));
              throw Error ("Bad callback IOR");
            }
          catch (const ::CORBA::Exception &ex)
            {
              CIAO_ERROR (1, (LM_ERROR, CLINFO "ComponentServer_Task::svc - "
                           "Caught CORBA Exception %C\n",
                           ex._info().c_str ()));
              throw Error ("Caught exception while calling back");
            }
          catch (...)
            {
              CIAO_ERROR (1, (LM_ERROR, CLINFO "ComponentServer_Task::svc - "
                           "Caught exception while calling back\n"));
              throw Error ("Caught exception while calling back");
            }

        }
      else
        {
          CIAO_DEBUG (9, (LM_TRACE, CLINFO "ComponentServer_Task::svc - "
                       "Initializing ComponentServer without ServantActivator "
                       "callback\n"));
          ci_srv->init (0, 0);
        }

      this->orb_->run ();

      CIAO_DEBUG (9, (LM_TRACE, CLINFO "ComponentServer_Task::svc - "
                   "ORB Event loop completed.\n"));

      root_poa->destroy (1, 1);

      this->orb_->destroy ();

      return 0;
    }

    void
    ComponentServer_Task::run (void)
    {
      CIAO_TRACE ("ComponentServer_Task::run");

      CIAO_DEBUG (6, (LM_DEBUG, CLINFO "ComponentServer_Task::run - Starting ORB\n"));
      this->svc ();
      CIAO_DEBUG (6, (LM_INFO,
                  CLINFO "ComponentServer_Task::run - ORB has "
                  "shutdown, terminating ComponentServer\n"));
    }

    void
    ComponentServer_Task::parse_args (int argc, ACE_TCHAR **argv)
    {
      CIAO_TRACE ("ComponentServer_Task::parse_args");

      CIAO_DEBUG (9, (LM_TRACE, CLINFO "ComponentServer_Task::parse_args - "
                    "parsing arguments...\n"));

      ACE_Get_Opt opts (argc, argv, ACE_TEXT("hu:c:"), 1, 0,
                        ACE_Get_Opt::RETURN_IN_ORDER);
      opts.long_option (ACE_TEXT("uuid"), 'u', ACE_Get_Opt::ARG_REQUIRED);
      opts.long_option (ACE_TEXT("callback-ior"), 'c', ACE_Get_Opt::ARG_REQUIRED);
      opts.long_option (ACE_TEXT("help"), 'h');
      opts.long_option (ACE_TEXT("log-level"),'l', ACE_Get_Opt::ARG_REQUIRED);
      opts.long_option (ACE_TEXT("trace"),'t', ACE_Get_Opt::NO_ARG);
      opts.long_option (ACE_TEXT("output-ior"),'o', ACE_Get_Opt::ARG_REQUIRED);

      int c = 0;
      while ((c = opts ()) != -1)
        {
          CIAO_DEBUG (9, (LM_TRACE, CLINFO "ComponentServer_Task::parse_args - "
                       "Found option: \"%s\" with argument \"%s\"\n",
                       opts.last_option (), opts.opt_arg ()));

          switch (c)
            {
            case 'u':
              CIAO_DEBUG (6, (LM_DEBUG, CLINFO "ComponentServer_Task::parse_args - "
                           "uuid is %s\n",
                           opts.opt_arg ()));
              this->uuid_ = ACE_TEXT_ALWAYS_CHAR (opts.opt_arg ());
              break;

            case 'c':
              CIAO_DEBUG (6, (LM_DEBUG, CLINFO "ComponentServer_Task::parse_args - "
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
              CIAO_DEBUG (6, (LM_DEBUG, CLINFO "ComponentServer_Task::parse_args - "
                           "IOR Output file: %s\n",
                           opts.opt_arg ()));
              this->output_file_ = opts.opt_arg ();
              break;

            case 'h':
              this->usage ();
              throw Error ("Command line help requested, bailing out....");

            default:
              CIAO_ERROR (1, (LM_ERROR, CLINFO " Unknown option: %s\n",
                          opts.last_option ()));
              this->usage ();
              ACE_CString err ("Unknown option ");
              err += ACE_TEXT_ALWAYS_CHAR (opts.last_option ());
              throw Error (err);
            }
        }

      // check required options.
      if (this->uuid_ == "")
        throw Error ("Option required: -u|--uuid");
      if (this->callback_ior_str_ == ACE_TEXT(""))
        {
          CIAO_ERROR (1, (LM_WARNING, CLINFO
                       "ComponentServer_Task::parse_args - Starting ComponentServer without a callback IOR\n"));
        }
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
  }
}


