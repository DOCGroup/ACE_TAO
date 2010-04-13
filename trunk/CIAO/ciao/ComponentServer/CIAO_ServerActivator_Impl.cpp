// $Id$

#include "CIAO_ServerActivator_Impl.h"
#include "ace/Log_Msg.h"
#include "ace/UUID.h"
#include "ciao/Logger/Log_Macros.h"

#include "CIAO_FailureReasonsC.h"
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
                                                    Components::Deployment::ComponentInstallation_ptr ci,
                                                    CORBA::ORB_ptr orb,
                                                    PortableServer::POA_ptr poa)
      : spawn_delay_ (def_spawn_delay),
        multithreaded_ (multithreaded),
        orb_ (CORBA::ORB::_duplicate (orb)),
        poa_ (PortableServer::POA::_duplicate (poa)),
        cs_path_ (default_cs_path),
        cs_args_ (cs_args),
        mutex_ (),
        condition_ (mutex_),
        ci_ (Components::Deployment::ComponentInstallation::_duplicate (ci))

    {
      CIAO_TRACE ("CIAO_ServerActivator_i::CIAO_ServerActivator_i");
      ACE_Utils::UUID_GENERATOR::instance ()->init ();
    }

    CIAO_ServerActivator_i::~CIAO_ServerActivator_i(void)
    {
    }

    void
    CIAO_ServerActivator_i::component_server_callback (
      ::Components::Deployment::ComponentServer_ptr serverref,
      const char * server_UUID,
      ::Components::ConfigValues_out config)
    {
      CIAO_TRACE("CIAO_ServerActivator_i::component_server_callback");

      CIAO_DEBUG (9, (LM_TRACE, CLINFO
                   "CIAO_ServerActivator_i::component_server_callback - "
                   "Received callback from ComponentServer %C\n",
                   server_UUID));

      Server_Info *info = 0;

      for (SERVER_INFOS::iterator i (this->server_infos_.begin ());
           !i.done (); ++i)
        {
          CIAO_DEBUG (9, (LM_TRACE, CLINFO
                      "CIAO_ServerActivator_i::component_server_callback - "
                      "Comparing %C with %C\n", (*i)->uuid_.c_str (), server_UUID));
          if ((*i)->uuid_ == server_UUID)
            {
              info = (*i).get ();
            }
        }

      if (!info)
        {
          CIAO_ERROR (1, (LM_WARNING, CLINFO
                      "CIAO_ServerActivator_i::component_server_callback - "
                      "Received callback from ComponentServer %C, which doesn't belong to me.\n",
                      server_UUID));
          throw ::CORBA::BAD_PARAM ();
        }

      if (info->activated_)
        {
          CIAO_ERROR (1, (LM_ERROR, CLINFO
                       "CIAO_ServerActivator_i::component_server_callback - "
                       "Received callback from ComponentServer %C, which has already been configured.\n",
                       server_UUID));
          throw ::CORBA::BAD_INV_ORDER ();
        }

      if (!CORBA::is_nil (info->ref_))
        {
          CIAO_ERROR (1, (LM_ERROR, CLINFO
                       "CIAO_ServerActivator_i::component_server_callback - "
                       "Received callback from ComponentServer %C, which has already called back.\n",
                       server_UUID));
          throw ::CORBA::BAD_INV_ORDER ();
        }

      CIAO_DEBUG (9, (LM_TRACE, CLINFO
                   "CIAO_ServerActivator_i::component_server_callback - "
                   "Received callback from ComponentServer %C\n",
                   server_UUID));

      info->ref_ = ::Components::Deployment::ComponentServer::_duplicate (serverref);

      this->create_component_server_config_values (*info, config);

      // @@TODO: May want to print out configvalues here.
      CIAO_DEBUG (6, (LM_DEBUG, CLINFO
                   "CIAO_ServerActivator_i::component_server_callback - "
                   "Generated %u ConfigValues for ComponentServer %C\n",
                   config->length (), server_UUID));
    }

    void
    CIAO_ServerActivator_i::configuration_complete (const char *server_UUID)
    {
      CIAO_TRACE ("CIAO_ServerActivator_i::configuration_complete");

      CIAO_DEBUG (9, (LM_TRACE, CLINFO
                   "CIAO_ServerActivator_i::configuration_complete - "
                   "Received configuration_complete from ComponentServer %C\n",
                   server_UUID));

      try
        {
          Server_Info *info = 0;

          for (SERVER_INFOS::ITERATOR j (this->server_infos_);
               !j.done (); ++j)
            {
              if ((*j)->uuid_ == server_UUID)
                {
                  info = (*j).get ();
                }
            }

          if (!info)
            {
              CIAO_ERROR (1, (LM_WARNING, CLINFO
                           "CIAO_ServerActivator_i::configuration_complete - "
                           "Received configuration_complete from ComponentServer %C, which doesn't belong to me.\n",
                           server_UUID));
              throw ::CORBA::BAD_PARAM ();
            }

          if (info->activated_)
            {
              CIAO_ERROR (1, (LM_ERROR, CLINFO
                           "CIAO_ServerActivator_i::configuration_complete - "
                           "Received configuration_complete from ComponentServer %C, which has already been completed.\n",
                           server_UUID));
              throw ::CORBA::BAD_INV_ORDER ();
            }

          if (CORBA::is_nil (info->ref_.in ()))
            {
              CIAO_ERROR (1, (LM_ERROR, CLINFO
                           "CIAO_ServerActivator_i::configuration_complete - "
                           "Received configuration_complete from ComponentServer %C, which has not called back.\n",
                           server_UUID));
              throw ::CORBA::BAD_INV_ORDER ();
            }

          info->activated_ = true;
        }
      catch (...)
        {
          CIAO_ERROR (1, (LM_ERROR, CLINFO
                       "CIAO_ServerActivator_i::configuration_complete - "
                       "Caught unknown exception while processing configuration_complete\n"));
          throw;
        }
    }

    ::Components::Deployment::ComponentServer_ptr
    CIAO_ServerActivator_i::create_component_server (const ::Components::ConfigValues & config)
    {
      CIAO_TRACE("CIAO_ServerActivator_i::create_component_server");

      Safe_Server_Info server (new Server_Info (config.length () + 1));

      CIAO::Utility::build_config_values_map (*server->cmap_, config);

      ACE_CString cmd_options = this->construct_command_line (*server);

      CIAO_DEBUG (6, (LM_DEBUG, CLINFO
                   "CIAO_ServerActivator_i::create_component_server - ComponentServer arguments: %C\n",
                   cmd_options.c_str ()));

      server_infos_.insert_tail (server);

      CIAO_DEBUG (9, (LM_TRACE, CLINFO
                   "CIAO_ServerActivator_i::create_component_server - "
                   "Attempting to spawn ComponentServer with UUID %C\n",
                   server->uuid_.c_str ()));

      // Now we need to get a copy of the one that was inserted...
      pid_t const pid = this->spawn_component_server (*server, cmd_options);
      ACE_UNUSED_ARG (pid);

      ACE_Time_Value timeout (this->spawn_delay_);

      CORBA::Any val;

      if (server->cmap_->find (SERVER_TIMEOUT, val) == 0)
        {
          CORBA::ULong t;
          if (val >>= t)
            {
              CIAO_DEBUG (6, (LM_DEBUG, CLINFO "CIAO_ServerActivator_i::create_component_server - "
                           "Using provided non-default server timeout of %u\n", t));
              timeout = ACE_Time_Value (t);
            }
          else
            {
              CIAO_ERROR (1, (LM_WARNING, CLINFO "CIAO_ServerActivator_i::create_component_server - "
                           "Failed to extract provided non-default server timeout from property '%C', "
                           "falling back to default timeout of %u\n",
                           this->spawn_delay_));
            }
        }

      if (this->multithreaded_)
        this->multi_threaded_wait_for_callback (*server, timeout/*, pid*/);
      else
        this->single_threaded_wait_for_callback (*server, timeout/*, pid*/);

      CIAO_DEBUG (6, (LM_DEBUG, CLINFO
                   "CIAO_ServerActivator_i::create_component_server - "
                   "ComponentServer %C successfully spawned and configured!\n",
                   server->uuid_.c_str ()));

      return ::Components::Deployment::ComponentServer::_duplicate (server->ref_.in ());
    }

    ACE_CString
    CIAO_ServerActivator_i::construct_command_line (Server_Info &server)
    {
      CIAO_TRACE ("CIAO_ServerActivator_i::construct_command_line");

      // Build our command line to launch the component server
      ACE_CString cmd_options (this->cs_args_);

      CORBA::Any val;

      if (server.cmap_->find (SERVER_UUID, val) == 0)
        {
          // Nodeapplication has requested a custom uuid
          CIAO_DEBUG (9, (LM_TRACE, CLINFO
                       "CIAO_ServerActivator_i::construct_command_line - Using provided UUID\n"));
          const char *uuid = 0;
          val >>= uuid;
          server.uuid_ = uuid;
        }
      else
        {
          CIAO_DEBUG (9, (LM_TRACE, CLINFO
                       "CIAO_ServerActivator_i::construct_command_line - Using generated UUID\n"));
          ACE_Utils::UUID uuid;
          ACE_Utils::UUID_GENERATOR::instance ()->generate_UUID (uuid);
          server.uuid_ = *uuid.to_string ();
        }

      if (server.cmap_->find (SERVER_ARGUMENTS, val) == 0)
        {
          const char *args = 0;
          val >>= args;

          CIAO_DEBUG (9, (LM_TRACE, CLINFO
                 "CIAO_ServerActivator_i::construct_command_line - "
                 "Adding provided server arguments %C\n", args));

          cmd_options += ' ';
          cmd_options += args;
          cmd_options += ' ';
        }

      CIAO_DEBUG (6, (LM_DEBUG, CLINFO
                   "CIAO_ServerActivator_i::construct_command_line - Creating component server"
                   " with UUID %C\n", server.uuid_.c_str ()));

      cmd_options += " -u ";
      cmd_options += server.uuid_;

      return cmd_options;
    }

    pid_t
    CIAO_ServerActivator_i::spawn_component_server (const Server_Info &si,
                                                    const ACE_CString &cmd_line)
    {
      CIAO_TRACE ("CIAO_ServerActivator_i::spawn_component_server");

      // Get my object reference
      CORBA::Object_var obj = this->poa_->servant_to_reference (this);
      CORBA::String_var ior = this->orb_->object_to_string (obj.in ());
      CORBA::Any val;

      const char *path = this->cs_path_.c_str ();

      if (si.cmap_->find (SERVER_EXECUTABLE, val) == 0)
        {
          val >>= path;
          CIAO_DEBUG (6, (LM_DEBUG, CLINFO "CIAO_ServerActivator_i::spawn_component_server - "
                       "Using provided component server executable: %C\n", path));
        }
      else
        {
          CIAO_DEBUG (6, (LM_DEBUG, CLINFO "CIAO_ServerActivator_i::spawn_component_server - "
          "Using default component server executable\n"));
        }

      ACE_Process_Options options (true,
        ACE_OS::strlen (path) + ACE_OS::strlen (cmd_line.c_str()) + ACE_OS::strlen (ior.in ()) + 15);
      if (options.command_line ("%s %s -c %s",
                                path,
                                cmd_line.c_str (),
                                ior.in ()) != 0)
        {
          CIAO_ERROR (1, (LM_ERROR, CLINFO
                       "Failed to create commandline\n"));
          throw Components::CreateFailure (CIAO::SERVER_SPAWN_FAILURE);
        }

      options.avoid_zombies (0);

      CIAO_DEBUG (9, (LM_TRACE, CLINFO
                   "CIAO_ServerActivator_i::spawn_component_server - Spawning process, command line is %s\n",
                   options.command_line_buf ()));

      pid_t const pid = this->process_manager_.spawn (options,
                                                      &this->child_handler_);

      if (pid == ACE_INVALID_PID)
        {
          CIAO_ERROR (1, (LM_ERROR, CLINFO
                       "Failed to spawn a ComponentServer process\n"));
          throw Components::CreateFailure (CIAO::SERVER_SPAWN_FAILURE);
        }

      CIAO_DEBUG (9, (LM_TRACE, CLINFO
                   "CIAO_ServerActivator_i::spawn_component_server - Process successfully spawned with pid %u\n",
                   pid));
      return pid;
    }

    void
    CIAO_ServerActivator_i::
    single_threaded_wait_for_callback (const Server_Info &si,
                                       ACE_Time_Value &timeout)
    {
      CIAO_TRACE ("CIAO_ServerActivator_i::single_threaded_wait_for_callback");

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
              CIAO_ERROR (1, (LM_ERROR, CLINFO
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
      CIAO_TRACE ("CIAO_ServerActivator_i::multi_threaded_wait_for_callback");

      // Wait for a conditional variable
      ACE_GUARD_THROW_EX ( TAO_SYNCH_MUTEX,
                           guard,
                           this->mutex_,
                           CORBA::NO_RESOURCES ());

      while (! si.activated_ )
        if (this->condition_.wait (&timeout) == -1)
          {
            CIAO_ERROR (1, (LM_ERROR, CLINFO
                         "CIAO_ServerActivator_i::multi_threaded_wait_for_callback - "
                         "Timed out while waiting for ComponentServer %C to call back.\n",
                         si.uuid_.c_str ()));
            throw Components::CreateFailure (CIAO::CALLBACK_TIMEOUT_EXCEEDED);
          }
    }

    void
    CIAO_ServerActivator_i::remove_component_server (::Components::Deployment::ComponentServer_ptr server)
    {
      CIAO_TRACE ("CIAO_ServerActivator_i::remove_component_server");

      Server_Info *info = 0;

      for (SERVER_INFOS::ITERATOR i (this->server_infos_);
           !i.done (); ++i)
        {
          if ((*i)->ref_->_is_equivalent (server))
            {
              info = (*i).get ();
            }
        }

      if (!info)
        {
          CIAO_ERROR (1, (LM_ERROR, CLINFO
                       "CIAO_ServerActivator_i::remove_component_server - "
                       "Failed to find equivalent ComponentServer under my management.\n"));
          throw ::Components::RemoveFailure ();
        }

      try
        {
          CIAO_DEBUG (9, (LM_TRACE, CLINFO
                       "CIAO_ServerActivator_i::remove_component_server - "
                       "Calling remove () on ComponentServer %C\n",
                       info->uuid_.c_str ()));
          server->remove ();
        }
      catch (::Components::RemoveFailure &)
        {
          CIAO_ERROR (1, (LM_WARNING, CLINFO
                       "CIAO_ServerActivator_i::remove_component_server - "
                       "Received RemoveFailure exception from ComponentServer %C\n",
                       info->uuid_.c_str ()));
        }

      // If this is a CIAO component server, call shutdown
      CIAO::Deployment::ComponentServer_var ccs =
        CIAO::Deployment::ComponentServer::_narrow (server);
      if (!CORBA::is_nil (ccs))
        {
          CIAO_DEBUG (9, (LM_TRACE, CLINFO
                       "CIAO_ServerActivator_i::remove_component_server - "
                       "Calling shutdown () on ComponentServer %C\n",
                       info->uuid_.c_str ()));
          ccs->shutdown ();
        }
      else
        CIAO_DEBUG (9, (LM_TRACE, CLINFO
                     "CIAO_ServerActivator_i::remove_component_server - "
                     "ComponentServer %C is not a CIAO_ComponentServer, not calling shutdown.\n",
                     info->uuid_.c_str ()));

      CIAO_DEBUG (6, (LM_INFO, CLINFO
                   "CIAO_ServerActivator_i::remove_component_server - "
                   "ComponentServer %C successfully shut down.\n",
                   info->uuid_.c_str ()));
    }

    ::Components::Deployment::ComponentServers *
    CIAO_ServerActivator_i::get_component_servers (void)
    {
      CIAO_TRACE ("CIAO_ServerActivator_i::get_component_servers");

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
                        Components::ConfigValues (1),
                        CORBA::NO_MEMORY ());

      Components::Deployment::ComponentInstallation_ptr ci =
        Components::Deployment::ComponentInstallation::_duplicate (this->ci_.in ());
      CORBA::Any ci_any;
      ci_any <<= ci;

      OBV_Components::ConfigValue* p = 0;
      ACE_NEW_THROW_EX (p,
                        OBV_Components::ConfigValue (),
                        CORBA::NO_MEMORY ());
      p->name (CIAO::Deployment::COMPONENTINSTALLATION_REF);
      p->value (ci_any);
      config->length (1);
      config.operator[](0) = p;
    }
  }
}

