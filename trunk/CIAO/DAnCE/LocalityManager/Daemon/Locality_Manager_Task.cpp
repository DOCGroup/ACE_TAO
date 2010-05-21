/**
 * @file Locality_Manager_Task.cpp
 * @author William R. Otte
 * $Id$
 *
 * Implementation and main for LocalityManager.
 */

#include "Locality_Manager_Task.h"

#include "ace/OS_NS_string.h"
#include "ace/Log_Msg.h"
#include "ace/Get_Opt.h"
#include "tao/ORB.h"
#include "tao/PortableServer/PortableServer.h"
#include "DAnCE/Logger/Logger_Service.h"
#include "DAnCE/Logger/Log_Macros.h"
#include "DAnCE/DAnCE_Utility.h"
#include "ciao/Logger/Log_Macros.h"
#include "ciao/Logger/Logger_Service.h"
#include "LocalityManager/Daemon/Locality_Manager_Impl.h"

namespace DAnCE
{
  LocalityManager_Task::Error::Error(const ACE_CString &err) : err_(err)
  {
  }

  LocalityManager_Task::LocalityManager_Task (int argc, ACE_TCHAR **argv)
  {
    DANCE_TRACE ("DAnCE_LocalityManager_Task::DAnCE_LocalityManager_Task ()");

    Logger_Service
      *dlf = ACE_Dynamic_Service<Logger_Service>::instance ("DAnCE_Logger");
    
    if (dlf)
      {
        dlf->init (argc, argv);
      }
    
    // Also initialize CIAO logger since we reuse parts of CIAO in the locality manager
    CIAO::Logger_Service
      * clf = ACE_Dynamic_Service<CIAO::Logger_Service>::instance ("CIAO_Logger");
    
    if (clf)
      {
        clf->init (argc, argv);
      }
    
    DANCE_DEBUG (9, (LM_TRACE, DLINFO
                    "DAnCE_LocalityManager_Task::DAnCE_LocalityManager_Task - "
                    "Creating ORB\n"));

    this->orb_ = CORBA::ORB_init (argc, argv);

    this->parse_args (argc, argv);

    DANCE_DEBUG (9, (LM_TRACE, DLINFO
                    "DAnCE_LocalityManager_Task::DAnCE_LocalityManager_Task - "
                    "DAnCE_LocalityManager_Task_ object created.\n"));
  }

  int
  LocalityManager_Task::svc (void)
  {
    DANCE_TRACE ("LocalityManager_Task::svc");

    DANCE_DEBUG (9, (LM_TRACE, DLINFO
                    "LocalityManager_Task::svc - "
                    "Activating the root POA\n"));

    CORBA::Object_var object =
      this->orb_->resolve_initial_references ("RootPOA");

    PortableServer::POA_var root_poa =
      PortableServer::POA::_narrow (object.in ());

    PortableServer::POAManager_var poa_manager =
      root_poa->the_POAManager ();

    poa_manager->activate ();

    DANCE_DEBUG (9, (LM_TRACE, DLINFO
                    "LocalityManager_Task::svc - "
                    "Creating server implementation object\n"));

    DAnCE::LocalityManager_i *lm_srv = 0;
    ACE_NEW_NORETURN (lm_srv,
                      LocalityManager_i (this->uuid_,
                                         this->orb_.in (),
                                         root_poa.in ()));

    if (lm_srv == 0)
      {
        DANCE_ERROR (1, (LM_CRITICAL,
                        "LocalityManager_Task::run - "
                        "Out of memory error while allocating servant."));
        throw Error ("Out of memory whilst allocating servant.");
      }

    PortableServer::ServantBase_var safe_config = lm_srv;
    PortableServer::ObjectId_var id =
      root_poa->activate_object (lm_srv);
    CORBA::Object_var lm_object = root_poa->id_to_reference (id.in ());
    DAnCE::LocalityManager_var lm =
      DAnCE::LocalityManager::_narrow (lm_object.in ());

    if (this->output_file_ != ACE_TEXT(""))
      {
        CORBA::String_var ior = this->orb_->object_to_string (lm.in ());
        ::DAnCE::Utility::write_IOR (this->output_file_.c_str (), ior.in ());
      }

    if (this->callback_ior_str_ != ACE_TEXT(""))
      {
        DANCE_DEBUG (9, (LM_TRACE, DLINFO "LocalityManager_Task::run - "
                        "Resolving callback IOR\n"));
        CORBA::Object_var obj =
          this->orb_->string_to_object (this->callback_ior_str_.c_str ());
        LocalityManagerActivator_var sa (LocalityManagerActivator::_narrow (obj));

        if (CORBA::is_nil (sa.in ()))
          {
            DANCE_DEBUG (6, (LM_ERROR, DLINFO
                            "LocalityManager_Task::svc - "
                            "Failed to narrow callback IOR [%s]\n",
                            this->callback_ior_str_.c_str ()));
            throw Error ("Failed to narrow callback IOR");
          }

        Deployment::Properties_var config;
        {
          Deployment::Properties *cf = 0;
          ACE_NEW_NORETURN (cf, Deployment::Properties (0));

          if  (cf == 0)
            {
              DANCE_ERROR (1, (LM_CRITICAL, "LocalityManager_Task::run - "
                              "Out of memory error while allocating config values."));
            }
          else
            {
              config = cf;
            }
        }

        // Make callback.
        DANCE_DEBUG (9, (LM_TRACE, DLINFO
                        "LocalityManager_Task::svc - "
                        "Making callback on my Activator\n"));

        try
          {
            // Callback to NodeApplication to get configuration
            sa->locality_manager_callback (lm.in (),
                                           this->uuid_.c_str (),
                                           config.out ());

            DANCE_DEBUG (9, (LM_TRACE, DLINFO "LocalityManager_Task::svc - "
                            "Configuration received\n"));
            
            lm_srv->init (config._retn ());

            DANCE_DEBUG (6, (LM_NOTICE, DLINFO "LocalityManager_Task::svc - "
                            "Configuration complete for component server %C\n",
                            this->uuid_.c_str ()));

            sa->configuration_complete (this->uuid_.c_str ());
          }
        catch (const CORBA::BAD_PARAM &)
          {
            DANCE_ERROR (1, (LM_ERROR, DLINFO "LocalityManager_Task::svc - "
                            "The Callback IOR provided pointed to the "
                            "wrong Activator\n"));
            throw Error ("Bad callback IOR");
          }
        catch (...)
          {
            DANCE_ERROR (1, (LM_ERROR, DLINFO "LocalityManager_Task::svc - "
                            "Caught exception while calling back\n"));
            throw Error ("Caught exception while calling back");
          }

      }
    else
      {
        DANCE_DEBUG (9, (LM_TRACE, DLINFO "LocalityManager_Task::svc - "
                        "Initializing ComponentServer without ServantActivator "
                        "callback\n"));
        lm_srv->init (0);
      }

    this->orb_->run ();

    DANCE_DEBUG (9, (LM_TRACE, DLINFO "LocalityManager_Task::svc - "
                    "ORB Event loop completed.\n"));

    root_poa->destroy (1, 1);

    this->orb_->destroy ();

    return 0;
  }

