//$Id$

#include "NodeApplication_Impl.h"

#if !defined (__ACE_INLINE__)
# include "NodeApplication_Impl.inl"
#endif /* __ACE_INLINE__ */

CIAO::NodeApplication_Impl::~NodeApplication_Impl ()
{
  // @@ (OO) You've got a possible leak here.  In particular, you're
  //         not calling operator delete() on this->containers_ here.
  //         Don't forget to initialize this->containers_ to zero in
  //         the constructor to make it safe to call delete() on
  //         this->containers_ regardless of whether or not it was
  //         initialized.
}

// @@ (OO) Method definitions should never use "_WITH_DEFAULTS"
//         versions of emulated exception parameters.  Please remove
//         the "_WITH_DEFAULTS"
void
CIAO::NodeApplication_Impl::
finishLaunch (const Deployment::Connections & providedReference,
	      CORBA::Boolean start
	      ACE_ENV_ARG_DECL_WITH_DEFAULTS)
  ACE_THROW_SPEC ((CORBA::SystemException,
		   Deployment::StartError,
		   Deployment::InvalidConnection))
{
  // @@ (OO) Please declare this as a "const CORBA::ULong".  This
  //         improves "const correctness", and improves code self
  //         documentation.
  CORBA::ULong length = providedReference.length ();

  // @@ (OO) Your indentation does not conform to DOC group coding
  //         conventions/guidelines.  Please place two spaces before
  //         the brace, e.g.:
  //
  //            for (...)
  //              {
  //              }
  //
  //         instead of:
  //
  //            for (...)
  //            {
  //            }
  //
  //         and similar for "if/else" blocks.

  // For every connection struct we finish the connection.
  for (CORBA::ULong i = 0; i < length; ++i)
  {
    ACE_CString name = providedReference[i].instanceName.in ();
    Components::CCMObject_ptr comp;

    if (this->component_map_.find (name, comp) != 0)
      ACE_THROW (Deployment::InvalidConnection ());

    // @@ (OO) What purpose does the following _duplicate() and
    //         assignment to a "_var" serve?  Can the object in
    //         question disappear (e.g. reference count drop to zero)
    //         prior to invoking the connect() operations below?  If
    //         so, the below _duplicate() won't help since you've
    //         still got a race condition immediately after the map
    //         find() operation above, assuming of course the map can
    //         potentially be accessed concurrently by multiple
    //         threads.

    // Since we have done _narrow before
    Components::CCMObject_var save_comp = Components::CCMObject::_duplicate(comp);

    Components::EventConsumerBase_var consumer;
    //Since we know CCMObject inherits from navigation/event/receptacle, no need
    //to narrow here.

    switch (providedReference[i].kind)
    {
    case Deployment::SimplexReceptacle:
      comp->connect (providedReference[i].portName.in (),
		     providedReference[i].endpoint.in ()
		     ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
      break;

    case Deployment::MultiplexReceptacle:
      comp->connect(providedReference[i].portName.in (),
		    providedReference[i].endpoint.in ()
		    ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
      break;

    case Deployment::EventEmitter:
      consumer = Components::EventConsumerBase::
	_narrow (providedReference[i].endpoint.in ()
		 ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
      if (CORBA::is_nil (consumer.in ()))
	ACE_THROW (Deployment::InvalidConnection ());

      comp->connect_consumer(providedReference[i].portName.in (),
			     consumer.in ()
			     ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
      break;

    case Deployment::EventPublisher:
      consumer = Components::EventConsumerBase::
	_narrow (providedReference[i].endpoint.in ()
		 ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
      if (CORBA::is_nil (consumer.in ()))
	ACE_THROW (Deployment::InvalidConnection ());

      comp->subscribe (providedReference[i].portName.in (),
		       consumer.in ()
		       ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
      break;

    default:
      ACE_THROW (Deployment::InvalidConnection ());
    }
  }
  if (start)
  {
    // @@ (OO) Shouldn't you only print this message if some CIAO
    //         debug level is enabled.
    ACE_DEBUG ((LM_DEBUG, "The Start value is true in FinishLaunch Call!\n"));

    // @@ (OO) You're missing the emulated exception argument to the
    //         start() call.  It should be:
    //
    //            this->start (ACE_ENV_SINGLE_ARG_PARAMETER);
    this->start ();
    ACE_CHECK;
  }
}

// @@ (OO) Method definitions should never use "_WITH_DEFAULTS"
//         versions of emulated exception parameters.  Please remove
//         the "_WITH_DEFAULTS"
void
CIAO::NodeApplication_Impl::
start (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
  ACE_THROW_SPEC ((CORBA::SystemException,
		   Deployment::StartError))
{
  //@@ Note: set_session_context will be called when the servant is created.
  Funct_Ptr functor = & Components::CCMObject::ciao_preactivate;
  start_i (functor ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  functor = & Components::CCMObject::ciao_activate;
  start_i (functor ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  functor = & Components::CCMObject::ciao_postactivate;
  start_i (functor ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

// @@ (OO) Method definitions should never use "_WITH_DEFAULTS"
//         versions of emulated exception parameters.  Please remove
//         the "_WITH_DEFAULTS"
void
CIAO::NodeApplication_Impl::
start_i (Funct_Ptr functor
	 ACE_ENV_ARG_DECL_WITH_DEFAULTS)
  ACE_THROW_SPEC ((CORBA::SystemException,
		   Deployment::StartError))
{
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
    //@@ I don't know what if Components::InvalidConfiguration
    //   is thrown from here, so it's ignored for now.  --Tao
    (((*iter).int_id_)->*functor) (ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_CHECK;
  }
}

// @@ (OO) Method definitions should never use "_WITH_DEFAULTS"
//         versions of emulated exception parameters.  Please remove
//         the "_WITH_DEFAULTS"
::Deployment::Properties *
CIAO::NodeApplication_Impl::
properties (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  Deployment::Properties * tmp;

  // @@ (OO) This code is unnecessarily complex.  Simply passing
  //         this->properties_ to the Deployment::Properties() copy
  //         constructor achieves the same effect with less lines of
  //         code.  Please rewrite this implementation as follows:
  //
  //   ACE_NEW_THROW_EX (tmp,
  //                     Deployment::Properties (this->properties_),
  //                     CORBA::INTERNAL ());
  //   ACE_CHECK_RETURN (0);
  //
  //   return tmp;

  ACE_NEW_THROW_EX (tmp,
                    Deployment::Properties (),
                    CORBA::INTERNAL ());
  ACE_CHECK_RETURN (0);

  Deployment::Properties_var retval (tmp);

  retval.inout() = this->properties_;

  return retval._retn ();
}

// @@ (OO) Method definitions should never use "_WITH_DEFAULTS"
//         versions of emulated exception parameters.  Please remove
//         the "_WITH_DEFAULTS"
::Deployment::ComponentInfos *
CIAO::NodeApplication_Impl::
install (const ::Deployment::ImplementationInfos & impl_infos
	 ACE_ENV_ARG_DECL_WITH_DEFAULTS)
  ACE_THROW_SPEC ((CORBA::SystemException,
		   Deployment::UnknownImplId,
		   Deployment::ImplEntryPointNotFound,
		   Deployment::InstallationFailure,
		   Components::InvalidConfiguration))
{
  Deployment::ComponentInfos * tmp;
  ACE_NEW_THROW_EX (tmp,
		    Deployment::ComponentInfos,
		    CORBA::INTERNAL ());
  ACE_CHECK_RETURN (0);
  Deployment::ComponentInfos_var retv (tmp);

  // @@ (OO) Please declare this as "const CORBA::ULong len".  Minor
  //         "const correctness" change.
  CORBA::ULong len = impl_infos.length ();

  retv->length (len);

  // @@ (OO) There is no need to declare these variables outside of
  //         the loop.  Some folks doing so is an optimization but
  //         doing so generally defeats some compiler optimizations.
  //         Please move these declaration within the loop.
  Components::CCMHome_var home;
  Components::CCMObject_var comp;

  for (CORBA::ULong i = 0; i < len; ++i)
  {
    home = this->install_home (impl_infos[i]
			       ACE_ENV_ARG_PARAMETER);
    ACE_CHECK;

    Components::KeylessCCMHome_var kh =
      Components::KeylessCCMHome::_narrow (home.in ()
					   ACE_ENV_ARG_PARAMETER);
    ACE_CHECK;

    if (CORBA::is_nil (kh.in ()))
	ACE_THROW_RETURN (Deployment::InstallationFailure (), 0);

    // @@ Note, here we are missing the CreateFailure.
    // Sometime I will come back to add exception rethrow.
    comp = kh->create_component (ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_CHECK;

    if (this->component_map_.bind (impl_infos[i].component_instance_name.in (),
				   Components::CCMObject::_duplicate (comp.in ())))
      ACE_THROW_RETURN (Deployment::InstallationFailure (), 0);

    // Set the return value.
    (*retv)[i].component_instance_name
      = impl_infos[i].component_instance_name.in ();

    (*retv)[i].component_ref = Components::CCMObject::_duplicate (comp.in ());

    // Deal with Component instance related Properties.
    // Now I am only concerning about the COMPOENTIOR and here is only
    // the hardcoded version of the configuration. Hopefully we will
    // reach an agreement after the RTWS about how the configuration
    // should be done.

    // @@ (OO) Please move the length() call below out of the loop.
    //         It will be unnecessarily executed on each loop
    //         iteration.  For example, do the following:
    //
    //     const CORBA::ULong clen = impl_infos[i].component_config.length ();
    //     for (CORBA::ULong prop_len = 0; prop_len < clen; ++prop_len)

    for (CORBA::ULong prop_len = 0;
	 prop_len < impl_infos[i].component_config.length ();
	 ++prop_len)
    {
      if (ACE_OS::strcmp (impl_infos[i].component_config[prop_len].name.in (),
			  "ComponentIOR") == 0)
      {
        // @@ (OO) Shouldn't you only print output if CIAO debugging
        //         is enabled?
	ACE_DEBUG ((LM_DEBUG, "Found property to write the IOR.\n"));
	const char * path;
	impl_infos[i].component_config[prop_len].value >>= path;

	CORBA::String_var ior =
	  this->orb_->object_to_string (comp.in() ACE_ENV_ARG_PARAMETER);
	ACE_CHECK;

	if (write_IOR (path, ior.in ()) != 0)
	{
        // @@ (OO) Shouldn't you only print output if CIAO debugging
        //         is enabled?
	  ACE_DEBUG ((LM_DEBUG, "Failed to write the IOR.\n"));
	  ACE_THROW (CORBA::INTERNAL ());
	}

      }

    }
  }
  return retv._retn ();
}

// @@ (OO) Method definitions should never use "_WITH_DEFAULTS"
//         versions of emulated exception parameters.  Please remove
//         the "_WITH_DEFAULTS"
::Components::CCMHome_ptr
CIAO::NodeApplication_Impl::
install_home (const ::Deployment::ImplementationInfo & impl_info
	      ACE_ENV_ARG_DECL_WITH_DEFAULTS)
  ACE_THROW_SPEC ((CORBA::SystemException,
		   Deployment::UnknownImplId,
		   Deployment::ImplEntryPointNotFound,
		   Deployment::InstallationFailure,
		   Components::InvalidConfiguration))
{
  Components::CCMHome_var newhome =
    this->container_->ciao_install_home
    (impl_info.executor_dll.in (),
     impl_info.executor_entrypt.in (),
     impl_info.servant_dll.in (),
     impl_info.servant_entrypt.in ()
     ACE_ENV_ARG_PARAMETER);

  ACE_CHECK_RETURN (Components::CCMHome::_nil ());
  // We don't have to do _narrow since the generated code makes sure of
  // the object type for us

  // Bind the home in the map.
  if (this->home_map_.bind (impl_info.component_instance_name.in (),
			    Components::CCMHome::_duplicate (newhome.in ())))
    ACE_THROW_RETURN (Deployment::InstallationFailure (),
		      Components::CCMHome::_nil ());

  //Note: If the return value will be discarded, it must be kept in a var or
  //      release () will have to be called explicitly.
  return newhome._retn ();
}

// @@ (OO) Method definitions should never use "_WITH_DEFAULTS"
//         versions of emulated exception parameters.  Please remove
//         the "_WITH_DEFAULTS"
void
CIAO::NodeApplication_Impl::
remove (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
  ACE_THROW_SPEC ((CORBA::SystemException,
		   Components::RemoveFailure))
{
  //Remove all the components in the NodeApplication/Container
  Home_Iterator iter (this->home_map_.begin ());

  // Remove all components first.
  remove_components ();

  ACE_CHECK;

  // @@ (OO) Same as above.  Please remove "this->home_map_.end()" out
  //         of the loop.

  // Even if above op failed we should still remove homes.
  for (;
       iter != this->home_map_.end ();
       ++iter)
  {
    this->container_->ciao_uninstall_home ( (*iter).int_id_
					    ACE_ENV_ARG_PARAMETER);
    ACE_CHECK;

    // @@ (OO) Consider making the internal ID of the home_map_ a
    //         CCMHome_var instead of CCMHome_ptr.  That will simplify
    //         memory management, and allow to remove the below
    //         release() call.  The same can be done for the
    //         component_map_.
    CORBA::release ( (*iter).int_id_);
  }

  this->home_map_.unbind_all ();

  // @@ (OO) More debugging output to remove.
  ACE_DEBUG ((LM_DEBUG, "Shutting down this NodeApplication!\n"));
  this->orb_->shutdown (0 ACE_ENV_ARG_PARAMETER);
}

// @@ (OO) Method definitions should never use "_WITH_DEFAULTS"
//         versions of emulated exception parameters.  Please remove
//         the "_WITH_DEFAULTS"
void
CIAO::NodeApplication_Impl::
remove_home (const char * comp_ins_name
	     ACE_ENV_ARG_DECL_WITH_DEFAULTS)
  ACE_THROW_SPEC ((CORBA::SystemException,
		   Components::RemoveFailure))
{
  Components::CCMHome_ptr home;
  ACE_CString str (comp_ins_name);

  if (this->home_map_.find (str, home) != 0)
    ACE_THROW (CORBA::BAD_PARAM ());

  // We should remove all components created by this home as well.
  // This is not implemented yet.

  this->container_->ciao_uninstall_home (home
					 ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // @@ (OO) Consider making the internal ID of the home_map_ a
  //         CCMHome_var instead of CCMHome_ptr.  That will simplify
  //         memory management, and allow to remove the below
  //         release() call.  The same can be done for the
  //         component_map_.

  // If the previous calls failed, what should we do here??
  CORBA::release (home);

  // @@ Still need to remove the home if the previous operation fails?
  if (this->home_map_.unbind (str) == -1)
    ACE_THROW (::Components::RemoveFailure ());
}

// @@ (OO) Method definitions should never use "_WITH_DEFAULTS"
//         versions of emulated exception parameters.  Please remove
//         the "_WITH_DEFAULTS"
::Components::CCMHomes *
CIAO::NodeApplication_Impl::
get_homes (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  Components::CCMHomes * tmp;
  ACE_NEW_THROW_EX (tmp,
                    Components::CCMHomes (),
                    CORBA::INTERNAL ());
  ACE_CHECK_RETURN (0);

  Components::CCMHomes_var retval (tmp);

  // @@ (OO) Please declare len as "const".
  CORBA::ULong len = this->home_map_.current_size ();
  retval->length (len);

  Home_Iterator iter (this->home_map_.begin ());
  CORBA::ULong i;

  // @@ (OO) Please move the end() call out of the loop.  Another
  //         option is to rewrite the loop in slightly cleaner way as
  //         follows:
  //
  //   for (CORBA::ULong i = 0;
  //        i < len;
  //        ++i, ++iter)
  //     {
  //       retval[i] = Components::CCMHome::_duplicate ( (*iter).int_id_);
  //     }
  //
  //         and remove the "CORBA::ULong i" declaration outside of
  //         the loop.
  for (i = 0;
       iter != this->home_map_.end ();
       ++iter, ++i)
  {
    retval[i] = Components::CCMHome::_duplicate ( (*iter).int_id_);
  }

  return retval._retn ();
}

// @@ (OO) Method definitions should never use "_WITH_DEFAULTS"
//         versions of emulated exception parameters.  Please remove
//         the "_WITH_DEFAULTS"
CORBA::Long
CIAO::NodeApplication_Impl::
init (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_NEW_THROW_EX (this->container_,
                    CIAO::Session_Container (this->orb_.in (),
                                             0,
                                             0),
                    CORBA::INTERNAL ());
  ACE_CHECK_RETURN (-1);

  return this->container_->init (0,
				 0
				 ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);
  return 0;
}

// @@ (OO) Method definitions should never use "_WITH_DEFAULTS"
//         versions of emulated exception parameters.  Please remove
//         the "_WITH_DEFAULTS"
::CORBA::Object_ptr
CIAO::NodeApplication_Impl::
get_node_application_manager (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return ::CORBA::Object::_duplicate (this->node_app_manager_.in ());
}


PortableServer::POA_ptr
CIAO::NodeApplication_Impl::
_default_POA (void)
{
  return PortableServer::POA::_duplicate (this->poa_.in ());
}

// @@ (OO) Method definitions should never use "_WITH_DEFAULTS"
//         versions of emulated exception parameters.  Please remove
//         the "_WITH_DEFAULTS"
void
CIAO::NodeApplication_Impl::
remove_components (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
  ACE_THROW_SPEC ((CORBA::SystemException,
		   Components::RemoveFailure))
{
  //Remove all the components in the NodeApplication/Container
  Component_Iterator iter (this->component_map_.begin ());

  // Release all component servant object.
  for (;
       iter != this->component_map_.end ();
       ++iter)
  {
    Components::CCMHome_ptr home;
    if (this->home_map_.find ( (*iter).ext_id_, home) != 0)
      ACE_THROW (CORBA::BAD_PARAM ());

    // This will call ccm_passivate on the component executor.
    home->remove_component ((*iter).int_id_);
    ACE_CHECK;

    // @@ (OO) Consider making the internal ID of the home_map_ a
    //         CCMHome_var instead of CCMHome_ptr.  That will simplify
    //         memory management, and allow to remove the below
    //         release() call.  The same can be done for the
    //         component_map_.
    CORBA::release ( (*iter).int_id_);
  }

  this->component_map_.unbind_all ();
  // To this point the servant should have been destroyed. However,
  // if someone is still making calls on the servant, terrible thing
  // will happen.
}

// @@ (OO) Method definitions should never use "_WITH_DEFAULTS"
//         versions of emulated exception parameters.  Please remove
//         the "_WITH_DEFAULTS"
void
CIAO::NodeApplication_Impl::
remove_component (const char * comp_ins_name
	     ACE_ENV_ARG_DECL_WITH_DEFAULTS)
  ACE_THROW_SPEC ((CORBA::SystemException,
		   Components::RemoveFailure))
{
  Components::CCMObject_ptr comp;
  Components::CCMHome_ptr home;

  ACE_CString str (comp_ins_name);

  /* Before we do remove component we have to inform the homeservant so
   * Component::ccm_passivate ()
   * constainer::ninstall_component () ->deactivate_object () will be called.
   *
   * ccm_remove will be called when the poa destroys the servant.
   */

  if (this->component_map_.find (str, comp) != 0)
    ACE_THROW (CORBA::BAD_PARAM ());

  if (this->home_map_.find (str, home) != 0)
    ACE_THROW (CORBA::BAD_PARAM ());

  // This will call ccm_passivate on the component executor.
  home->remove_component (comp);
  ACE_CHECK;

  // If the previous calls failed, what should we do here??
  CORBA::release (comp);

  // @@ Still need to remove the home if the previous operation fails?
  if (this->component_map_.unbind (str) == -1)
  ACE_THROW (::Components::RemoveFailure ());
}

// @@ (OO) Method definitions should never use "_WITH_DEFAULTS"
//         versions of emulated exception parameters.  Please remove
//         the "_WITH_DEFAULTS"

// The code below is obsolete now. However I want to keep it arround as a
// start point for configurations.
/*
void
CIAO::NodeApplication_Impl::
parse_config_values (const ::Deployment::Properties & properties,
  struct home_installation_info &component_install_info
  ACE_ENV_ARG_DECL_WITH_DEFAULTS)
  ACE_THROW_SPEC ((CORBA::SystemException,
    Deployment::UnknownImplId,
    Deployment::ImplEntryPointNotFound,
    Components::InvalidConfiguration))
{

  for (CORBA::ULong i = 0; i < properties.length(); ++i)
  {
    // Place holder for string values
    const char * str = 0;
    const char * name = properties[i].name.in();

    // I assume the property will be given in the following format! --Tao
    if (ACE_OS::strcmp (name, "CIAO-servant-location"))
    {
      properties[i].value >>= str;
      component_install_info.servant_dll_= str;  //deep copy happens here.
    }
    else if (ACE_OS::strcmp (name, "CIAO-servant-entryPoint"))
    {
      properties[i].value >>= str;
      component_install_info.servant_entrypt_= str;  //deep copy happens here.
    }
    else if (ACE_OS::strcmp (name, "CIAO-executor-location"))
    {
      properties[i].value >>= str;
      component_install_info.executor_dll_= str;  //deep copy happens here.
    }
    else if (ACE_OS::strcmp (name, "CIAO-executor-entryPoint"))
    {
      properties[i].value >>= str;
      component_install_info.executor_entrypt_= str;  //deep copy happens here.
    }
    else
    {
      ACE_DEBUG ((LM_DEBUG, "Found unrecognized property: %s\n",name));
      //I should put the name of the configuration inside of the exception. --Tao
      ACE_THROW (Components::InvalidConfiguration ());
    }
  }

  // We don't need to worry about the case when some info is missing since
  // the modeling tool will ensure the complete info to presented in the properties.
}
*/
