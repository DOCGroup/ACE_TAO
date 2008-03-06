/**
 * @file CIAO_ComponentServer.cpp
 * @author William R. Otte
 *
 * Implementation and main for CIAO_ComponentServer.
 */

#include "CIAO_ComponentServer.h"

#include <ace/Log_Msg.h>
#include <ace/Get_Opt.h>
#include <tao/ORB.h>
#include <tao/Object.h>
#include <tao/CORBA_methods.h>
#include <tao/PortableServer/PortableServer.h>
#include <ciao/CIAO_common.h>
#include <ciao/CIAO_Logger/CIAOLoggerFactory.h>

#include "CIAO_ComponentServer_Impl.h"
#include "CIAO_CS_ClientC.h"

int ACE_TMAIN (int argc, ACE_TCHAR **argv)
{
  CIAO_TRACE ("CIAO_ComponentServer::ACE_TMAIN");
  
  try
    {
      CIAO::Deployment::ComponentServer cs (argc, argv);
      cs.run ();
      return 0;
    }
  catch (CIAO::Deployment::ComponentServer::Error &e)
    {
      ACE_DEBUG ((LM_ALERT, CLINFO "CIAO_ComponentServer main: Caught ComponentServer exception: %s\n",
                  e.err_.c_str ()));
    }
  catch (...)
    {
      ACE_DEBUG ((LM_ALERT, CLINFO "CIAO_ComponentServer main: Caught unknown exception.\n"));
    }
  
  return -1;
}

using namespace Components::Deployment::CIAO;
      
namespace CIAO
{
  namespace Deployment
  {
    ComponentServer::ComponentServer (int argc, ACE_TCHAR **argv)
      : orb_ (CORBA::ORB_init (argc, argv)),
        log_level_ (5)
    {
      CIAO_TRACE ("CIAO_ComponentServer::CIAO_ComponentServer ()");
      
      this->get_log_level (argc, argv);
      this->set_log_level ();
      
      ACE_DEBUG ((LM_TRACE, CLINFO "CIAO_ComponentServer: Creating server object\n"));
      this->parse_args (argc, argv);
      
      CIAOLoggerFactory
        *clf = ACE_Dynamic_Service<CIAOLoggerFactory>::instance ("CIAO_Logger_Backend_Factory");
      if (clf)
        {
          ACE_DEBUG ((LM_TRACE, CLINFO "CIAO_ComponentServeR: Replacing logger backend"));
          ACE_Log_Msg_Backend * backend = clf->get_logger_backend(this->orb_);
          backend->open(0);
          ACE_Log_Msg::msg_backend (backend);
          ACE_Log_Msg * ace = ACE_Log_Msg::instance();
          ace->clr_flags(ace->flags());
          ace->set_flags(ACE_Log_Msg::CUSTOM);
        }
      
      ACE_DEBUG ((LM_TRACE, CLINFO "CIAO_ComponentServer object created.\n"));
    }
    
    void 
    ComponentServer::run (void)
    {
      CIAO_TRACE ("ComponentServer::run");
      
      ACE_DEBUG ((LM_TRACE, CLINFO "CIAO_ComponentServer: resolving callback IOR\n"));
      CORBA::Object_ptr obj = this->orb_->string_to_object (this->callback_ior_str_.c_str ());
      ServerActivator_var sa (ServerActivator::_narrow (obj));
      
      if (CORBA::is_nil (sa.in ()))
        {
          ACE_DEBUG ((LM_ERROR, CLINFO "CIAO_ComponentServer: Failed to narrow callback IOR\n"));
          throw Error ("Faled to narrow callback IOR");
        }
      
      ACE_DEBUG ((LM_TRACE, CLINFO "CIAO_ComponentServer: Creating server implementation object\n"));
      CIAO::Deployment::CIAO_ComponentServer_i *ci_srv = 0;
      ACE_NEW_NORETURN (ci_srv, CIAO_ComponentServer_i (this->uuid_));
      
      if (ci_srv == 0)
        {
          ACE_ERROR ((LM_CRITICAL, "CIAO_ComponentServer: Out of memory error while allocating servant."));
          throw Error ("Out of memory whilst allocating servant.");
        }
      
      PortableServer::ServantBase_var safe (ci_srv);
      ComponentServer_var cs (ci_srv->_this ());

      Components::ConfigValues_var config;
      
      {  
        Components::ConfigValues *cf;
        ACE_NEW_NORETURN (cf, Components::ConfigValues (0));
        
        if  (cf == 0)
          {
            ACE_ERROR ((LM_CRITICAL, "CIAO_ComponentServer: Out of memory error while allocating config values."));
          }
        else config = cf;
      }
      
      
      ACE_DEBUG ((LM_TRACE, CLINFO "CIAO_ComponentServer: Calling back to ServerActivator\n"));
      // Callback to NodeApplication to get configuration
      sa->component_server_callback (cs.in (),
                                     this->uuid_.c_str (),
                                     config.out ());
      
      ACE_DEBUG ((LM_TRACE, CLINFO "CIAO_ComponentServer: Configuration received\n"));
      // @@WO: Probably need to do something with these config values.
      
      ACE_DEBUG ((LM_NOTICE, CLINFO "CIAO_ComponentServer: Configuration complete for component server %s\n",
                  this->uuid_.c_str ()));
      
      sa->configuration_complete (this->uuid_.c_str ());
      
      this->orb_->run ();
      ACE_DEBUG ((LM_TRACE, CLINFO "CIAO_ComponentServer: ORB Event loop completed.\n"));
    }
    