  void
  LocalityManager_Task::run (void)
  {
    DANCE_TRACE ("LocalityManager_Task::run");

    DANCE_DEBUG (6, (LM_DEBUG, DLINFO "LocalityManager_Task::run - Starting ORB\n"));
    this->svc ();
    DANCE_DEBUG (6, (LM_INFO,
                    DLINFO "LocalityManager_Task::run - ORB has "
                    "shutdown, terminating ComponentServer\n"));
  }

  void
  LocalityManager_Task::parse_args (int argc, ACE_TCHAR **argv)
  {
    DANCE_TRACE ("LocalityManager_Task::parse_args");

    DANCE_DEBUG (9, (LM_TRACE, DLINFO "LocalityManager_Task::parse_args - "
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
        DANCE_DEBUG (9, (LM_TRACE, DLINFO "LocalityManager_Task::parse_args - "
                        "Found option: \"%s\" with argument \"%s\"\n",
                        opts.last_option (), opts.opt_arg ()));

        switch (c)
          {
          case 'u':
            DANCE_DEBUG (6, (LM_DEBUG, DLINFO "LocalityManager_Task::parse_args - "
                            "uuid is %s\n",
                            opts.opt_arg ()));
            this->uuid_ = ACE_TEXT_ALWAYS_CHAR (opts.opt_arg ());
            break;

          case 'c':
            DANCE_DEBUG (6, (LM_DEBUG, DLINFO "LocalityManager_Task::parse_args - "
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
            DANCE_DEBUG (6, (LM_DEBUG, DLINFO "LocalityManager_Task::parse_args - "
                            "IOR Output file: %s\n",
                            opts.opt_arg ()));
            this->output_file_ = opts.opt_arg ();
            break;

          case 'h':
            this->usage ();
            throw Error ("Command line help requested, bailing out....");

          default:
            DANCE_ERROR (1, (LM_ERROR, DLINFO " Unknown option: %s\n",
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
        DANCE_ERROR (1, (LM_WARNING, DLINFO
                        "LocalityManager_Task::parse_args - Starting ComponentServer without a callback IOR\n"));
      }
  }

  void
  LocalityManager_Task::usage (void)
  {
    DANCE_TRACE ("LocalityManager_Task::usage");
    // Shouldn't be subject to CIAO's logging policy
    ACE_ERROR ((LM_EMERGENCY, "Usage: dance_locality_manager <options>\n"
                "Options:\n"
                "\t-h|--help\t\t\t\tShow help\n"
                "\t-l|--log-level <level>\t\t\tSets log level (default 5). 1 - most detailed.\n"
                "\t-u|--uuid <uuid> \t\t\tSets UUID of spawned component server (required)\n"
                "\t-c|--callback-ior <string ior>\t\tSets callback url for the spawning Activator.\n"
                "\t-o|--output-ior <filename>\t\tOutputs the IOR of the component server object to file\n"
                ));

  }
}


