#include "CIAO_ServerActivator_Impl.h"

#include <ace/Log_Msg.h>
#include <ace/UUID.h>
#include <ciao/CIAO_common.h>
#include <ciao/Client_init.h>
#include <ciao/CIAO_FailureReasonsC.h>

#include "CIAO_ServerResourcesC.h"
#include "CIAO_ComponentServerC.h"

const ACE_CString SERVER_RESOURCES = "edu.vanderbilt.dre.ServerResources";
const ACE_CString SERVER_UUID = "edu.vanderbilt.dre.ServerUUID";

namespace CIAO
{
  namespace Deployment
  {
    CIAO_ServerActivator_i::CIAO_ServerActivator_i (CORBA::ULong def_spawn_delay,
                                                    const char * default_cs_path,
                                                    CORBA::ORB_ptr orb,
                                                    PortableServer::POA_ptr poa)
      : spawn_delay_ (def_spawn_delay),
        multithreaded_ (true),
        orb_ (CORBA::ORB::_duplicate (orb)),
        poa_ (PortableServer::POA::_duplicate (poa)),
        cs_path_ (default_cs_path),
        mutex_ (),
        condition_ (mutex_)
    {
      CIAO_TRACE (CLINFO "CIAO_ServerActivator_i::CIAO_ServerActivator_i");
    }
    
    void
    CIAO_ServerActivator_i::component_server_callback (::Components::Deployment::ComponentServer_ptr serverref,
                                                       const char * server_UUID,
                                                       ::Components::ConfigValues_out config)
    {
      CIAO_TRACE(CLINFO "CIAO_ServerActivator_i::component_server_callback");
      
      CIAO_DEBUG ((LM_TRACE, CLINFO
                   "CIAO_ServerActivator_i::component_server_callback - "
                   "Received callback from ComponentServer %s\n",
                   server_UUID));
      
      if (this->server_infos_.is_empty ())
        {
          CIAO_ERROR ((LM_ERROR, CLINFO
                       "CIAO_ServerActivator_i::component_server_callback - "
                       "Received callback from ComponentServer %s, but I don't manage any.\n",
                       server_UUID));
          throw CORBA::BAD_PARAM ();
        }
      
      SERVER_INFOS_ITERATOR i (this->server_infos_);
      i.first ();
      Server_Info *info = 0;
      
      while ((info = i.next ()) != 0)
        {
          if (info->uuid_ != server_UUID)
            break;
        } 
        
      if (info == 0)
        {
          CIAO_ERROR ((LM_WARNING, CLINFO
                       "CIAO_ServerActivator_i::component_server_callback - "
                       "Received callback from ComponentServer %s, which doesn't belong to me.\n",
                       server_UUID));
          throw CORBA::BAD_PARAM ();
        }
      
      if (info->activated_)
        {
          CIAO_ERROR ((LM_ERROR, CLINFO
                       "CIAO_ServerActivator_i::component_server_callback - "
                       "Received callback from ComponentServer %s, which has already been configured.\n",
                       server_UUID));
          throw CORBA::BAD_INV_ORDER ();
        }
      
      if (!CORBA::is_nil (info->ref_))
        {
          CIAO_ERROR ((LM_ERROR, CLINFO
                       "CIAO_ServerActivator_i::component_server_callback - "
                       "Received callback from ComponentServer %s, which has already called back.\n",
                       server_UUID));
          throw CORBA::BAD_INV_ORDER ();
        }
      
      CIAO_DEBUG ((LM_TRACE, CLINFO
                   "CIAO_ServerActivator_i::component_server_callback - "
                   "Received callback from ComponentServer %s\n",
                   server_UUID));
      
      info->ref_ = serverref;
      
      this->create_component_server_config_values (*info, config);
      
      // @@TODO: May want to print out configvalues here.
      CIAO_DEBUG ((LM_DEBUG, CLINFO
                  "CIAO_ServerActivator_i::component_server_callback - "
                  "Generated %u ConfigValues for ComponentServer %s\n",
                  config->length (), server_UUID));
    }
    
