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
  //@@ Call CCM_activate ....
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

  ACE_CHECK_RETURN (0);

  // Bind the home in the map.
  // Note: The bind will duplicate the ObjectRef.
  if (this->home_map_.bind (impl_info.component_instance_name.in (),
			    Components::CCMHome::_duplicate (newhome.in ())));
      ACE_THROW_RETURN (Deployment::InstallationFailure (), 0);

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
  // Of course we can call remove on the home to remove the component
  // but this is easier.
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
  PortableServer::ObjectId_var oid;

  // Release all component servant object.
  for (;
       iter != this->component_map_.end ();
       ++iter)
  {
    this->container_->uninstall_component ((*iter).int_id_,
					   oid.out ());
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

  ACE_CString str (comp_ins_name);
  PortableServer::ObjectId_var oid;

  if (this->component_map_.find (str, comp) != 0)
    ACE_THROW (CORBA::BAD_PARAM ());

  this->container_->uninstall_component (comp, oid.out ());
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
