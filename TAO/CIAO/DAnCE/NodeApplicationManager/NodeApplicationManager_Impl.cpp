// $Id$

#include "NodeApplicationManager_Impl.h"
#include "ace/Process.h"
#include "ace/OS_NS_stdio.h"

#if !defined (__ACE_INLINE__)
# include "NodeApplicationManager_Impl.inl"
#endif /* __ACE_INLINE__ */


CIAO::NodeApplicationManager_Impl::~NodeApplicationManager_Impl (void)
{
}

Deployment::NodeApplicationManager_ptr
CIAO::NodeApplicationManager_Impl::init (
    const char *nodeapp_location,
    const CORBA::ULong delay,
    const Deployment::DeploymentPlan & plan,
    const PortableServer::POA_ptr callback_poa
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Deployment::InvalidProperty))
{
  ACE_TRY
    {
      if (nodeapp_location == 0)
        ACE_TRY_THROW (CORBA::BAD_PARAM ());

      if (spawn_delay_ == 0)
        ACE_TRY_THROW (CORBA::BAD_PARAM ());

      this->nodeapp_path_.set (nodeapp_location);
      this->spawn_delay_ = delay;

      // Make a copy of the plan for later usage.
      this->plan_ =  plan;

      // Cache the call back POA for callback object.
      this->callback_poa_ = PortableServer::POA::_duplicate (callback_poa);

      // Activate the ourself.
      PortableServer::ObjectId_var oid
        = this->poa_->activate_object (this
                                       ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::Object_var obj =
        this->poa_->id_to_reference (oid.in ()
                                     ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // And cache the object reference.
      this->objref_ =
        Deployment::NodeApplicationManager::_narrow (obj.in ()
                                                     ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "NodeApplicationManager_Impl::init\t\n");
      ACE_RE_THROW;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (Deployment::NodeApplicationManager::_nil ());

  //return this object reference
  return
    Deployment::NodeApplicationManager::_duplicate (this->objref_.in ());
}

void
CIAO::NodeApplicationManager_Impl::
parse_config_value (ACE_CString & str
                    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Deployment::InvalidProperty))
{
  // The unused arg is for future improvemnts.
  ACE_UNUSED_ARG (str);

  ACE_THROW ( CORBA::NO_IMPLEMENT() );
}

Deployment::NodeApplication_ptr
CIAO::NodeApplicationManager_Impl::
create_node_application (const ACE_CString & options
                         ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Deployment::ResourceNotAvailable,
                   Deployment::StartError,
                   Deployment::InvalidProperty))
{
  Deployment::NodeApplication_var retval;
  Deployment::Properties_var prop;

  ACE_NEW_THROW_EX (prop,
                    Deployment::Properties,
                    CORBA::INTERNAL ());
  ACE_CHECK_RETURN (Deployment::NodeApplication::_nil());

  // @@ Create a new callback servant.
  CIAO::NodeApplication_Callback_Impl * callback_servant = 0;
  ACE_NEW_THROW_EX (callback_servant,
                    CIAO::NodeApplication_Callback_Impl (this->orb_.in (),
                                                    this->callback_poa_.in (),
                                                    this->objref_.in (),
                                                    prop.in ()),
                                                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (Deployment::NodeApplication::_nil());

  PortableServer::ServantBase_var servant_var (callback_servant);
  PortableServer::ObjectId_var cb_id
    = this->callback_poa_->activate_object (callback_servant
                                            ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (Deployment::NodeApplication::_nil());

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
          if (CIAO::debug_level () > 1)
            ACE_DEBUG ((LM_ERROR, "Fail to spawn a NodeApplication process\n"));
          ACE_TRY_THROW (Deployment::ResourceNotAvailable ());
        }

      // wait for nodeApp to pass back its object reference. with a
      // timeout value. using perform_work and stuff.

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
          if (CIAO::debug_level () > 1)
            ACE_DEBUG ((LM_ERROR, "Fail to acquire the NodeApplication object\n"));

          ACE_TRY_THROW (Deployment::ResourceNotAvailable ());
        }

      {
        //ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, ace_mon, this->lock_, 0);
        this->nodeapp_ = Deployment::NodeApplication::_duplicate (retval.in ());
      }
    }
  ACE_CATCHANY
    {
      this->callback_poa_->deactivate_object (cb_id.in ());
      ACE_TRY_CHECK;

      ACE_RE_THROW;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (Deployment::NodeApplication::_nil ());

  this->callback_poa_->deactivate_object (cb_id.in () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (Deployment::NodeApplication::_nil ());

  if (CIAO::debug_level () > 1)
    ACE_DEBUG ((LM_DEBUG, "CIAO::NodeApplicationManager_Impl::NodeApplication spawned!\n"));

  return retval._retn ();
}

Deployment::Connections *
CIAO::NodeApplicationManager_Impl::
create_connections (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Deployment::ResourceNotAvailable,
                   Deployment::StartError,
                   Deployment::InvalidProperty))
{
  Deployment::Connections_var retv;

  ACE_NEW_THROW_EX (retv,
                    Deployment::Connections (),
                    CORBA::INTERNAL ());
  ACE_CHECK_RETURN (0);

  CORBA::ULong len = retv->length ();

  const Component_Iterator end (this->component_map_.end ());
  for (Component_Iterator iter (this->component_map_.begin ());
       iter != end;
       ++iter)
  {
    // Get all the facets first
    Components::FacetDescriptions_var facets =
      ((*iter).int_id_)->get_all_facets (ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_CHECK_RETURN (0);

    // Get all the event consumers
    Components::ConsumerDescriptions_var consumers =
      ((*iter).int_id_)->get_all_consumers (ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_CHECK_RETURN (0);

    const CORBA::ULong facet_len = facets->length ();
    const CORBA::ULong consumer_len = consumers->length ();

    CORBA::ULong curr_len = retv->length ();
    retv->length (curr_len + facet_len + consumer_len);

    CORBA::ULong i = 0;
    for (i = 0; i < facet_len; ++i)
    {
      Deployment::Connection & conn = retv[len];
      conn.instanceName = (*iter).ext_id_.c_str ();
      conn.portName = facets[i]->name ();
      conn.kind = Deployment::Facet;
      conn.endpoint = CORBA::Object::_duplicate (facets[i]->facet_ref ());
      ++len;
    }

    for (i = 0; i < consumer_len; ++i)
    {
      Deployment::Connection & conn = retv[len];
      conn.instanceName = (*iter).ext_id_.c_str ();
      conn.portName = consumers[i]->name ();
      conn.kind = Deployment::EventConsumer;
      conn.endpoint = CORBA::Object::_duplicate (consumers[i]->consumer ());
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
             ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Deployment::ResourceNotAvailable,
                   Deployment::StartError,
                   Deployment::InvalidProperty))
{
  ACE_UNUSED_ARG (configProperty);
  ACE_UNUSED_ARG (start);
  /**
   *  1. First Map properties to TAO/CIAO specific property/configurations
   *  2. Necessary property checking (needed?)
   *  3. Call create_nade_application to spawn new process.
   *  4. Initialize the NodeApplication.
   *  5. get the provided connection endpoints back and return them.
   */

  Deployment::ImplementationInfos infos;

  if (!(infos << (this->plan_)))
  {
    if (CIAO::debug_level () > 1)
      ACE_DEBUG ((LM_DEBUG, "Failed to create Component Implementation Infos!\n"));

    ACE_THROW_RETURN (Deployment::StartError (),
                      Deployment::Application::_nil());
  } //@@ I am not sure about which exception to throw. I will come back to this.

  // Now spawn the NodeApplication process.
  ACE_CString cmd_option ("");
  Deployment::NodeApplication_var tmp =
    create_node_application (cmd_option.c_str () ACE_ENV_ARG_PARAMETER);

  ACE_CHECK_RETURN (Deployment::Application::_nil());

  // This is what we will get back, a sequence of compoent object refs.
  Deployment::ComponentInfos_var comp_info;

  // For debugging.
  if (true) //(CIAO::debug_level () > 1)
  {
    const CORBA::ULong info_len = infos.length ();
    for (CORBA::ULong i = 0; i < info_len; ++i)
    {
      ACE_DEBUG ((LM_DEBUG, "The info for installation: \n\t%s\n\t%s\n\t%s\n\t%s\n\t%s\n",
                  infos[i].component_instance_name.in (),
                  infos[i].executor_dll.in (),
                  infos[i].executor_entrypt.in (),
                  infos[i].servant_dll.in (),
                  infos[i].servant_entrypt.in () ));
    }
  }

  // This will install all homes and components.
  comp_info = this->nodeapp_->install (infos ACE_ENV_ARG_PARAMETER);

  ACE_CHECK_RETURN (Deployment::Application::_nil());

  // Now fill in the map we have for the components.
  const CORBA::ULong comp_len = comp_info->length ();
  for (CORBA::ULong len = 0;
       len < comp_len;
       ++len)
  {
    //Since we know the type ahead of time...narrow is omitted here.
    if (this->component_map_.
        bind (comp_info[len].component_instance_name.in(),
              Components::CCMObject::_duplicate (comp_info[len].component_ref.in())))
      ACE_THROW_RETURN (Deployment::StartError (), 0);
  }

  providedReference = this->create_connections (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (Deployment::Application::_nil());

  if (providedReference == 0)
    ACE_THROW_RETURN (Deployment::StartError () ,
                      Deployment::Application::_nil());

  return Deployment::NodeApplication::_duplicate (this->nodeapp_.in ());
}


void
CIAO::NodeApplicationManager_Impl::
destroyApplication (Deployment::Application_ptr app
                    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException
                   , Deployment::StopError))
{
  ACE_UNUSED_ARG (app);

  //ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->lock_);
  //@@ Since we know there is only 1 nodeapp so the passed in
  //   parameter is ignored for now.
  if (CORBA::is_nil (this->nodeapp_.in () ))
    ACE_THROW (Deployment::StopError ());

  this->nodeapp_->remove (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
}
