// $Id$

#include "NodeApplicationManager_Impl.h"
#include "ace/Process.h"
#include "ace/OS_NS_stdio.h"

#if !defined (__ACE_INLINE__)
# include "NodeApplicationManager_Impl.inl"
#endif /* __ACE_INLINE__ */

CIAO::NodeApplicationManager_Impl::~NodeApplicationManager_Impl ()
{
}

Deployment::NodeApplicationManager_ptr
CIAO::NodeApplicationManager_Impl::
init (const char *nodeapp_location,
      CORBA::ULong delay,
      const Deployment::DeploymentPlan & plan
      ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
		   Deployment::InvalidProperty))
{
  ACE_UNUSED_ARG (delay); // @@ We need to use this argumetn
                                // later to determine is a
                                // NodeApplication has been spawned
                                // successfully.

  if (nodeapp_location == 0)
    ACE_THROW_RETURN (CORBA::BAD_PARAM (), CORBA::Object::_nil ());

  if (spawn_delay_ == 0)
    ACE_THROW_RETURN (CORBA::BAD_PARAM (), CORBA::Object::_nil ());

  this->nodeapp_path_.set (nodeapp_location);
  this->spawn_delay_ = delay;

  PortableServer::POAManager_var mgr
    = this->poa_->the_POAManager (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::Object::_nil ());

  CORBA::PolicyList policies (0);

  // Create a separate POA for callback objects.
  this->callback_poa_ =
    this->poa_->create_POA ("callback_poa",
                            mgr.in (),
                            policies
                            ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::Object::_nil ());

  // Activate the ourself.
  PortableServer::ObjectId_var oid
    = this->poa_->activate_object (this
                                   ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::Object::_nil ());

  CORBA::Object_var obj = this->poa_->id_to_reference (oid.in ()
                                                       ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::Object::_nil ());

  // And cache the object reference.
  this->objref_ = Deployment::NodeApplicationManager::_narrow (obj.in ()
							       ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::Object::_nil ());

  return Deployment::NodeApplicationManager::_duplicate (this->objref_.in ());
}

void
CIAO::NodeApplicationManager_Impl::
parse_config_value (ACE_CString & string
		    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
		   Deployment::InvalidProperty))
{
  ACE_DEBUG ((LM_DEBUG, "Not implemnted!\n"));
}


Deployment::NodeApplication_ptr
CIAO::NodeApplicationManager_Impl::
create_node_application (const ACE_CString & options
			 ACE_ENV_ARG_DECL_WITH_DEFAULTS)
  ACE_THROW_SPEC ((CORBA::SystemException,
		   Deployment::StartError,
		   Deployment::InvalidProperty))
{
  ACE_DEBUG ((LM_DEBUG, "CIAO::NodeApplicationManager_Impl::create_node_application\n"));

  Deployment::NodeApplication_var retval;
  Deployment::Properties_var prop;

  ACE_NEW_THROW_EX (prop,
		    Deployment::Properties,
		    CORBA::INTERNAL ());
  ACE_CHECK_RETURN (CORBA::_nil ());

  // @@ Create a new callback servant.
  CIAO::NodeApplication_Callback_Impl * callback_servant = 0;
  ACE_NEW_THROW_EX (callback_servant,
                    CIAO::NodeApplication_Callback_Impl (this->orb_.in (),
							 this->callback_poa_.in (),
							 this->objref_.in (),
							 prop.in ()),
                    CORBA::INTERNAL ());
  ACE_CHECK_RETURN (CORBA::_nil ());

  PortableServer::ServantBase_var servant_var (callback_servant);
  PortableServer::ObjectId_var cb_id
    = this->callback_poa_->activate_object (callback_servant
                                            ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::_nil ());

  ACE_Process node_application;
  ACE_Process_Options p_options;

  ACE_TRY
    {
      CORBA::Object_var cb_obj =
        this->callback_poa_->id_to_reference (cb_id.in ()
                                              ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CIAO::NodeApplication_Callback_var cb =
        CIAO::NodeApplication_Callback::_narrow (cb_obj.in ()
						 ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::String_var cb_ior =
        this->orb_->object_to_string (cb.in ()
                                      ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // spawn the new NodeApplication.
      p_options.command_line ("%s -k %s"
			      "%s",
			      this->nodeapp_path_.c_str (),
			      cb_ior.in (),
			      options.c_str ());

      p_options.avoid_zombies (1);

      if (node_application.spawn (p_options) == -1)
        {
          ACE_DEBUG ((LM_ERROR, "Fail to spawn a NodeApplication process\n"));
          ACE_TRY_THROW (Components::CreateFailure ());
        }

      // wait for nodeApp to pass back its object reference. with a timeout value.
      // using perform_work and stuff.

      int looping = 1;

      ACE_Time_Value timeout (this->spawn_delay_, 0);

      while (looping)
        {
          this->orb_->perform_work (timeout
                                    ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          retval = callback_servant->get_nodeapp_ref ();

          if (timeout == ACE_Time_Value::zero || !CORBA::is_nil (retval.in ()))
            looping = 0;
        }

      if (CORBA::is_nil (retval.in ()))
        {
          ACE_DEBUG ((LM_ERROR, "Fail to acquire the NodeApplication object\n"));
          ACE_TRY_THROW (Components::CreateFailure ());
        }

      {
        ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, ace_mon, this->lock_, 0);

        this->nodeapp_ = retval.in ();
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

Deployment::Connections *
CIAO::NodeApplicationManager_Impl::
create_connections (void)
  ACE_THROW_SPEC ((CORBA::SystemException,
		   Deployment::ResourceNotAvailable,
		   Deployment::StartError,
		   Deployment::InvalidProperty))
{
  return (0);
}

Deployment::Application_ptr
CIAO::NodeApplicationManager_Impl::
startLaunch (const Deployment::Properties & configProperty,
	     Deployment::Connections_out providedReference,
	     CORBA::Boolean start
	     ACE_ENV_ARG_DECL_WITH_DEFAULTS)
  ACE_THROW_SPEC ((CORBA::SystemException,
		   Deployment::ResourceNotAvailable,
		   Deployment::StartError,
		   Deployment::InvalidProperty))
{
  /**
   *  1. First Map properties to TAO/CIAO specific property/configurations
   *  2. Necessary property checking (needed?)
   *  3. Call create_nade_application to spawn new process.
   *  4. Initialize the NodeApplication.
   *  5. get the provided connection endpoints back and return them.
   */
  ACE_TRY
    {

    }
  ACE_CATCHANY
    {
    }
  ACE_ENDTRY;

  ACE_CHECK_RETURN (0);

  return this->nodeapp_.in ();
}


void
CIAO::NodeApplicationManager_Impl::
destroyApplication (Deployment::Application_ptr app
		    ACE_ENV_ARG_DECL_WITH_DEFAULTS)
  ACE_THROW_SPEC ((CORBA::SystemException
		   , Deployment::StopError))
{
  ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->lock_);
  //@@ Since we know there is only 1 nodeapp so the passed in
  //   parameter could be ignored.

  if (CORBA::is_nil (this->nodeapp_.in () ))
    ACE_THROW (Deployment::StopError ());

  this->nodeapp_->remove (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
}
