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
      ACE_THROW (CORBA::BAD_PARAM ());

    if (spawn_delay_ == 0)
      ACE_THROW (CORBA::BAD_PARAM ());

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

    CORBA::Object_var obj = this->poa_->id_to_reference (oid.in ()
                                                         ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;

    // And cache the object reference.
    this->objref_ = Deployment::NodeApplicationManager::_narrow (obj.in ()
                                                                 ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;

    //return this object reference
    return Deployment::NodeApplicationManager::_duplicate (this->objref_.in ());

  }
  ACE_CATCHANY
  {
    ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                         "NodeApplicationManager_Impl::init\t\n");
    ACE_RE_THROW;
  }

  ACE_ENDTRY;
}

void
CIAO::NodeApplicationManager_Impl::
parse_config_value (ACE_CString & str
                    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Deployment::InvalidProperty))
{
  // @@ (OO) An unused argument in C++ is marked by removing the
  //         parameter name.  We only use ACE_UNUSED_ARG() when there
  //         are multiple preprocessor-time cases (e.g. different
  //         platform-specific implementations, and at least one of
  //         them uses the argument, while some others do not.  In
  //         this case, please remove the below ACE_UNUSED_ARG, and
  //         comment out the "str" parameter name above, e.g.:
  //
  //             ACE_CString & /* str */

  ACE_UNUSED_ARG (str);

  // @@ (OO) Would it be appropriate to throw CORBA::NO_IMPLEMENT()
  //         here?
  //
  // @@ (OO) Production code should not display debugging output
  //         unless requested by the user.  You should probably make
  //         the following output dependent on CIAO's debugging flag.
  ACE_DEBUG ((LM_DEBUG, "Not implemented!\n"));
}

// @@ (OO) Method definitions should never use "_WITH_DEFAULTS"
//         versions of emulated exception parameters.  Please remove
//         the "_WITH_DEFAULTS"
Deployment::NodeApplication_ptr
CIAO::NodeApplicationManager_Impl::
create_node_application (const ACE_CString & options
                         ACE_ENV_ARG_DECL_WITH_DEFAULTS)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Deployment::StartError,
                   Deployment::InvalidProperty))
{
  Deployment::NodeApplication_var retval;
  Deployment::Properties_var prop;

  ACE_NEW_THROW_EX (prop,
                    Deployment::Properties,
                    CORBA::INTERNAL ());
  // @@ (OO) You should be returning
  //         Deployment::NodeApplication::_nil() here, not
  //         CORBA::_nil().
  ACE_CHECK_RETURN (CORBA::_nil ());

  // @@ Create a new callback servant.
  CIAO::NodeApplication_Callback_Impl * callback_servant = 0;
  ACE_NEW_THROW_EX (callback_servant,
                    CIAO::NodeApplication_Callback_Impl (this->orb_.in (),
                                                              this->callback_poa_.in (),
                                                              this->objref_.in (),
                                                              prop.in ()),
                    CORBA::INTERNAL ());
  // @@ (OO) You should be returning
  //         Deployment::NodeApplication::_nil() here, not
  //         CORBA::_nil().
  ACE_CHECK_RETURN (CORBA::_nil ());

  PortableServer::ServantBase_var servant_var (callback_servant);
  PortableServer::ObjectId_var cb_id
    = this->callback_poa_->activate_object (callback_servant
                                            ACE_ENV_ARG_PARAMETER);
  // @@ (OO) You should be returning
  //         Deployment::NodeApplication::_nil() here, not
  //         CORBA::_nil().
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
          // @@ (OO) Production code should not display debugging output
          //         unless requested by the user.  You should probably make
          //         the following output dependent on CIAO's debugging flag.
          ACE_DEBUG ((LM_ERROR, "Fail to spawn a NodeApplication process\n"));
          ACE_TRY_THROW (Components::CreateFailure ());
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
          // @@ (OO) Production code should not display debugging output
          //         unless requested by the user.  You should probably make
          //         the following output dependent on CIAO's debugging flag.
          ACE_DEBUG ((LM_ERROR, "Fail to acquire the NodeApplication object\n"));
          ACE_TRY_THROW (Components::CreateFailure ());
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
  // @@ (OO) You should be returning
  //         Deployment::NodeApplication::_nil() here, not zero.
  ACE_CHECK_RETURN (0);

  // @@ (OO) You're missing the ACE_ENV_ARG_PARAMETER emulated
  //         exception argument the below deactivate_object() call.
  this->callback_poa_->deactivate_object (cb_id.in ());
  // @@ (OO) You should be returning
  //         Deployment::NodeApplication::_nil() here, not zero.
  ACE_CHECK_RETURN (0);

  // @@ (OO) Production code should not display debugging output
  //         unless requested by the user.  You should probably make
  //         the following output dependent on CIAO's debugging flag.
  ACE_DEBUG ((LM_DEBUG, "CIAO::NodeApplicationManager_Impl::NodeApplication spawned!\n"));

  return retval._retn ();
}

