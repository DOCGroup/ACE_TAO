// $Id$

#include "ServerActivator_Impl.h"
#include "ace/Process.h"

#if !defined (__ACE_INLINE__)
# include "ServerActivator_Impl.inl"
#endif /* __ACE_INLINE__ */

CIAO::Activator_Callback_Impl::~Activator_Callback_Impl ()
{
}

PortableServer::POA_ptr
CIAO::Activator_Callback_Impl::_default_POA (void)
{
  return PortableServer::POA::_duplicate (this->poa_.in ());
}

::Components::Deployment::ServerActivator_ptr
CIAO::Activator_Callback_Impl::register_component_server (::Components::Deployment::ComponentServer_ptr svr)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->server_ = ::Components::Deployment::ComponentServer::_duplicate (svr);
  return ::Components::Deployment::ServerActivator::_duplicate (this->activator_);
}

::Components::Deployment::ComponentServer_ptr
CIAO::Activator_Callback_Impl::get_server_ref (void)
{
  // Relinquish the ownership of the server reference.  This method
  // should only be called from the ServerActivator.
  return this->server_._retn ();
}

CIAO::ServerActivator_Impl::~ServerActivator_Impl ()
{
  // @@ remove all ComponentServer's?
}

PortableServer::POA_ptr
CIAO::ServerActivator_Impl::_default_POA (void)
{
  return PortableServer::POA::_duplicate (this->poa_.in ());
}

int
CIAO::ServerActivator_Impl::init (const char *server_location,
                                  CORBA::ULong spawn_delay,
                                  const char *installation_ior
                                  ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_UNUSED_ARG (spawn_delay); // @@ We need to use this argumetn
                                // later to determine is a
                                // ComponentServer has been spawned
                                // successfully.

  if (server_location == 0)
    ACE_THROW_RETURN (CORBA::BAD_PARAM (), -1);

  if (spawn_delay_ == 0)
    ACE_THROW_RETURN (CORBA::BAD_PARAM (), -1);

  this->server_path_ = CORBA::string_dup (server_location);

  this->installation_ior_ = CORBA::string_dup (installation_ior);

  PortableServer::POAManager_var mgr
    = this->poa_->the_POAManager (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  CORBA::PolicyList policies (0);

  // Create a separate POA for callback objects.
  this->callback_poa_ =
    this->poa_->create_POA ("callback_poa",
                            mgr.in (),
                            policies
                            ACE_ENV_ARG_PARAMETER);

  ACE_CHECK_RETURN (-1);

  // Activate the ServerActivator Object.
  PortableServer::ObjectId_var oid
    = this->poa_->activate_object (this
                                   ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  CORBA::Object_var obj = this->poa_->id_to_reference (oid.in ()
                                                       ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  // And cache the object reference.
  this->objref_ = ::Components::Deployment::ServerActivator::_narrow (obj.in ()
                                                                      ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  this->ior_ = this->orb_->object_to_string (this->objref_.in ()
                                             ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  return 0;
}

::Components::Deployment::ComponentServer_ptr
CIAO::ServerActivator_Impl::create_component_server (const Components::ConfigValues & config
                                                     ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CreateFailure,
                   Components::InvalidConfiguration))
{
  ACE_UNUSED_ARG (config);      // This will be used later.

  ACE_DEBUG ((LM_DEBUG, "CIAO::ServerActivator_Impl::create_component_server\n"));

  ::Components::Deployment::ComponentServer_var retval;

  // @@ Need to figure out a way to pass the config data over to the
  // ComponentServer.  One option will be to convert it (again) into
  // an XML document and psss it along in the command line.

  // @@ Create a new callback servant.
  CIAO::Activator_Callback_Impl *callback_servant = 0;
  ACE_NEW_THROW_EX (callback_servant,
                    CIAO::Activator_Callback_Impl (this->orb_.in (),
                                                   this->callback_poa_.in (),
                                                   this->objref_.in ()),
                    CORBA::INTERNAL ());
  ACE_CHECK_RETURN (0);

  PortableServer::ServantBase_var servant_var (callback_servant);
  PortableServer::ObjectId_var cb_id
    = this->callback_poa_->activate_object (callback_servant
                                            ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  ACE_Process component_server;
  ACE_Process_Options options;

  ACE_TRY
    {
      CORBA::Object_var cb_obj =
        this->callback_poa_->id_to_reference (cb_id.in ()
                                              ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CIAO::Activator_Callback_var cb =
        CIAO::Activator_Callback::_narrow (cb_obj.in ()
                                           ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::String_var cb_ior =
        this->orb_->object_to_string (cb.in ()
                                      ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // spawn the new ComponentServer.
      options.command_line ("%s -k %s -ORBInitRef ComponentInstallation=%s",
                            this->server_path_.in (),
                            cb_ior.in (),
                            this->installation_ior_.in ());
      options.avoid_zombies (1);

      if (component_server.spawn (options) == -1)
        {
          ACE_DEBUG ((LM_ERROR, "Fail to spawn a ComponentServer process\n"));
          ACE_TRY_THROW (::Components::CreateFailure ());
        }

      // wait for server to pass back its object reference. with a timeout value.
      // using perform_work and stuff.

      int looping = 1;

      ACE_Time_Value timeout (this->spawn_delay_, 0);

      while (looping)
        {
          this->orb_->perform_work (timeout
                                    ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          retval = callback_servant->get_server_ref ();

          if (timeout == ACE_Time_Value::zero || !CORBA::is_nil (retval.in ()))
            looping = 0;
        }

      if (CORBA::is_nil (retval.in ()))
        {
          ACE_DEBUG ((LM_ERROR, "Fail to acquire the ComponentServer object\n"));
          ACE_TRY_THROW (::Components::CreateFailure ());
        }

      {
        ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, ace_mon, this->lock_, 0);

        // Add the server reference to this->cs_set_;
        this->cs_set_.add (retval.in ());
      }
    }
  ACE_CATCHANY
    {
      this->callback_poa_->deactivate_object (cb_id.in ());
      ACE_TRY_CHECK;

      ACE_RE_THROW;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (0);

  this->callback_poa_->deactivate_object (cb_id.in ());
  ACE_CHECK_RETURN (0);

  return retval._retn ();
}

void
CIAO::ServerActivator_Impl::remove_component_server (Components::Deployment::ComponentServer_ptr server
                                                     ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::RemoveFailure))
{
  ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->lock_);

  if (this->cs_set_.object_in_set (server) == 0)
    ACE_THROW (::Components::RemoveFailure ());

  server->remove (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  // Should we remove the server still even if the previous call failed.

  if (this->cs_set_.remove (server) == -1)
    ACE_THROW (::Components::RemoveFailure ());
}

::Components::Deployment::ComponentServers *
CIAO::ServerActivator_Impl::get_component_servers (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, ace_mon, this->lock_, 0);

  Components::Deployment::ComponentServers_var retval;

  ACE_NEW_THROW_EX (retval.out (),
                    Components::Deployment::ComponentServers (),
                    CORBA::INTERNAL ());
  ACE_CHECK_RETURN (0);

  CORBA::ULong len = this->cs_set_.size ();
  retval->length (len);          // resize

#if 0
  // TAO is broken here.  Both <replace>, <get_buffer> and friends are missing.
  this->cs_set_.copy (len, retval->get_buffer (0));
#else
  for (CORBA::ULong i = 0; i < len; ++i)
    {
      retval[i] = this->cs_set_.at (i);
    }
#endif

  return retval._retn ();
}