    void 
    CIAO_ServerActivator_i::configuration_complete (const char *server_UUID)
    {
      CIAO_TRACE (CLINFO "CIAO_ServerActivator_i::configuration_complete");
      
      CIAO_DEBUG ((LM_TRACE, CLINFO
                   "CIAO_ServerActivator_i::configuration_complete - "
                   "Received configuration_complete from ComponentServer %s\n",
                   server_UUID));
      
      if (this->server_infos_.is_empty ())
        {
          CIAO_ERROR ((LM_ERROR, CLINFO
                       "CIAO_ServerActivator_i::configuration_complete - "
                       "Received callback from ComponentServer %s, but I don't manage any.\n",
                       server_UUID));
          throw CORBA::BAD_PARAM ();
        }

      SERVER_INFOS_ITERATOR i (this->server_infos_);
      i.first ();
      Server_Info *info = 0;
      
      while ((info = i.next ()) != 0)
        {
          if (info->uuid_ != server_UUID)
            break;
        } 

      if (info == 0)
        {
          CIAO_ERROR ((LM_WARNING, CLINFO
                       "CIAO_ServerActivator_i::configuration_complete - "
                       "Received configuration_complete from ComponentServer %s, which doesn't belong to me.\n",
                       server_UUID));
          throw CORBA::BAD_PARAM ();
        }
      
      if (info->activated_)
        {
          CIAO_ERROR ((LM_ERROR, CLINFO
                       "CIAO_ServerActivator_i::configuration_complete - "
                       "Received configuration_complete from ComponentServer %s, which has already been completed.\n",
                       server_UUID));
          throw CORBA::BAD_INV_ORDER ();
        }
      
      if (CORBA::is_nil (info->ref_))
        {
          CIAO_ERROR ((LM_ERROR, CLINFO
                       "CIAO_ServerActivator_i::configuration_complete - "
                       "Received configuration_complete from ComponentServer %s, which has not called back.\n",
                       server_UUID));
          throw CORBA::BAD_INV_ORDER ();
        }
      
      info->activated_ = true;
    }
    
    ::Components::Deployment::ComponentServer_ptr 
    CIAO_ServerActivator_i::create_component_server (const ::Components::ConfigValues & config)
    {
      CIAO_TRACE(CLINFO "CIAO_ServerActivator_i::create_component_server");
      
      Server_Info *tmp;
      ACE_NEW_THROW_EX (tmp, Server_Info(config.length ()), CORBA::NO_MEMORY ());
      
      auto_ptr<Server_Info> server (tmp);
      
      CIAO::Utility::build_config_values_map (server->cmap_, config);
      
      ACE_CString cmd_options = this->construct_command_line (*server);
      
      CIAO_DEBUG ((LM_DEBUG, CLINFO
                   "CIAO_ServerActivator_i::create_component_server - ComponentServer arguments: %s\n ",
                   cmd_options.c_str ()));
      
      server_infos_.insert_head (server.release ());
      
      this->spawn_component_server (cmd_options);
      
      ACE_Time_Value timeout (this->spawn_delay_, 0);

      if (this->multithreaded_)
        this->multi_threaded_wait_for_callback (*server, timeout);
      else
        this->single_threaded_wait_for_callback (*server, timeout);
      
      CIAO_DEBUG ((LM_DEBUG, CLINFO
                   "CIAO_ServerActivator_i::create_component_server - "
                   "ComponentServer %s successfully spawned and configured!\n",
                   server->uuid_.c_str ()));
      
      return server->ref_.in ();
    }
    
