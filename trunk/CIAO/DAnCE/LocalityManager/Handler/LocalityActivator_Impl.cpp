// $Id$

#include "LocalityActivator_Impl.h"
#include "ace/Log_Msg.h"
#include "ace/UUID.h"
#include "DAnCE/Logger/Log_Macros.h"
#include "DAnCE/DAnCE_PropertiesC.h"
#include "tao/ORB_Core.h"

namespace DAnCE
{
    DAnCE_LocalityActivator_i::DAnCE_LocalityActivator_i (CORBA::ULong def_spawn_delay,
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
      DANCE_TRACE ("DAnCE_LocalityActivator_i::DAnCE_LocalityActivator_i");
      ACE_Utils::UUID_GENERATOR::instance ()->init ();

      // initialize the process manager with the ORBs reactor
      // so the exit handlers get triggered when needed
      this->process_manager_.open (ACE_Process_Manager::DEFAULT_SIZE,
                                   this->orb_->orb_core ()->reactor ());
    }

    DAnCE_LocalityActivator_i::~DAnCE_LocalityActivator_i(void)
    {
    }

    void
    DAnCE_LocalityActivator_i::locality_manager_callback (
      ::DAnCE::LocalityManager_ptr serverref,
      const char * server_UUID,
      ::Deployment::Properties_out config)
    {
      DANCE_TRACE("DAnCE_LocalityActivator_i::locaity_manager_callback");

      DANCE_DEBUG (9, (LM_TRACE, DLINFO
                       ACE_TEXT ("DAnCE_LocalityActivator_i::locality_manager_callback - ")
                       ACE_TEXT ("Received callback from LocalityManager %C\n"),
                   server_UUID));

      Server_Info *info = 0;

      {
        ACE_GUARD_THROW_EX ( TAO_SYNCH_MUTEX,
                             guard,
                             this->container_mutex_,
                             CORBA::NO_RESOURCES ());
        for (SERVER_INFOS::iterator i (this->server_infos_.begin ());
             !i.done (); ++i)
          {
            DANCE_DEBUG (9, (LM_TRACE, DLINFO
                             ACE_TEXT ("DAnCE_LocalityActivator_i::locality_manager_callback - ")
                             ACE_TEXT ("Comparing %C with %C\n"),
                             (*i)->uuid_.c_str (), server_UUID));
            if ((*i)->uuid_ == server_UUID)
              {
                info = (*i).get ();
              }
          }
      }

      if (!info)
        {
          DANCE_ERROR (1, (LM_WARNING, DLINFO
                           ACE_TEXT ("DAnCE_LocalityActivator_i::locality_manager_callback - ")
                           ACE_TEXT ("Received callback from LocalityManager %C, which doesn't belong to me.\n"),
                      server_UUID));
          throw ::CORBA::BAD_PARAM ();
        }

      if (info->activated_)
        {
          DANCE_ERROR (1, (LM_ERROR, DLINFO
                           ACE_TEXT ("DAnCE_LocalityActivator_i::locality_manager_callback - ")
                           ACE_TEXT ("Received callback from LocalityManager %C, ")
                           ACE_TEXT ("which has already been configured.\n"),
                       server_UUID));
          throw ::CORBA::BAD_INV_ORDER ();
        }

      if (!CORBA::is_nil (info->ref_))
        {
          DANCE_ERROR (1, (LM_ERROR, DLINFO
                           ACE_TEXT ("DAnCE_LocalityActivator_i::locality_manager_callback - ")
                           ACE_TEXT ("Received callback from LocalityManager %C, ")
                           ACE_TEXT ("which has already called back.\n"),
                       server_UUID));
          throw ::CORBA::BAD_INV_ORDER ();
        }

      DANCE_DEBUG (9, (LM_TRACE, DLINFO
                       ACE_TEXT ("DAnCE_LocalityActivator_i::locality_manager_callback - ")
                       ACE_TEXT ("Received callback from LocalityManager %C\n"),
                   server_UUID));

      info->ref_ = ::DAnCE::LocalityManager::_duplicate (serverref);

      this->create_properties (*info, config);

      // @@TODO: May want to print out configvalues here.
      DANCE_DEBUG (6, (LM_DEBUG, DLINFO
                       ACE_TEXT ("DAnCE_LocalityActivator_i::locality_manager_callback - ")
                       ACE_TEXT ("Generated %u Properties for LocalityManager %C\n"),
                       config->length (), server_UUID));
    }

