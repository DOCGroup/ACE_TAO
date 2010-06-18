/**
 * @file CIAO_RTComponentServer_Task.cpp
 * @author Johnny Willemsen  <jwillemsen@remedy.nl>
 * $Id$
 *
 * Implementation and main for CIAO_RTComponentServer.
 */

#include "CIAO_RTComponentServer_Task.h"

#include "ace/OS_NS_string.h"
#include "ace/Log_Msg.h"
#include "ace/Get_Opt.h"
#include "tao/ORB.h"
#include "tao/PortableServer/PortableServer.h"
#include "tao/RTPortableServer/RTPortableServer.h"
#include "ciao/Logger/Logger_Service.h"
#include "ciao/Logger/Log_Macros.h"
#include "CIAO_ComponentServer_Impl.h"
#include "Server_init.h"
#include "tao/RTCORBA/Priority_Mapping_Manager.h"
#include "tao/Protocols_Hooks.h"
#include "tao/ORB_Core.h"

namespace CIAO
{
  namespace Deployment
  {
    CORBA::Short
    get_implicit_thread_CORBA_priority (CORBA::ORB_ptr orb)
    {
      CORBA::Object_var obj =
        orb->resolve_initial_references (TAO_OBJID_PRIORITYMAPPINGMANAGER);

      TAO_Priority_Mapping_Manager_var mapping_manager =
        TAO_Priority_Mapping_Manager::_narrow (obj.in ());

      if (CORBA::is_nil (mapping_manager.in ()))
        throw CORBA::INTERNAL ();

      RTCORBA::PriorityMapping *pm =
        mapping_manager.in ()->mapping ();

      TAO_Protocols_Hooks *tph =
        orb->orb_core ()->get_protocols_hooks ();

      CORBA::Short corba_priority;
      CORBA::Short native_priority;

      if (tph->get_thread_native_priority (native_priority) == 0 &&
          pm->to_CORBA (native_priority, corba_priority))
      {
        return corba_priority;
      }

      ACE_DEBUG ((LM_DEBUG, "get_implicit_thread_CORBA_priority - failed to access priority\n"));
      throw CORBA::DATA_CONVERSION (CORBA::OMGVMCID | 2, CORBA::COMPLETED_NO);
    }

    RTComponentServer_Task::Error::Error(const ACE_CString &err) : err_(err)
    {
    }

    RTComponentServer_Task::RTComponentServer_Task (int argc, ACE_TCHAR **argv) :
      static_threads_ (0),
      dynamic_threads_ (0)
    {
      CIAO_TRACE ("CIAO_RTComponentServer_Task::CIAO_RTComponentServer_Task");

      Logger_Service
        *clf = ACE_Dynamic_Service<Logger_Service>::instance ("CIAO_Logger");

      if (clf)
        {
          clf->init (argc, argv);
        }

      CIAO_DEBUG (9, (LM_TRACE, CLINFO
                   "CIAO_RTComponentServer_Task::CIAO_RTComponentServer_Task - "
                   "Creating ORB\n"));

      this->orb_ = CORBA::ORB_init (argc, argv);

      this->parse_args (argc, argv);

      CIAO::Server_init (this->orb_.in ());

      CIAO_DEBUG (9, (LM_TRACE, CLINFO
                   "CIAO_RTComponentServer_Task::CIAO_RTComponentServer_Task - "
                   "CIAO_ComponentServer object created.\n"));
    }