    ACE_CString
    CIAO_ServerActivator_i::construct_command_line (Server_Info &server)
    {
      CIAO_TRACE (CLINFO "CIAO_ServerActivator_i::construct_command_line");
      // Build our command line to launch the compoent server
      ACE_CString cmd_options;
      
      CORBA::Any val;
      
      if (server.cmap_.find (SERVER_UUID, val) == 0)
        {
          // Nodeapplication has requested a custom uuid
          CIAO_DEBUG ((LM_TRACE, CLINFO
                       "CIAO_ServerActivator_i::construct_command_line - Using provided UUID\n"));
          const char *uuid;
          val >>= uuid;
          server.uuid_ = uuid;
        }
      else
        {
          CIAO_DEBUG ((LM_TRACE, CLINFO
                       "CIAO_ServerActivator_i::construct_command_line - Using generated UUID\n"));
          ACE_Utils::UUID uuid;
          ACE_Utils::UUID_GENERATOR::instance ()->generate_UUID (uuid);
          server.uuid_ = *uuid.to_string ();
        }
      
      CIAO_DEBUG ((LM_DEBUG, CLINFO
                   "CIAO_ServerActivator_i::construct_command_line - Creating component server "
                   " with UUID %s\n", server.uuid_.c_str ()));

      cmd_options += " -u ";
      cmd_options += server.uuid_;
      
      if (server.cmap_.find (SERVER_RESOURCES, val) == 0)
        {
          // There may be command line arguments specified in the plan
          ServerResource_var sr;
          val >>= sr;
          
          // If command line options are specified through RTCCM descriptors,
          // then we should honor these command line options as well.
          for (CORBA::ULong arg_i = 0;
               arg_i < sr->args.length ();
               ++arg_i)
            {
              CIAO_DEBUG ((LM_TRACE, CLINFO
                           "CIAO_ServerActivator_i::construct_command_line - Adding argument %s from ServerResource\n ",
                           sr->args[arg_i].in ()));
              cmd_options += " "; // space between command line args
              cmd_options += sr->args[arg_i];
            }

          // If service configuration file is specified through RTCCM
          // descriptors, then we should honor it as well.
          if (ACE_OS::strcmp (sr->svcconf.in (),
                              "") != 0)
            {
              CIAO_DEBUG ((LM_TRACE, CLINFO
                           "CIAO_ServerActivator_i::construct_command_line - Using SvcConf file %s\n ",
                           sr->svcconf.in ()));
              cmd_options += " -ORBSvcConf ";
              cmd_options += sr->svcconf;
            }
        }
      
      return cmd_options;
    }
    
    pid_t
    CIAO_ServerActivator_i::spawn_component_server (const ACE_CString &cmd_line)
    {
      CIAO_TRACE ("CIAO_ServerActivator_i::spawn_component_server\n");
      
      ACE_Process_Options options;
      
      // Get my object reference
      CORBA::Object_var obj = this->poa_->servant_to_reference (this);
      CORBA::String_var ior = this->orb_->object_to_string (obj.in ());
      
      options.command_line ("%s -c %s %s",
                            this->cs_path_.c_str (),
                            ior.in (),
                            cmd_line.c_str ());
      
      options.avoid_zombies (0);
      
      CIAO_DEBUG ((LM_TRACE, CLINFO 
                   "CIAO_ServerActivator_i::spawn_component_server - Spawning process\n"));

      pid_t pid = this->process_manager_.spawn (options,
                                                &this->child_handler_);
      
      if (pid == ACE_INVALID_PID)
        {
          CIAO_ERROR ((LM_ERROR, CLINFO 
                       "Failed to spawn a ComponentServer process\n"));
          throw Components::CreateFailure (CIAO::SERVER_SPAWN_FAILURE);
        }
      
      CIAO_DEBUG ((LM_TRACE, CLINFO 
                   "CIAO_ServerActivator_i::spawn_component_server - Process successfully spawned.\n"));
      return pid;
    }

    void
    CIAO_ServerActivator_i::
    single_threaded_wait_for_callback (const Server_Info &si,
                                       ACE_Time_Value &timeout)
    {
      CIAO_TRACE (CLINFO "CIAO_ServerActivator_i::single_threaded_wait_for_callback");
      // Below code is broken for thread-per-connection concurrency model,
      // since the main thread is running ORB event loop and will spawn
      // a different thread to handle the call <register_node_application>,
      // the <perform_work> operation will not be invoked and finally
      // a timeout will occur. For a similar reason, it won't work
      // for thread-pool concurrency model.
      while (true)
        {
          this->orb_->perform_work (timeout);
          
          if (timeout == ACE_Time_Value::zero)
            {
              CIAO_ERROR ((LM_ERROR, CLINFO
                           "CIAO_ServerActivator_i::single_threaded_wait_for_callback - "
                           "Timed out while waiting for ComponentServer %s to call back.\n",
                           si.uuid_.c_str ()));
              throw ::Components::CreateFailure (CIAO::CALLBACK_TIMEOUT_EXCEEDED);
            }
          
          if (si.activated_)
            break;
        }
    }
    
