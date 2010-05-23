// $Id$

#include "LocalityActivator_Impl.h"
#include "ace/Log_Msg.h"
#include "ace/UUID.h"
#include "DAnCE/Logger/Log_Macros.h"
#include "DAnCE/DAnCE_PropertiesC.h"

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
			   ACE_TEXT ("Received callback from LocalityManager %C, which has already been configured.\n"),
                       server_UUID));
          throw ::CORBA::BAD_INV_ORDER ();
        }

      if (!CORBA::is_nil (info->ref_))
        {
          DANCE_ERROR (1, (LM_ERROR, DLINFO
			   ACE_TEXT ("DAnCE_LocalityActivator_i::locality_manager_callback - ")
			   ACE_TEXT ("Received callback from LocalityManager %C, which has already called back.\n"),
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

          info->activated_ = true;
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

      server_infos_.insert_tail (server);

      DANCE_DEBUG (9, (LM_TRACE, DLINFO
		       ACE_TEXT ("DAnCE_LocalityActivator_i::create_locality_manager - ")
		       ACE_TEXT ("Attempting to spawn LocalityManager with UUID %C\n"),
		       server->uuid_.c_str ()));

      // Now we need to get a copy of the one that was inserted...
      pid_t const pid = this->spawn_locality_manager (*server, cmd_options);
      ACE_UNUSED_ARG (pid);

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
        this->multi_threaded_wait_for_callback (*server, timeout/*, pid*/);
      else
        this->single_threaded_wait_for_callback (*server, timeout/*, pid*/);

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
    DAnCE_LocalityActivator_i::spawn_locality_manager (const Server_Info &si,
                                                    const ACE_CString &cmd_line)
    {
      DANCE_TRACE ("DAnCE_LocalityActivator_i::spawn_locality_manager");

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

      DANCE_DEBUG (9, (LM_TRACE, DLINFO
		       ACE_TEXT ("DAnCE_LocalityActivator_i::spawn_locality_manager - ")
		       ACE_TEXT ("Spawning process, command line is %s\n"),
                   options.command_line_buf ()));

      pid_t const pid = this->process_manager_.spawn (options,
                                                      &this->child_handler_);

      if (pid == ACE_INVALID_PID)
        {
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
          this->orb_->perform_work (timeout);

          if (timeout == ACE_Time_Value::zero)
            {
              DANCE_ERROR (1, (LM_ERROR, DLINFO
			       ACE_TEXT ("DAnCE_LocalityActivator_i::single_threaded_wait_for_callback - ")
			       ACE_TEXT ("Timed out while waiting for LocalityManager %C to call back.\n"),
			       si.uuid_.c_str ()));
              throw ::Deployment::StartError ("locality_manager",
					      "Timed out waiting for LocalityManager");
            }

          if (si.activated_)
            {
              break;
            }
        }
    }

    void
    DAnCE_LocalityActivator_i::
    multi_threaded_wait_for_callback (const Server_Info &si,
                                      ACE_Time_Value &timeout)
    {
      DANCE_TRACE ("DAnCE_LocalityActivator_i::multi_threaded_wait_for_callback");

      // Wait for a conditional variable
      ACE_GUARD_THROW_EX ( TAO_SYNCH_MUTEX,
                           guard,
                           this->mutex_,
                           CORBA::NO_RESOURCES ());

      while (! si.activated_ )
        if (this->condition_.wait (&timeout) == -1)
          {
            DANCE_ERROR (1, (LM_ERROR, DLINFO
			     ACE_TEXT ("DAnCE_LocalityActivator_i::multi_threaded_wait_for_callback - ")
			     ACE_TEXT ("Timed out while waiting for LocalityManager %C to call back.\n"),
			     si.uuid_.c_str ()));
            throw Deployment::StartError ("locality_manager",
					  "timed out waiting for callback");
          }
    }
  
    void
    DAnCE_LocalityActivator_i::remove_locality_manager (::DAnCE::LocalityManager_ptr server)
    {
      DANCE_TRACE ("DAnCE_LocalityActivator_i::remove_locality_manager");

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
}