    int
    RTComponentServer_Task::svc (void)
    {
      CIAO_TRACE ("RTComponentServer_Task::svc");

      CIAO_DEBUG (9, (LM_TRACE, CLINFO
                   "RTComponentServer_Task::svc - "
                   "Activating the root POA\n"));

      CORBA::Object_var object =
        this->orb_->resolve_initial_references ("RootPOA");

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (object.in ());

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager ();

      poa_manager->activate ();

      object =
        this->orb_->resolve_initial_references ("RTORB");

      RTCORBA::RTORB_var rt_orb =
        RTCORBA::RTORB::_narrow (object.in ());
      CORBA::ULong stacksize = 0;
      CORBA::Boolean allow_request_buffering = false;
      CORBA::ULong max_buffered_requests = 0;
      CORBA::ULong max_request_buffer_size = 0;

      CIAO_DEBUG (9, (LM_TRACE, CLINFO
                   "RTComponentServer_Task::svc - "
                   "Creating server implementation object\n"));

      CORBA::PolicyList policies (0);

      if (this->static_threads_ > 0)
        {
          object =
            this->orb_->resolve_initial_references ("RTCurrent");

          RTCORBA::Current_var current =
            RTCORBA::Current::_narrow (object.in ());

          RTCORBA::Priority default_thread_priority =
            get_implicit_thread_CORBA_priority (this->orb_.in ());

          RTCORBA::ThreadpoolId threadpool_id_1 =
            rt_orb->create_threadpool (stacksize,
                                       this->static_threads_,
                                       this->dynamic_threads_,
                                       default_thread_priority,
                                       allow_request_buffering,
                                       max_buffered_requests,
                                       max_request_buffer_size);

          CORBA::Policy_var threadpool_policy_1 =
            rt_orb->create_threadpool_policy (threadpool_id_1);

          policies.length (2);
          policies[0] =
            CORBA::Policy::_duplicate (threadpool_policy_1);

          policies[1] =
            rt_orb->create_priority_model_policy (RTCORBA::SERVER_DECLARED,
                                                  0);
        }

      // Create the POA under the RootPOA.
      PortableServer::POA_var poa =
        root_poa->create_POA ("RTPOA",
                              poa_manager,
                              policies);

      // Creation of POAs is over. Destroy the Policy objects.
      for (CORBA::ULong i = 0;
           i < policies.length ();
           ++i)
        {
          policies[i]->destroy ();
        }

      CIAO::Deployment::CIAO_ComponentServer_i *ci_srv = 0;
      ACE_NEW_NORETURN (ci_srv,
                        CIAO_ComponentServer_i (this->uuid_,
                                                this->orb_.in (),
                                                poa.in ()));

      if (ci_srv == 0)
        {
          CIAO_ERROR (1, (LM_CRITICAL,
                       "RTComponentServer_Task::run - "
                       "Out of memory error while allocating servant."));
          throw Error ("Out of memory whilst allocating servant.");
        }

      PortableServer::ServantBase_var safe (ci_srv);

      PortableServer::ObjectId_var id =
        poa->activate_object (ci_srv);
      CORBA::Object_var cs_object = poa->id_to_reference (id.in ());
      ComponentServer_var cs =
        ComponentServer::_narrow (cs_object.in ());

      if (this->output_file_ != ACE_TEXT(""))
        {
          CORBA::String_var ior = this->orb_->object_to_string (cs.in ());
          ::CIAO::Utility::write_IOR (this->output_file_.c_str (), ior.in ());
        }


      if (this->callback_ior_str_ != ACE_TEXT(""))
        {
          CIAO_DEBUG (9, (LM_TRACE, CLINFO "RTComponentServer_Task::run - "
                      "Resolving callback IOR\n"));
          CORBA::Object_var obj =
            this->orb_->string_to_object (this->callback_ior_str_.c_str ());
          ServerActivator_var sa (ServerActivator::_narrow (obj));

          if (CORBA::is_nil (sa.in ()))
            {
              CIAO_DEBUG (6, (LM_ERROR, CLINFO
                           "RTComponentServer_Task::svc - "
                           "Failed to narrow callback IOR\n"));
              throw Error ("Failed to narrow callback IOR");
            }

          Components::ConfigValues_var config;
          {
            Components::ConfigValues *cf = 0;
            ACE_NEW_NORETURN (cf, Components::ConfigValues (0));

            if  (cf == 0)
              {
                CIAO_ERROR (1, (LM_CRITICAL, "RTComponentServer_Task::run - "
                             "Out of memory error while allocating config values."));
              }
            else
              {
                config = cf;
              }
          }

          // Make callback.
          CIAO_DEBUG (9, (LM_TRACE, CLINFO
                       "RTComponentServer_Task::svc - "
                       "Making callback on my ServerActivator\n"));

          try
            {
              // Callback to NodeApplication to get configuration
              sa->component_server_callback (cs.in (),
                                             this->uuid_.c_str (),
                                             config.out ());

              CIAO_DEBUG (9, (LM_TRACE, CLINFO "RTComponentServer_Task::svc - "
                           "Configuration received\n"));
              // @@WO: Probably need to do something with these config values.

              ci_srv->init (sa.in (), config._retn ());

              CIAO_DEBUG (6, (LM_NOTICE, CLINFO "RTComponentServer_Task::svc - "
                           "Configuration complete for component server %C\n",
                           this->uuid_.c_str ()));

              sa->configuration_complete (this->uuid_.c_str ());
            }
          catch (const CORBA::BAD_PARAM &)
            {
              CIAO_ERROR (1, (LM_ERROR, CLINFO "RTComponentServer_Task::svc - "
                           "The Callback IOR provided pointed to the "
                           "wrong ServerActivator\n"));
              throw Error ("Bad callback IOR");
            }
          catch (const ::CORBA::Exception &ex)
            {
              CIAO_ERROR (1, (LM_ERROR, CLINFO "RTComponentServer_Task::svc - "
                           "Caught CORBA Exception %C\n",
                           ex._info().c_str ()));
              throw Error ("Caught exception while calling back");
            }
          catch (...)
            {
              CIAO_ERROR (1, (LM_ERROR, CLINFO "RTComponentServer_Task::svc - "
                           "Caught exception while calling back\n"));
              throw Error ("Caught exception while calling back");
            }

        }
      else
        {
          CIAO_DEBUG (9, (LM_TRACE, CLINFO "RTComponentServer_Task::svc - "
                       "Initializing ComponentServer without ServantActivator "
                       "callback\n"));
          ci_srv->init (0, 0);
        }

      this->orb_->run ();

      CIAO_DEBUG (9, (LM_TRACE, CLINFO "RTComponentServer_Task::svc - "
                   "ORB Event loop completed.\n"));

      root_poa->destroy (1, 1);

      this->orb_->destroy ();

      return 0;
    }