    void
    CIAO_ServerActivator_i::
    multi_threaded_wait_for_callback (const Server_Info &si,
                                      ACE_Time_Value &timeout)
    {
      CIAO_TRACE (CLINFO "CIAO_ServerActivator_i::multi_threaded_wait_for_callback");
      
      // Wait for a conditional variable
      ACE_GUARD_THROW_EX ( ACE_Thread_Mutex,
                           guard,
                           this->mutex_,
                           CORBA::NO_RESOURCES ());
      
      while (! si.activated_ )
        if (this->condition_.wait (&timeout) == -1)
          {
            CIAO_ERROR ((LM_ERROR, CLINFO
                         "CIAO_ServerActivator_i::multi_threaded_wait_for_callback - "
                         "Timed out while waiting for ComponentServer %s to call back.\n",
                         si.uuid_.c_str ()));
            throw Components::CreateFailure (CIAO::CALLBACK_TIMEOUT_EXCEEDED);
          }
    }
    
    void
    CIAO_ServerActivator_i::remove_component_server (::Components::Deployment::ComponentServer_ptr server)
    {
      CIAO_TRACE(CLINFO "CIAO_ServerActivator_i::remove_component_server");
      
      if (this->server_infos_.is_empty ())
        {
          CIAO_ERROR ((LM_ERROR, CLINFO
                       "CIAO_ServerActivator_i::remove_component_server - "
                       "I don't manage any ComponentServers.\n"));
          throw CORBA::BAD_PARAM ();
        }

      SERVER_INFOS_ITERATOR i (this->server_infos_);
      i.first ();
      Server_Info *info = 0;
      
            
      while ((info = i.next ()) != 0)
        {
          if (info->ref_->_is_equivalent (server))
            break;
        } 

      if (info == 0)
        {
          CIAO_ERROR ((LM_ERROR, CLINFO
                       "CIAO_ServerActivator_i::remove_component_server - "
                       "Failed to find equivalent ComponentServer under my management.\n"));
          throw ::Components::RemoveFailure ();
        }
      
      try
        {
          CIAO_DEBUG ((LM_TRACE, CLINFO
                       "CIAO_ServerActivator_i::remove_component_server - "
                       "Calling remove () on ComponentServer %s\n",
                       info->uuid_.c_str ()));
          server->remove ();
        }
      catch (::Components::RemoveFailure &)
        {
          CIAO_ERROR ((LM_WARNING, CLINFO
                       "CIAO_ServerActivator_i::remove_component_server - "
                      "Received RemoveFailure exception from ComponentServer %s\n",
                      info->uuid_.c_str ()));
        }
      
      // If this is a CIAO component server, call shutdown
      CIAO::Deployment::ComponentServer_var ccs = 
        CIAO::Deployment::ComponentServer::_narrow (server);
      if (!CORBA::is_nil (ccs))
        {
          CIAO_DEBUG ((LM_TRACE, CLINFO
                       "CIAO_ServerActivator_i::remove_component_server - "
                       "Calling shutdown () on ComponentServer %s\n",
                       info->uuid_.c_str ()));
          ccs->shutdown ();
        }
      else
        CIAO_DEBUG ((LM_TRACE, CLINFO
                     "CIAO_ServerActivator_i::remove_component_server - "
                     "ComponentServer %s is not a CIAO_ComponentServer, not calling shutdown.\n",
                     info->uuid_.c_str ()));
      
      CIAO_DEBUG ((LM_INFO, CLINFO
                   "CIAO_ServerActivator_i::remove_component_server - "
                   "ComponentServer %s successfully shut down.\n",
                   info->uuid_.c_str ()));
    }
    
    ::Components::Deployment::ComponentServers *
    CIAO_ServerActivator_i::get_component_servers (void)
    {
      CIAO_TRACE(CLINFO "CIAO_ServerActivator_i::get_component_servers");
      
      ::Components::Deployment::ComponentServers_var retval = new
          Components::Deployment::ComponentServers (this->server_infos_.size ());
      
      if (retval->length () == 0)
        return retval;
      
      SERVER_INFOS_ITERATOR i (this->server_infos_);
      i.first ();
      Server_Info *info(0);
      CORBA::ULong pos = 0;
      
      while ((info = i.next ()) != 0)
        {
          retval[pos++] = ::Components::Deployment::ComponentServer::_duplicate (info->ref_);
        } 
      
      return retval._retn ();
    }
  }  
}