    void
    DAnCE_LocalityActivator_i::configuration_complete (const char *server_UUID)
    {
      DANCE_TRACE ("DAnCE_LocalityActivator_i::configuration_complete");

      DANCE_DEBUG (9, (LM_TRACE, DLINFO
                       ACE_TEXT ("DAnCE_LocalityActivator_i::configuration_complete - ")
                       ACE_TEXT ("Received configuration_complete from LocalityManager %C\n"),
                       server_UUID));

      try
        {
          Server_Info *info = 0;

          {
            ACE_GUARD_THROW_EX ( TAO_SYNCH_MUTEX,
                                 guard,
                                 this->container_mutex_,
                                 CORBA::NO_RESOURCES ());
            for (SERVER_INFOS::ITERATOR j (this->server_infos_);
                 !j.done (); ++j)
              {
                if ((*j)->uuid_ == server_UUID)
                  {
                    info = (*j).get ();
                  }
              }
          }

          if (!info)
            {
              DANCE_ERROR (1, (LM_WARNING, DLINFO
                               ACE_TEXT ("DAnCE_LocalityActivator_i::configuration_complete - ")
                               ACE_TEXT ("Received configuration_complete from LocalityManager %C, which doesn't belong to me.\n"),
                               server_UUID));
              throw ::CORBA::BAD_PARAM ();
            }

          if (info->activated_)
            {
              DANCE_ERROR (1, (LM_ERROR, DLINFO
                               ACE_TEXT ("DAnCE_LocalityActivator_i::configuration_complete - ")
                               ACE_TEXT ("Received configuration_complete from LocalityManager %C, ")
                               ACE_TEXT ("which has already been completed.\n"),
                           server_UUID));
              throw ::CORBA::BAD_INV_ORDER ();
            }

          if (CORBA::is_nil (info->ref_.in ()))
            {
              DANCE_ERROR (1, (LM_ERROR, DLINFO
                               ACE_TEXT ("DAnCE_LocalityActivator_i::configuration_complete - ")
                               ACE_TEXT ("Received configuration_complete from LocalityManager %C, ")
                               ACE_TEXT ("which has not called back.\n"),
                           server_UUID));
              throw ::CORBA::BAD_INV_ORDER ();
            }

          ACE_GUARD_THROW_EX (TAO_SYNCH_MUTEX,
                              guard,
                              info->mutex_,
                              CORBA::NO_RESOURCES ());
          info->activated_ = true;
          info->condition_.signal ();
        }
      catch (...)
        {
          DANCE_ERROR (1, (LM_ERROR, DLINFO
                           ACE_TEXT ("DAnCE_LocalityActivator_i::configuration_complete - ")
                           ACE_TEXT ("Caught unknown exception while processing ")
                           ACE_TEXT ("configuration_complete\n")));
          throw;
        }
    }

