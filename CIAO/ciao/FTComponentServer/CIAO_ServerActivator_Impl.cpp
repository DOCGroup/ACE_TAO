// $Id$
#include "CIAO_ServerActivator_Impl.h"

#include "ace/Log_Msg.h"
#include "ace/UUID.h"
#include "ciao/CIAO_common.h"
#include "ciao/Client_init.h"
#include "ciao/CIAO_FailureReasonsC.h"

#include "CIAO_ServerResourcesC.h"
#include "CIAO_ComponentServerC.h"
#include "CIAO_PropertiesC.h"

namespace CIAO
{
  namespace Deployment
  {
    CIAO_ServerActivator_i::CIAO_ServerActivator_i (CORBA::ULong def_spawn_delay,
                                                    const char * default_cs_path,
                                                    const char * cs_args,
                                                    bool multithreaded,
                                                    CORBA::ORB_ptr orb,
                                                    PortableServer::POA_ptr poa)
      : spawn_delay_ (def_spawn_delay),
        multithreaded_ (multithreaded),
        orb_ (CORBA::ORB::_duplicate (orb)),
        poa_ (PortableServer::POA::_duplicate (poa)),
        cs_path_ (default_cs_path),
        cs_args_ (cs_args),
        mutex_ (),
        condition_ (mutex_)
    {
      CIAO_TRACE (CLINFO "CIAO_ServerActivator_i::CIAO_ServerActivator_i");
    }

    CIAO_ServerActivator_i::~CIAO_ServerActivator_i(void)
    {
    }

    void
    CIAO_ServerActivator_i::component_server_callback (::Components::Deployment::ComponentServer_ptr serverref,
                                                       const char * server_UUID,
                                                       ::Components::ConfigValues_out config)
    {
      CIAO_TRACE(CLINFO "CIAO_ServerActivator_i::component_server_callback");

      CIAO_DEBUG ((LM_TRACE, CLINFO
                   "CIAO_ServerActivator_i::component_server_callback - "
                   "Received callback from ComponentServer %C\n",
                   server_UUID));

      if (this->server_infos_.is_empty ())
        {
          CIAO_ERROR ((LM_ERROR, CLINFO
                       "CIAO_ServerActivator_i::component_server_callback - "
                       "Received callback from ComponentServer %C, but I don't manage any.\n",
                       server_UUID));
          throw CORBA::BAD_PARAM ();
        }

      //SERVER_INFOS::iterator i (this->server_infos_.begin ());
      //i.first ();
      Server_Info *info = 0;

      for (SERVER_INFOS::iterator i (this->server_infos_.begin ());
           !i.done (); ++i)
        {
          CIAO_DEBUG ((LM_TRACE, CLINFO
                      "CIAO_ServerActivator_i::component_server_callback - "
                      "Comparing %C with %C\n", (*i)->uuid_.c_str (), server_UUID));
          if ((*i)->uuid_ == server_UUID)
            {
              info = (*i).get ();
            }
        }

      if (info == 0)
        {          CIAO_ERROR ((LM_WARNING, CLINFO
                                "CIAO_ServerActivator_i::component_server_callback - "
                                "Received callback from ComponentServer %C, which doesn't belong to me.\n",
                                server_UUID));
          throw CORBA::BAD_PARAM ();
        }

      if (info->activated_)
        {
          CIAO_ERROR ((LM_ERROR, CLINFO
                       "CIAO_ServerActivator_i::component_server_callback - "
                       "Received callback from ComponentServer %C, which has already been configured.\n",
                       server_UUID));
          throw CORBA::BAD_INV_ORDER ();
        }

      if (!CORBA::is_nil (info->ref_))
        {
          CIAO_ERROR ((LM_ERROR, CLINFO
                       "CIAO_ServerActivator_i::component_server_callback - "
                       "Received callback from ComponentServer %C, which has already called back.\n",
                       server_UUID));
          throw CORBA::BAD_INV_ORDER ();
        }

      CIAO_DEBUG ((LM_TRACE, CLINFO
                   "CIAO_ServerActivator_i::component_server_callback - "
                   "Received callback from ComponentServer %C\n",
                   server_UUID));

      info->ref_ = ::Components::Deployment::ComponentServer::_duplicate (serverref);

      this->create_component_server_config_values (*info, config);

      // @@TODO: May want to print out configvalues here.
      CIAO_DEBUG ((LM_DEBUG, CLINFO
                   "CIAO_ServerActivator_i::component_server_callback - "
                   "Generated %u ConfigValues for ComponentServer %C\n",
                   config->length (), server_UUID));
    }

