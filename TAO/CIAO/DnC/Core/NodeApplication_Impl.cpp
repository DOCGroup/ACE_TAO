//$Id$

#include "NodeApplication_Impl.h"

#if !defined (__ACE_INLINE__)
# include "NodeApplication_Impl.inl"
#endif /* __ACE_INLINE__ */

CIAO::NodeApplication_Impl::~NodeApplication_Impl ()
{
  //@@ This should clean up all components and homes.

}

void
CIAO::NodeApplication_Impl::
finishLaunch (const Deployment::Connections & providedReference,
	      CORBA::Boolean start
	      ACE_ENV_ARG_DECL_WITH_DEFAULTS)
  ACE_THROW_SPEC ((CORBA::SystemException,
		   Deployment::StartError,
		   Deployment::InvalidConnection))
{
  //@@ not implemented yet.
}

void
CIAO::NodeApplication_Impl::
start (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
  ACE_THROW_SPEC ((CORBA::SystemException,
		   Deployment::StartError))
{
  //@@ Note: set_session_context will be called when the servant is created.

  Funct_Ptr functor = & Components::CCMObject::ciao_preactivate;
  start_i (functor);
  ACE_CHECK;

  functor = & Components::CCMObject::ciao_activate;
  start_i (functor);
  ACE_CHECK;

  functor = & Components::CCMObject::ciao_postactivate;
  start_i (functor);
  ACE_CHECK;
}

void
CIAO::NodeApplication_Impl::
start_i (Funct_Ptr functor
	 ACE_ENV_ARG_DECL_WITH_DEFAULTS)
  ACE_THROW_SPEC ((CORBA::SystemException,
		   Deployment::StartError))
{
  Component_Iterator iter (this->component_map_.begin ());
  for (;
       iter != this->component_map_.end ();
       ++iter)
  {
    //@@ I don't know what if Components::InvalidConfiguration
    //   is thrown from here, so it's ignored for now.  --Tao
    (((*iter).int_id_)->*functor) ();
    ACE_CHECK;
  }
}

::Deployment::Properties *
CIAO::NodeApplication_Impl::
properties (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ::Deployment::Properties *retval;

  ACE_NEW_THROW_EX (retval,
                    Deployment::Properties (),
                    CORBA::INTERNAL ());
  ACE_CHECK_RETURN (0);

  *retval = this->properties_;

  return retval;
}

::Deployment::ComponentInfos *
CIAO::NodeApplication_Impl::
install (const ::Deployment::ImplementationInfos & impl_infos
	 ACE_ENV_ARG_DECL_WITH_DEFAULTS)
  ACE_THROW_SPEC ((CORBA::SystemException,
		   ::Deployment::UnknownImplId,
		   ::Deployment::ImplEntryPointNotFound,
		   ::Deployment::InstallationFailure,
		   ::Components::InvalidConfiguration))
{
  Deployment::ComponentInfos_var retv;
  ACE_NEW_THROW_EX (retv,
		    Deployment::ComponentInfos,
		    CORBA::INTERNAL ());
  ACE_CHECK_RETURN (0);

  CORBA::ULong len = impl_infos.length ();
  retv->length (len);

  Components::CCMHome_var home;
  Components::CCMObject_var comp;

  for (CORBA::ULong i = 0; i < len; ++i)
  {
    home = install_home (impl_infos [len]);
    ACE_CHECK;

    Components::KeylessCCMHome_var kh =
      Components::KeylessCCMHome::_narrow (home.in ()
					   ACE_ENV_ARG_PARAMETER);
    ACE_CHECK;

    if (CORBA::is_nil (kh.in ()))
	ACE_THROW_RETURN (Deployment::InstallationFailure (), 0);

    // @@ Note, here we are missing the CreateFailure.
    // Sometime I will come back to add exception rethrow.
    comp = kh->create_component ();//_component ();
    ACE_CHECK;

    if (this->component_map_.bind (impl_infos[i].component_instance_name.in (),
				   Components::CCMObject::_duplicate (comp.in ())))
      ACE_THROW_RETURN (Deployment::InstallationFailure (), 0);

    // Set the return value.
    (*retv)[i].component_instance_name
      = impl_infos[i].component_instance_name.in ();

    (*retv)[i].component_ref = Components::CCMObject::_duplicate (comp.in ());
  }
  return retv._retn ();
}

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

  ACE_CHECK_RETURN (CORBA::Object::_nil ());

  // Bind the home in the map.
  if (this->home_map_.bind (impl_info.component_instance_name.in (),
			    Components::CCMHome::_duplicate (newhome.in ())))
    ACE_THROW_RETURN (Deployment::InstallationFailure (),
		      CORBA::Object::_nil ());

  //Note: If the return value will be discarded, it must be kept in a var or
  //      release () will have to be called explicitly.
  return newhome._retn ();
}


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

  // Even if above op failed we should still remove homes.
  for (;
       iter != this->home_map_.end ();
       ++iter)
  {
    this->container_->ciao_uninstall_home ( (*iter).int_id_);
    ACE_CHECK;

    CORBA::release ( (*iter).int_id_);
  }

  this->home_map_.unbind_all ();
}

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

  this->container_->ciao_uninstall_home (home);
  ACE_CHECK;

  // If the previous calls failed, what should we do here??
  CORBA::release (home);

  // @@ Still need to remove the home if the previous operation fails?
  if (this->home_map_.unbind (str) == -1)
    ACE_THROW (::Components::RemoveFailure ());
}

::Components::CCMHomes *
CIAO::NodeApplication_Impl::
get_homes (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  Components::CCMHomes_var retval;

  ACE_NEW_THROW_EX (retval.out (),
                    Components::CCMHomes (),
                    CORBA::INTERNAL ());
  ACE_CHECK_RETURN (0);

  CORBA::ULong len = this->home_map_.current_size ();
  retval->length (len);

  Home_Iterator iter (this->home_map_.begin ());
  CORBA::ULong i;

  for (i = 0;
       iter != this->home_map_.end ();
       ++iter, ++i)
  {
    retval[i] = Components::CCMHome::_duplicate ( (*iter).int_id_);
  }

  return retval._retn ();

}

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
}

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

    CORBA::release ( (*iter).int_id_);
  }

  this->component_map_.unbind_all ();
  // To this point the servant should have been destroyed. However,
  // if someone is still making calls on the servant, terrible thing
  // will happen.
}

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