    ::DAnCE::LocalityManager_ptr
    DAnCE_LocalityActivator_i::create_locality_manager (const ::Deployment::Properties & config)
    {
      DANCE_TRACE("DAnCE_LocalityActivator_i::create_locality_manager");

      Safe_Server_Info server (new Server_Info (config.length () + 1));

      DANCE_DEBUG (6, (LM_DEBUG, DLINFO
                       ACE_TEXT ("DAnCE_LocalityActivator_i::create_locality_manager - ")
                       ACE_TEXT ("Received %u config properties\n"),
                       config.length ()));

      DAnCE::Utility::build_property_map (*server->cmap_, config);

      ACE_CString cmd_options = this->construct_command_line (*server);

      DANCE_DEBUG (6, (LM_DEBUG, DLINFO
                       ACE_TEXT ("DAnCE_LocalityActivator_i::create_locality_manager - ")
                       ACE_TEXT ("LocalityManager arguments: %C\n"),
                   cmd_options.c_str ()));

      {
        ACE_GUARD_THROW_EX ( TAO_SYNCH_MUTEX,
                             guard,
                             this->container_mutex_,
                             CORBA::NO_RESOURCES ());
        server_infos_.insert_tail (server);
      }

      DANCE_DEBUG (9, (LM_TRACE, DLINFO
                       ACE_TEXT ("DAnCE_LocalityActivator_i::create_locality_manager - ")
                       ACE_TEXT ("Attempting to spawn LocalityManager with UUID %C\n"),
                       server->uuid_.c_str ()));

      // Now we need to get a copy of the one that was inserted...
      pid_t const pid = this->spawn_locality_manager (new Server_Child_Handler (server),
                                                      cmd_options);

      ACE_Time_Value timeout (this->spawn_delay_);

      CORBA::Any val;

      if (server->cmap_->find (DAnCE::LOCALITY_TIMEOUT, val) == 0)
        {
          CORBA::ULong t;
          if (val >>= t)
            {
              DANCE_DEBUG (6, (LM_DEBUG, DLINFO
                               ACE_TEXT ("DAnCE_LocalityActivator_i::create_locality_manager - ")
                               ACE_TEXT ("Using provided non-default server timeout of %u\n"), t));
              timeout = ACE_Time_Value (t);
            }
          else
            {
              DANCE_ERROR (1, (LM_WARNING, DLINFO
                               ACE_TEXT ("DAnCE_LocalityActivator_i::create_locality_manager - ")
                               ACE_TEXT ("Failed to extract provided non-default server timeout ")
                               ACE_TEXT ("from property '%C', ")
                               ACE_TEXT ("falling back to default timeout of %u\n"),
                               this->spawn_delay_));
            }
        }

      if (this->multithreaded_)
        this->multi_threaded_wait_for_callback (*server, timeout);
      else
        this->single_threaded_wait_for_callback (*server, timeout);

      server->pid_ = pid; // register pid of successfully started lm process

      DANCE_DEBUG (6, (LM_DEBUG, DLINFO
                       ACE_TEXT ("DAnCE_LocalityActivator_i::create_locality_manager - ")
                       ACE_TEXT ("LocalityManager %C successfully spawned and configured!\n"),
                       server->uuid_.c_str ()));

      return ::DAnCE::LocalityManager::_duplicate (server->ref_.in ());
    }

    ACE_CString
    DAnCE_LocalityActivator_i::construct_command_line (Server_Info &server)
    {
      DANCE_TRACE ("DAnCE_LocalityActivator_i::construct_command_line");

      // Build our command line to launch the component server
      ACE_CString cmd_options (this->cs_args_);

      CORBA::Any val;

      if (server.cmap_->find (DAnCE::LOCALITY_UUID, val) == 0)
        {
          // Nodeapplication has requested a custom uuid
          DANCE_DEBUG (9, (LM_TRACE, DLINFO
                           ACE_TEXT ("DAnCE_LocalityActivator_i::construct_command_line - ")
                           ACE_TEXT ("Using provided UUID\n")));
          const char *uuid = 0;
          val >>= uuid;
          server.uuid_ = uuid;
        }
      else
        {
          DANCE_DEBUG (9, (LM_TRACE, DLINFO
                           ACE_TEXT ("DAnCE_LocalityActivator_i::construct_command_line - ")
                           ACE_TEXT ("Using generated UUID\n")));
          ACE_Utils::UUID uuid;
          ACE_Utils::UUID_GENERATOR::instance ()->generate_UUID (uuid);
          server.uuid_ = *uuid.to_string ();
        }

      if (server.cmap_->find (DAnCE::LOCALITY_ARGUMENTS, val) == 0)
        {
          const char *args = 0;
          val >>= args;

          DANCE_DEBUG (9, (LM_TRACE, DLINFO
                           ACE_TEXT ("DAnCE_LocalityActivator_i::construct_command_line - ")
                           ACE_TEXT ("Adding provided server arguments %C\n"), args));

          cmd_options += ' ';
          cmd_options += args;
          cmd_options += ' ';
        }

      DANCE_DEBUG (6, (LM_DEBUG, DLINFO
                       ACE_TEXT ("DAnCE_LocalityActivator_i::construct_command_line - ")
                       ACE_TEXT ("Creating component server")
                       ACE_TEXT (" with UUID %C\n"), server.uuid_.c_str ()));

      cmd_options += " -u ";
      cmd_options += server.uuid_;

      return cmd_options;
    }