// @@ (OO) Method definitions should never use "_WITH_DEFAULTS"
//         versions of emulated exception parameters.  Please remove
//         the "_WITH_DEFAULTS"
Deployment::Connections *
CIAO::NodeApplicationManager_Impl::
create_connections (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
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

  // @@ (OO) This variable appears to be unused.  Please remove it.
  //         It doesn't make sense to have it in any case since
  //         nothing has been added to the sequence.
  CORBA::ULong len = retv->length ();

  // @@ (OO) Please rewrite this for statement as follows:
  //
  //   const Component_Iterator end (this->component_map_.end ());
  //   for (Component_Iterator iter (this->component_map_.begin ());
  //        iter != end;
  //        ++iter)
  //
  //         The rationale behind this change is that:
  //          1. The "iter" variable is really local to the loop, so
  //             there is no need to expose it outside the loop.
  //          2. The stop condition portion of the for statement is
  //             executed during each loop iteration.  To improve
  //             performance execute it only once outside the
  //             for-loop, unless of course the end iterator is
  //             invalidated during a loop iteration.  If the end
  //             iterator is invalidated during a loop iteration, do
  //             not move it outside the for-loop statement.
  Component_Iterator iter (this->component_map_.begin ());
  for (;
       iter != this->component_map_.end ();
       ++iter)
  {
    // Get all the facets first
    Components::FacetDescriptions_var facets =
      ((*iter).int_id_)->get_all_facets (ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_CHECK_RETURN (0);

	CORBA::ULong i = 0;
    // @@ (OO) The "continue loop" condition portion of the for
    //         statement is executed during each loop iteration.  To
    //         improve performance execute it only once outside the
    //         for-loop.
    for (i = 0; i < facets->length (); ++i)
    {
      // @@ (OO) Incrementally increasing the length in this manner is
      //         extremely slow, since all elements must be copied
      //         each time the length is increased.  Instead increase
      //         the length once outside of this loop to
      //         facets->length() + consumers->length().
      retv->length (len + 1);

      // @@ (OO) This code can be cleaned up and made more efficient
      //         as follows:
      //
      //   Deployment::Connection & conn = retv[len];
      //   conn.instanceName = (*iter).ext_id_.c_str ();
      //   conn.portName = facets[i]->name ();
      //   conn.kind = Deployment::Facet;
      //   conn.endpoint = CORBA::Object::_duplicate (facets[i]->facet_ref ());

      retv[len].instanceName = (*iter).ext_id_.c_str ();
      retv[len].portName = facets[i]->name ();
      retv[len].kind = Deployment::Facet;
      retv[len].endpoint = CORBA::Object::_duplicate (facets[i]->facet_ref ());

      ++len;
    }

    // Get all the event consumers
    Components::ConsumerDescriptions_var consumers =
      ((*iter).int_id_)->get_all_consumers (ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_CHECK_RETURN (0);

    // @@ (OO) The "continue loop" condition portion of the for
    //         statement is executed during each loop iteration.  To
    //         improve performance execute it only once outside the
    //         for-loop.
    for (i = 0; i < consumers->length (); ++i)
    {
      // @@ (OO) Please see the comments in the previous loop
      //         regarding how to avoid the very slow performance of
      //         incremental growth.
      retv->length (len+1);

      // @@ (OO) This code can be cleaned up and made more efficient
      //         as follows:
      //
      //   Deployment::Connection & conn = retv[len];
      //   conn.instanceName = (*iter).ext_id_.c_str ();
      //   conn.portName = consumers[i]->name ();
      //   conn.kind = Deployment::EventConsumer;
      //   conn.endpoint = CORBA::Object::_duplicate (consumers[i]->consumer ());

      retv[len].instanceName = (*iter).ext_id_.c_str ();
      retv[len].portName = consumers[i]->name ();
      retv[len].kind = Deployment::EventConsumer;
      retv[len].endpoint = CORBA::Object::_duplicate (consumers[i]->consumer ());
      ++len;
    }
  }
  return retv._retn ();
}

// @@ (OO) Method definitions should never use "_WITH_DEFAULTS"
//         versions of emulated exception parameters.  Please remove
//         the "_WITH_DEFAULTS"
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
  // @@ (OO) An unused argument in C++ is marked by removing the
  //         parameter name.  We only use ACE_UNUSED_ARG() when there
  //         are multiple preprocessor-time cases (e.g. different
  //         platform-specific implementations, and at least one of
  //         them uses the argument, while some others do not.  In
  //         this case, please remove the below ACE_UNUSED_ARGs, and
  //         comment out the "configProperty" and "start" parameter
  //         names above, e.g.:
  //
  //             const Deployment::Properties & /* configProperty */,
  //             Deployment::Connections_out providedReference,
  //             CORBA::Boolean /* start */

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
    // @@ (OO) Production code should not display debugging output
    //         unless requested by the user.  You should probably make
    //         the following output dependent on CIAO's debugging flag.
    ACE_DEBUG ((LM_DEBUG, "Failed to create Component Implementation Infos!\n"));
    // @@ (OO) You should be returning Deployment::Application::_nil()
    //         here, not zero..
    ACE_THROW_RETURN (Deployment::StartError (), 0);
  } //@@ I am not sure about which exception to throw. I will come back to this.

  // Now spawn the NodeApplication process.
  ACE_CString cmd_option ("");
  Deployment::NodeApplication_var tmp =
    create_node_application (cmd_option.c_str () ACE_ENV_ARG_PARAMETER);
  // @@ (OO) You should be returning Deployment::Application::_nil()
  //         here, not zero..
  ACE_CHECK_RETURN (0);

  // This is what we will get back, a sequence of compoent object refs.
  Deployment::ComponentInfos_var comp_info;

  // For debugging.
  if (CIAO::debug_level () > 1)
  {
    // @@ (OO) The "continue loop" condition portion of the for
    //         statement is executed during each loop iteration.  To
    //         improve performance execute it only once outside the
    //         for-loop.
    for (CORBA::ULong i = 0; i < infos.length (); ++i)
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
  // @@ (OO) You should be returning Deployment::Application::_nil()
  //         here, not zero..
  ACE_CHECK_RETURN (0);

  // @@ (OO) The "continue loop" condition portion of the for
  //         statement is executed during each loop iteration.  To
  //         improve performance execute it only once outside the
  //         for-loop.
  // Now fill in the map we have for the components.
  for (CORBA::ULong len = 0;
       len < comp_info->length ();
       ++len)
  {
    //Since we know the type ahead of time...narrow is omitted here.
    if (this->component_map_.
        bind (comp_info[len].component_instance_name.in(),
              Components::CCMObject::_duplicate (comp_info[len].component_ref.in())))
      ACE_THROW_RETURN (Deployment::StartError (), 0);
  }

  providedReference = this->create_connections (ACE_ENV_SINGLE_ARG_PARAMETER);
    // @@ (OO) You should be returning Deployment::Application::_nil()
    //         here, not zero..
  ACE_CHECK_RETURN (0);

  if (providedReference == 0)
    // @@ (OO) You should be returning Deployment::Application::_nil()
    //         here, not zero..
    ACE_THROW_RETURN (Deployment::StartError () ,0);

  //if (start) this->nodeapp_->start (ACE_ENV_SINGLE_ARG_PARAMETER);
  // @@ (OO) You should be returning Deployment::Application::_nil()
  //         here, not zero..
  //  ACE_CHECK_RETURN (0);

  return Deployment::NodeApplication::_duplicate (this->nodeapp_.in ());
}


// @@ (OO) Method definitions should never use "_WITH_DEFAULTS"
//         versions of emulated exception parameters.  Please remove
//         the "_WITH_DEFAULTS"
void
CIAO::NodeApplicationManager_Impl::
destroyApplication (Deployment::Application_ptr app
                    ACE_ENV_ARG_DECL_WITH_DEFAULTS)
  ACE_THROW_SPEC ((CORBA::SystemException
                   , Deployment::StopError))
{
  // @@ (OO) An unused argument in C++ is marked by removing the
  //         parameter name.  We only use ACE_UNUSED_ARG() when there
  //         are multiple preprocessor-time cases (e.g. different
  //         platform-specific implementations, and at least one of
  //         them uses the argument, while some others do not.  In
  //         this case, please remove the below ACE_UNUSED_ARG, and
  //         comment out the "app" parameter name above, e.g.:
  //
  //             Deployment::Application_ptr /* app */
  ACE_UNUSED_ARG (app);

  //ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->lock_);
  //@@ Since we know there is only 1 nodeapp so the passed in
  //   parameter is ignored for now.

  if (CORBA::is_nil (this->nodeapp_.in () ))
    ACE_THROW (Deployment::StopError ());

  this->nodeapp_->remove (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
}