    void
    RTComponentServer_Task::run (void)
    {
      CIAO_TRACE ("RTComponentServer_Task::run");

      CIAO_DEBUG (6, (LM_DEBUG, CLINFO "RTComponentServer_Task::run - Starting ORB\n"));
      this->svc ();
      CIAO_DEBUG (6, (LM_INFO,
                  CLINFO "RTComponentServer_Task::run - ORB has "
                  "shutdown, terminating ComponentServer\n"));
    }

    void
    RTComponentServer_Task::parse_args (int argc, ACE_TCHAR **argv)
    {
      CIAO_TRACE ("RTComponentServer_Task::parse_args");

      CIAO_DEBUG (9, (LM_TRACE, CLINFO "RTComponentServer_Task::parse_args - "
                    "parsing arguments...\n"));

      ACE_Get_Opt opts (argc, argv, ACE_TEXT("hu:c:s:d:"), 1, 0,
                        ACE_Get_Opt::RETURN_IN_ORDER);
      opts.long_option (ACE_TEXT("uuid"), 'u', ACE_Get_Opt::ARG_REQUIRED);
      opts.long_option (ACE_TEXT("callback-ior"), 'c', ACE_Get_Opt::ARG_REQUIRED);
      opts.long_option (ACE_TEXT("help"), 'h');
      opts.long_option (ACE_TEXT("log-level"),'l', ACE_Get_Opt::ARG_REQUIRED);
      opts.long_option (ACE_TEXT("trace"),'t', ACE_Get_Opt::NO_ARG);
      opts.long_option (ACE_TEXT("output-ior"),'o', ACE_Get_Opt::ARG_REQUIRED);
      opts.long_option (ACE_TEXT("static"),'s', ACE_Get_Opt::ARG_REQUIRED);
      opts.long_option (ACE_TEXT("dynamic"),'d', ACE_Get_Opt::ARG_REQUIRED);

      int c = 0;
      while ((c = opts ()) != -1)
        {
          CIAO_DEBUG (9, (LM_TRACE, CLINFO "RTComponentServer_Task::parse_args - "
                       "Found option: \"%s\" with argument \"%s\"\n",
                       opts.last_option (), opts.opt_arg ()));

          switch (c)
            {
            case 's':
              this->static_threads_ = ACE_OS::atoi (opts.opt_arg ());
              CIAO_DEBUG (6, (LM_DEBUG, CLINFO "RTComponentServer_Task::parse_args - "
                           "Using %d static threads\n",
                           this->static_threads_));
              break;

            case 'd':
              this->dynamic_threads_ = ACE_OS::atoi (opts.opt_arg ());
              CIAO_DEBUG (6, (LM_DEBUG, CLINFO "RTComponentServer_Task::parse_args - "
                           "Using %d dynamic threads\n",
                           this->dynamic_threads_));
              break;

            case 'u':
              CIAO_DEBUG (6, (LM_DEBUG, CLINFO "RTComponentServer_Task::parse_args - "
                           "uuid is %s\n",
                           opts.opt_arg ()));
              this->uuid_ = ACE_TEXT_ALWAYS_CHAR (opts.opt_arg ());
              break;

            case 'c':
              CIAO_DEBUG (6, (LM_DEBUG, CLINFO "RTComponentServer_Task::parse_args - "
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
              CIAO_DEBUG (6, (LM_DEBUG, CLINFO "RTComponentServer_Task::parse_args - "
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
                       "RTComponentServer_Task::parse_args - Starting ComponentServer without a callback IOR\n"));
        }
    }

    void
    RTComponentServer_Task::usage (void)
    {
      CIAO_TRACE ("RTComponentServer_Task::usage");
      // Shouldn't be subject to CIAO's logging policy
      ACE_ERROR ((LM_EMERGENCY, "Usage: CIAO_ComponentServer <options>\n"
                   "Options:\n"
                   "\t-h|--help\t\t\t\tShow help\n"
                   "\t-l|--log-level <level>\t\t\tSets log level (default 5). 1 - most detailed.\n"
                   "\t-u|--uuid <uuid> \t\t\tSets UUID of spawned component server (required)\n"
                   "\t-c|--callback-ior <string ior>\t\tSets callback url for the spawning ServerActivator.\n"
                   "\t-o|--output-ior <filename>\t\tOutputs the IOR of the component server object to file\n"
                   "\t-s|--static <number>\t\tNumber of static threads to run\n"
                   "\t-d|--dynamic <number>\t\tNumber of dynamic threads to run\n"
                   ));

    }
  }
}