    pid_t
    DAnCE_LocalityActivator_i::spawn_locality_manager (Server_Child_Handler* exit_handler,
                                                    const ACE_CString &cmd_line)
    {
      DANCE_TRACE ("DAnCE_LocalityActivator_i::spawn_locality_manager");

      const Server_Info &si = exit_handler->server_info ();

      // Get my object reference
      CORBA::Object_var obj = this->poa_->servant_to_reference (this);
      CORBA::String_var ior = this->orb_->object_to_string (obj.in ());
      CORBA::Any val;

      const char *path = this->cs_path_.c_str ();

      if (si.cmap_->find (SERVER_EXECUTABLE, val) == 0)
        {
          val >>= path;
          DANCE_DEBUG (6, (LM_DEBUG, DLINFO
                           ACE_TEXT ("DAnCE_LocalityActivator_i::spawn_locality_manager - ")
                           ACE_TEXT ("Using provided component server executable: %C\n"),
                           path));
        }
      else
        {
          DANCE_DEBUG (6, (LM_DEBUG, DLINFO
                           ACE_TEXT ("DAnCE_LocalityActivator_i::spawn_locality_manager - ")
                           ACE_TEXT ("Using default component server executable\n")));
        }

      ACE_Process_Options options (true,
        ACE_OS::strlen (path) + ACE_OS::strlen (cmd_line.c_str()) + ACE_OS::strlen (ior.in ()) + 15);
      if (options.command_line ("%s %s -c %s",
                                path,
                                cmd_line.c_str (),
                                ior.in ()) != 0)
        {
          DANCE_ERROR (1, (LM_ERROR, DLINFO
                           ACE_TEXT ("Failed to create commandline\n")));
          throw Deployment::StartError ("locality_manager",
                                        "Failed to create command line for server");
        }

      options.avoid_zombies (0);

      // check for customized PATH addition
      if (si.cmap_->find (DANCE_LM_PATH, val) == 0)
        {
          path = 0;
          val >>= path;
          if (path)
            {
              ACE_CString newpath (ACE_OS::getenv ("PATH"));
              newpath += ACE_TEXT_ALWAYS_CHAR (ACE_LD_SEARCH_PATH_SEPARATOR_STR);
              newpath += path;
              options.setenv (ACE_TEXT("PATH"), ACE_TEXT_CHAR_TO_TCHAR (newpath.c_str ()));

              DANCE_DEBUG (9, (LM_TRACE, DLINFO
                              ACE_TEXT ("DAnCE_LocalityActivator_i::spawn_locality_manager - ")
                              ACE_TEXT ("configured customized PATH environment: %C\n"),
                              newpath.c_str ()));
            }
        }

      // check for customized LD search path addition
      if (si.cmap_->find (DANCE_LM_LIBPATH, val) == 0)
        {
          path = 0;
          val >>= path;
          if (path)
            {
              ACE_CString newpath (ACE_OS::getenv (ACE_TEXT_ALWAYS_CHAR (ACE_LD_SEARCH_PATH)));
              newpath += ACE_TEXT_ALWAYS_CHAR (ACE_LD_SEARCH_PATH_SEPARATOR_STR);
              newpath += path;
              options.setenv (ACE_LD_SEARCH_PATH, ACE_TEXT_CHAR_TO_TCHAR (newpath.c_str ()));

              DANCE_DEBUG (9, (LM_TRACE, DLINFO
                              ACE_TEXT ("DAnCE_LocalityActivator_i::spawn_locality_manager - ")
                              ACE_TEXT ("configured customized %s environment: %C\n"),
                              ACE_LD_SEARCH_PATH,
                              newpath.c_str ()));
            }
        }

      DANCE_DEBUG (9, (LM_TRACE, DLINFO
                       ACE_TEXT ("DAnCE_LocalityActivator_i::spawn_locality_manager - ")
                       ACE_TEXT ("Spawning process, command line is %s\n"),
                   options.command_line_buf ()));

      pid_t const pid = this->process_manager_.spawn (options,
                                                      exit_handler);

      if (pid == ACE_INVALID_PID)
        {
          // clean up as in this case the handler did not get registered
          delete exit_handler;

          DANCE_ERROR (1, (LM_ERROR, DLINFO
                           ACE_TEXT ("Failed to spawn a LocalityManager process\n")));
          throw Deployment::StartError ("locality_manager",
                                        "Failed to spawn process");
        }

      DANCE_DEBUG (9, (LM_TRACE, DLINFO
                       ACE_TEXT ("DAnCE_LocalityActivator_i::spawn_locality_manager - ")
                       ACE_TEXT ("Process successfully spawned with pid %u\n"),
                   pid));
      return pid;
    }