    void 
    ComponentServer::get_log_level (int argc, ACE_TCHAR* argv[])
    {
      CIAO_TRACE ("ComponentServer::get_log_level");

      ACE_Get_Opt opts(argc, argv, "l:", 1, 0, ACE_Get_Opt::RETURN_IN_ORDER);
      opts.long_option("log-level", 'l', ACE_Get_Opt::ARG_REQUIRED);
      int c = -1;
      while ( (c = opts ()) != -1)
        {
          if ( c  == 'l')
            {
              int j = ACE_OS::atoi (opts.opt_arg());
              if (j != 0)
                {
                  this->log_level_ = j;
                }
              else
                {
                  ACE_ERROR ( (LM_WARNING, CLINFO "--log-level without argument. Using default.\n"));
                }
            }
        }
    }
    
    void 
    ComponentServer::set_log_level ()
    {
      u_long new_mask = 0;
      if (this->log_level_ <= 1)
        {
          new_mask |= LM_TRACE;
        }
      if (this->log_level_ <= 2)
        {
          new_mask |= LM_DEBUG;
        }
      if (this->log_level_ <= 3)
        {
          new_mask |= LM_INFO;
        }
      if (this->log_level_ <= 4)
        {
          new_mask |= LM_NOTICE;
        }
      if (this->log_level_ <= 5)
        {
          new_mask |= LM_WARNING;
        }
      if (this->log_level_ <= 6)
        {
          new_mask |= LM_ERROR;
        }
      if (this->log_level_ <= 7)
        {
          new_mask |= LM_CRITICAL;
        }
      if (this->log_level_ <= 8)
        {
          new_mask |= LM_ALERT;
        }
      if (this->log_level_ <= 9)
        {
          new_mask |= LM_EMERGENCY;
        }
      ACE_Log_Msg::instance()->priority_mask(new_mask, ACE_Log_Msg::PROCESS);
      ACE_DEBUG ( (LM_TRACE, CLINFO "Logging level is set to %i\n", this->log_level_));
    }
    
    void
    ComponentServer::parse_args (int argc, ACE_TCHAR **argv)
    {
      CIAO_TRACE ("ComponentServer::parse_args");
      
      ACE_DEBUG ((LM_TRACE, "CIAO_ComponentServer - parsing arguments...\n"));
      
      ACE_Get_Opt opts (argc, argv, "hu:c:", 1, 0, 
                        ACE_Get_Opt::RETURN_IN_ORDER);
      opts.long_option ("uuid", 'u', ACE_Get_Opt::ARG_REQUIRED);
      opts.long_option ("callback-ior", 'c', ACE_Get_Opt::ARG_REQUIRED);
      opts.long_option ("help", 'h');
      
      //int j;
      char c;
      ACE_CString s;
      
      while ((c = opts ()) != -1)
        {
          ACE_DEBUG ((LM_TRACE, CLINFO "Found option: \"%s\" with argument \"%s\"\n",
                      opts.last_option (), opts.opt_arg ()));
          
          switch (c)
            {
            case 'u':
              ACE_DEBUG ((LM_DEBUG, CLINFO "CIAO_ComponentServer: uuid is %s\n",
                          opts.opt_arg ()));
              this->uuid_ = opts.opt_arg ();
              break;
              
            case 'c':
              ACE_DEBUG ((LM_DEBUG, CLINFO "CIAO_ComponentServer: callback ior is %s\n",
                          opts.opt_arg ()));
              this->callback_ior_str_ = opts.opt_arg ();
              break;
              
            case 'h':
              this->usage ();
              throw Error ("Command line help requested, bailing out....");
              
            default:
              ACE_ERROR ((LM_ERROR, CLINFO "CIAO_ComponentServer: Unknown option: %s\n",
                          opts.last_option ()));
              this->usage ();
              ACE_CString err ("Unknown option ");
              err += opts.last_option ();
              throw Error (err);
            }
        }
      
      // check required options.
      if (this->uuid_ == "")
        throw Error ("Option required: -u|--uuid");
      if (this->callback_ior_str_ == "")
        throw Error ("Option required: -c|--callback-ior");
    }
    
    void
    ComponentServer::usage (void)
    {
      CIAO_TRACE ("ComponentServer::usage");
      
      ACE_ERROR ((LM_EMERGENCY, "Usage: CIAO_ComponentServer <options>\n"
                  "Options: \n"
                  "\t-h|--help\t\tShow help\n"
                  "\t-l|--log-level <level>\t\tSets log level (default 5). 1 - most detailed.\n"
                  "\t-u|--uuid <uuid> \t\tSets UUID of spawned component server (required)\n"
                  "\t-c|--callback-ior <string ior>\t\tSets callback url for the spawning ServerActivator.\n"));
    }
    
  }
}