    void
    CIAO_ServerActivator_i::configuration_complete (const char *server_UUID)
    {
      CIAO_TRACE (CLINFO "CIAO_ServerActivator_i::configuration_complete");

      CIAO_DEBUG ((LM_TRACE, CLINFO
                   "CIAO_ServerActivator_i::configuration_complete - "
                   "Received configuration_complete from ComponentServer %C\n",
                   server_UUID));

      try
        {
          if (this->server_infos_.is_empty ())
            {
              CIAO_ERROR ((LM_ERROR, CLINFO
                           "CIAO_ServerActivator_i::configuration_complete - "
                           "Received callback from ComponentServer %C, but I don't manage any.\n",
                           server_UUID));
              throw CORBA::BAD_PARAM ();
            }

          Server_Info *info = 0;

          for (SERVER_INFOS::ITERATOR j (this->server_infos_);
               !j.done (); ++j)
            {
              if ((*j)->uuid_ == server_UUID)
                {
                  info = (*j).get ();
                }
            }

          if (info == 0)
            {
              CIAO_ERROR ((LM_WARNING, CLINFO
                           "CIAO_ServerActivator_i::configuration_complete - "
                           "Received configuration_complete from ComponentServer %C, which doesn't belong to me.\n",
                           server_UUID));
              throw CORBA::BAD_PARAM ();
            }

          if (info->activated_)
            {
              CIAO_ERROR ((LM_ERROR, CLINFO
                           "CIAO_ServerActivator_i::configuration_complete - "
                           "Received configuration_complete from ComponentServer %C, which has already been completed.\n",
                           server_UUID));
              throw CORBA::BAD_INV_ORDER ();
            }

          if (CORBA::is_nil (info->ref_.in ()))
            {
              CIAO_ERROR ((LM_ERROR, CLINFO
                           "CIAO_ServerActivator_i::configuration_complete - "
                           "Received configuration_complete from ComponentServer %C, which has not called back.\n",
                           server_UUID));
              throw CORBA::BAD_INV_ORDER ();
            }

          info->activated_ = true;
        }
      catch (...)
        {
          CIAO_ERROR ((LM_ERROR, CLINFO
                       "CIAO_ServerActivator_i::configuration_complete - "
                       "Caught unknown exception while processing configuration_complete\n"));
          throw;
        }
    }

    ::Components::Deployment::ComponentServer_ptr
    CIAO_ServerActivator_i::create_component_server (const ::Components::ConfigValues & config)
    {
      CIAO_TRACE(CLINFO "CIAO_ServerActivator_i::create_component_server");

      Safe_Server_Info server (new Server_Info (config.length () + 1));

      CIAO::Utility::build_config_values_map (*server->cmap_, config);

      ACE_CString cmd_options = this->construct_command_line (*server);

      CIAO_DEBUG ((LM_DEBUG, CLINFO
                   "CIAO_ServerActivator_i::create_component_server - ComponentServer arguments: %C\n",
                   cmd_options.c_str ()));

      server_infos_.insert_tail (server);

      CIAO_DEBUG ((LM_TRACE, CLINFO
                   "CIAO_ServerActivator_i::create_component_server - "
                   "Attempting to spawn ComponentServer with UUID %C\n",
                   server->uuid_.c_str ()));
      // Now we need to get a copy of the one that was inserted...
      pid_t pid = this->spawn_component_server (*server, cmd_options);
      ACE_UNUSED_ARG (pid);

      ACE_Time_Value timeout (this->spawn_delay_);

      CORBA::Any val;

      if (server->cmap_->find (SERVER_TIMEOUT, val) == 0)
        {
          CORBA::ULong t;
          if (val >>= t)
            {
              CIAO_DEBUG ((LM_DEBUG, CLINFO "CIAO_ServerActivator_i::create_component_server - "
                           "Using provided non-default server timeout of %u\n", t));
              timeout = ACE_Time_Value (t);
            }
          else
            {
              CIAO_ERROR ((LM_WARNING, CLINFO "CIAO_ServerActivator_i::create_component_server - "
                           "Failed to extract provided non-default server timeout from property '%C', "
                           "falling back to default timeout of %u\n",
                           this->spawn_delay_));
            }
        }

      if (this->multithreaded_)
        this->multi_threaded_wait_for_callback (*server, timeout/*, pid*/);
      else
        this->single_threaded_wait_for_callback (*server, timeout/*, pid*/);

      CIAO_DEBUG ((LM_DEBUG, CLINFO
                   "CIAO_ServerActivator_i::create_component_server - "
                   "ComponentServer %C successfully spawned and configured!\n",
                   server->uuid_.c_str ()));

      return ::Components::Deployment::ComponentServer::_duplicate (server->ref_.in ());
    }