    void
    DAnCE_LocalityActivator_i::
    single_threaded_wait_for_callback (const Server_Info &si,
                                       ACE_Time_Value &timeout)
    {
      DANCE_TRACE ("DAnCE_LocalityActivator_i::single_threaded_wait_for_callback");

      // Below code is broken for thread-per-connection concurrency model,
      // since the main thread is running ORB event loop and will spawn
      // a different thread to handle the call <register_node_application>,
      // the <perform_work> operation will not be invoked and finally
      // a timeout will occur. For a similar reason, it won't work
      // for thread-pool concurrency model.
      while (true)
        {
          if (!si.terminated_ && !si.activated_)
            {
              ACE_GUARD_THROW_EX ( TAO_SYNCH_MUTEX,
                                   guard,
                                   this->mutex_,
                                   CORBA::NO_RESOURCES ());
              // The next guy to acquire the mutex may have already
              // been activated by the previous leader's perform_work,
              // so let's check to make sure that only non-activated
              // folks are hanging on perform_work.
              if (!si.terminated_ && !si.activated_)
                {
                  this->orb_->perform_work (timeout);
                }
            }

          if (si.terminated_)
            {
              DANCE_ERROR (1, (LM_ERROR, DLINFO
                               ACE_TEXT ("DAnCE_LocalityActivator_i::single_threaded_wait_for_callback - ")
                               ACE_TEXT ("Startup failed for LocalityManager %C; process exited before activation.\n"),
                               si.uuid_.c_str ()));
              throw ::Deployment::StartError ("locality_manager",
                                              "Failed to startup LocalityManager");
            }

          if (si.activated_)
            {
              break;
            }

          if (timeout == ACE_Time_Value::zero)
            {
              DANCE_ERROR (1, (LM_ERROR, DLINFO
                               ACE_TEXT ("DAnCE_LocalityActivator_i::single_threaded_wait_for_callback - ")
                               ACE_TEXT ("Timed out while waiting for LocalityManager %C to call back.\n"),
                               si.uuid_.c_str ()));
              throw ::Deployment::StartError ("locality_manager",
                                              "Timed out waiting for LocalityManager");
            }
        }
    }

    void
    DAnCE_LocalityActivator_i::
      multi_threaded_wait_for_callback (Server_Info &si,
                                        ACE_Time_Value &timeout)
    {
      DANCE_TRACE ("DAnCE_LocalityActivator_i::multi_threaded_wait_for_callback");

      // Wait for a conditional variable
      ACE_GUARD_THROW_EX ( TAO_SYNCH_MUTEX,
                           guard,
                           si.mutex_,
                           CORBA::NO_RESOURCES ());

      while (! si.activated_ )
        {
          if (si.condition_.wait (&timeout) == -1)
            {
              DANCE_ERROR (1, (LM_ERROR, DLINFO
                              ACE_TEXT ("DAnCE_LocalityActivator_i::multi_threaded_wait_for_callback - ")
                              ACE_TEXT ("Timed out while waiting for LocalityManager %C to call back.\n"),
                              si.uuid_.c_str ()));
              throw Deployment::StartError ("locality_manager",
                                            "timed out waiting for callback");
            }

          if (si.terminated_)
            {
              DANCE_ERROR (1, (LM_ERROR, DLINFO
                               ACE_TEXT ("DAnCE_LocalityActivator_i::multi_threaded_wait_for_callback - ")
                               ACE_TEXT ("Startup failed for LocalityManager %C; process exited before activation.\n"),
                               si.uuid_.c_str ()));
              throw ::Deployment::StartError ("locality_manager",
                                              "Failed to startup LocalityManager");
            }
        }
    }

