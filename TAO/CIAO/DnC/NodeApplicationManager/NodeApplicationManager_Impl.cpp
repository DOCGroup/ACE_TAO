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

CIAO::NodeApplicationManager_Impl *
CIAO::NodeApplicationManager_Impl::
init (const char *nodeapp_location,
      CORBA::ULong delay
      ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
		   Deployment::InvalidProperty))
{
  if (nodeapp_location == 0)
    ACE_THROW_RETURN (CORBA::BAD_PARAM (), 0);

  if (spawn_delay_ == 0)
    ACE_THROW_RETURN (CORBA::BAD_PARAM (), 0);

  this->nodeapp_path_.set (nodeapp_location);
  this->spawn_delay_ = delay;

  PortableServer::POAManager_var mgr
    = this->poa_->the_POAManager (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  CORBA::PolicyList policies (0);

  // Create a separate POA for callback objects.
  this->callback_poa_ =
    this->poa_->create_POA ("callback_poa",
                            mgr.in (),
                            policies
                            ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  // Activate the ourself.
  PortableServer::ObjectId_var oid
    = this->poa_->activate_object (this
                                   ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  CORBA::Object_var obj = this->poa_->id_to_reference (oid.in ()
                                                       ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  // And cache the object reference.
  this->objref_ = Deployment::NodeApplicationManager::_narrow (obj.in ()
							       ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  // Note: even I created the object here but I don't hold the object ref.
  // Here I return pointer of myself so the NodeManager could do
  // servant_to_reference to get the reference since we are in the same POA.
  return this;
}

void
CIAO::NodeApplicationManager_Impl::
parse_config_value (ACE_CString & str
		    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
		   Deployment::InvalidProperty))
{
  ACE_UNUSED_ARG (str);
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
							 this->objref_,
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
create_connections (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
  ACE_THROW_SPEC ((CORBA::SystemException,
		   Deployment::ResourceNotAvailable,
		   Deployment::StartError,
		   Deployment::InvalidProperty))
{
  Deployment::Connections_var retv;
  CORBA::ULong len = retv->length ();

  ACE_NEW_THROW_EX (retv.out (),
                    Deployment::Connections (),
                    CORBA::INTERNAL ());
  ACE_CHECK_RETURN (0);

  Component_Iterator iter (this->component_map_.begin ());

  for (;
       iter != this->component_map_.end ();
       ++iter)
  {
    // Get all the facets first
    Components::FacetDescriptions_var facets =
      ((*iter).int_id_)->get_all_facets (ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_CHECK;

    for (CORBA::ULong i = 0; i < facets->length (); ++i)
    {
      // Note: Its a bit strange to me that in the valuetype a string access
      // method returns a const char * while an Object returns a Object_ptr
      // without a const. They are all public member, so?
      retv->length (len+1);
      retv[len].instanceName = (*iter).ext_id_.c_str ();
      retv[len].portName = facets[i]->name ();
      retv[len].kind = Deployment::Facet;
      retv[len].endpoint = CORBA::Object::_duplicate (facets[i]->facet_ref ());
      ++len;
    }

    // Get all the event consumers
    Components::ConsumerDescriptions_var consumers =
      ((*iter).int_id_)->get_all_consumers (ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_CHECK;

    for (CORBA::ULong i = 0; i < consumers->length (); ++i)
    {
      retv->length (len+1);
      retv[len].instanceName = (*iter).ext_id_.c_str ();
      retv[len].portName = consumers[i]->name ();
      retv[len].kind = Deployment::EventConsumer;
      retv[len].endpoint = CORBA::Object::_duplicate (consumers[i]->consumer ());
      ++len;
    }

  }
  return retv._retn ();
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
  ACE_UNUSED_ARG (configProperty);
  /**
   *  1. First Map properties to TAO/CIAO specific property/configurations
   *  2. Necessary property checking (needed?)
   *  3. Call create_nade_application to spawn new process.
   *  4. Initialize the NodeApplication.
   *  5. get the provided connection endpoints back and return them.
   */


  providedReference = this->create_connections (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  if (start) this->nodeapp_->start (ACE_ENV_SINGLE_ARG_PARAMETER);
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
  ACE_UNUSED_ARG (app);

  ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->lock_);
  //@@ Since we know there is only 1 nodeapp so the passed in
  //   parameter could be ignored.

  if (CORBA::is_nil (this->nodeapp_.in () ))
    ACE_THROW (Deployment::StopError ());

  this->nodeapp_->remove (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  //@@ I might have to deal with the leftover in the comoponentmap here.
}