    ACE_CString
    CIAO_ServerActivator_i::construct_command_line (Server_Info &server)
    {
      CIAO_TRACE (CLINFO "CIAO_ServerActivator_i::construct_command_line");
      // Build our command line to launch the compoent server
      ACE_CString cmd_options (this->cs_args_);

      CORBA::Any val;

      if (server.cmap_->find (SERVER_UUID, val) == 0)
        {
          // Nodeapplication has requested a custom uuid
          CIAO_DEBUG ((LM_TRACE, CLINFO
                       "CIAO_ServerActivator_i::construct_command_line - Using provided UUID\n"));
          const char *uuid = 0;
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
                   " with UUID %C\n", server.uuid_.c_str ()));

      cmd_options += " -u ";
      cmd_options += server.uuid_;

      if (server.cmap_->find (SERVER_RESOURCES, val) == 0)
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
                           "CIAO_ServerActivator_i::construct_command_line - Adding argument %C from ServerResource\n ",
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
                           "CIAO_ServerActivator_i::construct_command_line - Using SvcConf file %C\n ",
                           sr->svcconf.in ()));
              cmd_options += " -ORBSvcConf ";
              cmd_options += sr->svcconf;
            }
        }

      return cmd_options;
    }

    pid_t
    CIAO_ServerActivator_i::spawn_component_server (const Server_Info &si,
                                                    const ACE_CString &cmd_line)
    {
      CIAO_TRACE ("CIAO_ServerActivator_i::spawn_component_server");

      ACE_Process_Options options;

      // Get my object reference
      CORBA::Object_var obj = this->poa_->servant_to_reference (this);
      CORBA::String_var ior = this->orb_->object_to_string (obj.in ());
      CORBA::Any val;

      const char *path = this->cs_path_.c_str ();

      if (si.cmap_->find (SERVER_EXECUTABLE, val) == 0)
        {
          val >>= path;
          CIAO_DEBUG ((LM_DEBUG, CLINFO "CIAO_ServerActivator_i::spawn_component_server - "
                       "Using provided component server executable:%C\n", path));
        }
      else
        {
          CIAO_DEBUG ((LM_DEBUG, CLINFO "CIAO_ServerActivator_i::spawn_component_server - "
                       "Using default component server execuable\n"));
        }

      options.command_line ("%s %s -c %s",
                            path,
                            cmd_line.c_str (),
                            ior.in ());

      options.avoid_zombies (0);

      CIAO_DEBUG ((LM_TRACE, CLINFO
                   "CIAO_ServerActivator_i::spawn_component_server - Spawning process, command line is %C\n",
                   options.command_line_buf ()));

      pid_t const pid = this->process_manager_.spawn (options,
                                                      &this->child_handler_);

      if (pid == ACE_INVALID_PID)
        {
          CIAO_ERROR ((LM_ERROR, CLINFO
                       "Failed to spawn a ComponentServer process\n"));
          throw Components::CreateFailure (CIAO::SERVER_SPAWN_FAILURE);
        }

      CIAO_DEBUG ((LM_TRACE, CLINFO
                   "CIAO_ServerActivator_i::spawn_component_server - Process successfully spawned with pid %u\n",
                   pid));
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
                           "Timed out while waiting for ComponentServer %C to call back.\n",
                           si.uuid_.c_str ()));
              throw ::Components::CreateFailure (CIAO::CALLBACK_TIMEOUT_EXCEEDED);
            }

          if (si.activated_)
            {
              break;
            }
        }
    }

    void
    CIAO_ServerActivator_i::
    multi_threaded_wait_for_callback (const Server_Info &si,
                                      ACE_Time_Value &timeout)
    {
      CIAO_TRACE (CLINFO "CIAO_ServerActivator_i::multi_threaded_wait_for_callback");

      // Wait for a conditional variable
      ACE_GUARD_THROW_EX ( ACE_SYNCH_MUTEX,
                           guard,
                           this->mutex_,
                           CORBA::NO_RESOURCES ());

      while (! si.activated_ )
        if (this->condition_.wait (&timeout) == -1)
          {
            CIAO_ERROR ((LM_ERROR, CLINFO
                         "CIAO_ServerActivator_i::multi_threaded_wait_for_callback - "
                         "Timed out while waiting for ComponentServer %C to call back.\n",
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

      Server_Info *info = 0;

      for (SERVER_INFOS::ITERATOR i (this->server_infos_);
           !i.done (); ++i)
        {
          if ((*i)->ref_->_is_equivalent (server))
            {
              info = (*i).get ();
            }
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
                       "Calling remove () on ComponentServer %C\n",
                       info->uuid_.c_str ()));
          server->remove ();
        }
      catch (::Components::RemoveFailure &)
        {
          CIAO_ERROR ((LM_WARNING, CLINFO
                       "CIAO_ServerActivator_i::remove_component_server - "
                       "Received RemoveFailure exception from ComponentServer %C\n",
                       info->uuid_.c_str ()));
        }

      // If this is a CIAO component server, call shutdown
      CIAO::Deployment::ComponentServer_var ccs =
        CIAO::Deployment::ComponentServer::_narrow (server);
      if (!CORBA::is_nil (ccs))
        {
          CIAO_DEBUG ((LM_TRACE, CLINFO
                       "CIAO_ServerActivator_i::remove_component_server - "
                       "Calling shutdown () on ComponentServer %C\n",
                       info->uuid_.c_str ()));
          ccs->shutdown ();
        }
      else
        CIAO_DEBUG ((LM_TRACE, CLINFO
                     "CIAO_ServerActivator_i::remove_component_server - "
                     "ComponentServer %C is not a CIAO_ComponentServer, not calling shutdown.\n",
                     info->uuid_.c_str ()));

      CIAO_DEBUG ((LM_INFO, CLINFO
                   "CIAO_ServerActivator_i::remove_component_server - "
                   "ComponentServer %C successfully shut down.\n",
                   info->uuid_.c_str ()));
    }

    ::Components::Deployment::ComponentServers *
    CIAO_ServerActivator_i::get_component_servers (void)
    {
      CIAO_TRACE(CLINFO "CIAO_ServerActivator_i::get_component_servers");

      ::Components::Deployment::ComponentServers_var retval = new
          Components::Deployment::ComponentServers (this->server_infos_.size ());

      CORBA::ULong pos = 0;

      for (SERVER_INFOS::ITERATOR i (this->server_infos_);
           !i.done (); ++i)
        {
          retval[pos++] = 
            ::Components::Deployment::ComponentServer::_duplicate ((*i)->ref_);
        }

      return retval._retn ();
    }

    void
    CIAO_ServerActivator_i::create_component_server_config_values (
      const Server_Info &,
      Components::ConfigValues_out &config)
    {
      ACE_NEW_THROW_EX (config,
                        Components::ConfigValues (0),
                        CORBA::NO_MEMORY ());
    }
  }
}