    void
    DAnCE_LocalityActivator_i::remove_locality_manager (::DAnCE::LocalityManager_ptr server)
    {
      DANCE_TRACE ("DAnCE_LocalityActivator_i::remove_locality_manager");

      Server_Info *info = 0;

      {
        ACE_GUARD_THROW_EX ( TAO_SYNCH_MUTEX,
                             guard,
                             this->container_mutex_,
                             CORBA::NO_RESOURCES ());
        for (SERVER_INFOS::ITERATOR i (this->server_infos_);
             !i.done (); ++i)
          {
            if ((*i)->ref_->_is_equivalent (server))
              {
                info = (*i).get ();
              }
          }
      }

      if (!info)
        {
          DANCE_ERROR (1, (LM_ERROR, DLINFO
                           ACE_TEXT ("DAnCE_LocalityActivator_i::remove_locality_manager - ")
                           ACE_TEXT ("Failed to find equivalent LocalityManager under my management.\n")));
          throw ::Deployment::StopError ("locality manager",
                                         "unable to stop locality manager");
        }

      try
        {
          DANCE_DEBUG (9, (LM_TRACE, DLINFO
                           ACE_TEXT ("DAnCE_LocalityActivator_i::remove_locality_manager - ")
                           ACE_TEXT ("Calling shutdown () on LocalityManager %C\n"),
                           info->uuid_.c_str ()));
          server->shutdown ();
        }
      catch (::Deployment::StopError &)
        {
          DANCE_ERROR (1, (LM_WARNING, DLINFO
                           ACE_TEXT ("DAnCE_LocalityActivator_i::remove_locality_manager - ")
                           ACE_TEXT ("Received RemoveFailure exception from LocalityManager %C\n"),
                           info->uuid_.c_str ()));
        }

      DANCE_DEBUG (6, (LM_INFO, DLINFO
                       ACE_TEXT ("DAnCE_LocalityActivator_i::remove_locality_manager - ")
                       ACE_TEXT ("LocalityManager %C successfully shut down.\n"),
                       info->uuid_.c_str ()));
    }

    void
    DAnCE_LocalityActivator_i::create_properties (
      const Server_Info &info,
      Deployment::Properties_out config)
    {
      DANCE_DEBUG (6, (LM_DEBUG, DLINFO
                       ACE_TEXT ("DAnCE_LocalityActivator_i::create_properties - ")
                       ACE_TEXT ("Creating property sequence for LocalityManager <%C> with ")
                       ACE_TEXT ("length %u\n"),
                       info.uuid_.c_str (),
                       info.cmap_->current_size ()));

      if (info.cmap_->current_size () == 0) return;

      ACE_NEW_THROW_EX (config,
                        Deployment::Properties (info.cmap_->current_size ()),
                        CORBA::NO_MEMORY ());

      config->length (info.cmap_->current_size ());

      CORBA::ULong pos = 0;
      Utility::PROPERTY_MAP::iterator i = info.cmap_->begin ();

      do {
        config[pos].name = i->ext_id_.c_str ();
        config[pos].value = i->int_id_;
        ++pos;
      } while (i.advance () != 0);
    }

    DAnCE_LocalityActivator_i::Server_Child_Handler::Server_Child_Handler (
        const Safe_Server_Info&  si)
      : server_info_ (si)
      {}

    DAnCE_LocalityActivator_i::Server_Child_Handler::~Server_Child_Handler ()
    {
    }

    int DAnCE_LocalityActivator_i::Server_Child_Handler::handle_close (
        ACE_HANDLE, ACE_Reactor_Mask)
    {
      DANCE_DEBUG (9, (LM_DEBUG, DLINFO
                       ACE_TEXT ("DAnCE_LocalityActivator_i::Server_Child_Handler::handle_close\n")));

      delete this;  // clean us up
      return 0;
    }

    int DAnCE_LocalityActivator_i::Server_Child_Handler::handle_exit (
        ACE_Process *proc)
    {
      DANCE_DEBUG (1, (LM_INFO, DLINFO
                       ACE_TEXT ("DAnCE_LocalityActivator_i::Server_Child_Handler::handle_exit")
                       ACE_TEXT (" - Locality Manager UUID %C, pid=%d: %d\n"),
                       this->server_info_->uuid_.c_str (),
                       int (proc->getpid ()),
                       int (proc->exit_code ()) ));

      // this method is guarenteed to be called synchronously
      // so we can safely call anything we like

      // flag this process as exited
      this->server_info_->terminated_ = true;
      // signal possibly waiting startup thread
      this->server_info_->condition_.signal ();

      return 0;
    }

}


