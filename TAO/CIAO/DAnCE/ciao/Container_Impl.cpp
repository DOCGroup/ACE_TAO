// $Id$
#include "Container_Impl.h"

#if !defined (__ACE_INLINE__)
# include "Container_Impl.inl"
#endif /* __ACE_INLINE__ */

CIAO::Container_Impl::~Container_Impl ()
{
  // @@ remove all components and home?
  delete this->container_;
}

PortableServer::POA_ptr
CIAO::Container_Impl::_default_POA (void)
{
  return PortableServer::POA::_duplicate (this->poa_.in ());
}

  ///////////////////////////////////////////////////////////////

CORBA::Long
CIAO::Container_Impl::init (const ::Deployment::Properties &properties
                            ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // @@ Gan/Jai, see how this init can create many instances of the
  // same container? Neither thread safe or single entity safe.
  this->properties_ = properties;

  // @@ Initialize container and create the internal container
  // implementation that actually interacts with installed
  // homes/components.

  // @@ We will need a container factory here later on when we support
  // more kinds of container implementations.

  // @@Jai, what is the condition to create an upgradeable container?
  // Where is it getting created and how? Need to address that.

  ACE_NEW_THROW_EX (this->container_,
                    CIAO::Session_Container (this->orb_.in (), this),
                    CORBA::INTERNAL ());
  ACE_CHECK_RETURN (-1);

  return this->container_->init (0,
                                 0
                                 ACE_ENV_ARG_PARAMETER);
}


Deployment::ComponentInfos *
CIAO::Container_Impl::install (
    const ::Deployment::ContainerImplementationInfo & container_impl_info
    ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        ::Deployment::UnknownImplId,
        ::Deployment::ImplEntryPointNotFound,
        ::Deployment::InstallationFailure,
        ::Components::InvalidConfiguration))
{
  Deployment::ComponentInfos_var retv;
  ACE_TRY
   {
     ACE_NEW_THROW_EX (retv,
                       Deployment::ComponentInfos,
                       CORBA::NO_MEMORY ());
     ACE_TRY_CHECK;

     // Get the ComponentImplementationInfos from the ContainerImplementationInfo
     // to avoid too long syntax representation
     const ::Deployment::ComponentImplementationInfos impl_infos =
       container_impl_info.impl_infos;

     const CORBA::ULong len = impl_infos.length ();
     retv->length (len);

     for (CORBA::ULong i = 0; i < len; ++i)
       {
         // Install home
         Components::CCMHome_var home =
           this->install_home (impl_infos[i]
                               ACE_ENV_ARG_PARAMETER);
         ACE_TRY_CHECK;

         Components::KeylessCCMHome_var kh =
           Components::KeylessCCMHome::_narrow (home.in ()
                                                ACE_ENV_ARG_PARAMETER);
         ACE_TRY_CHECK;

         if (CORBA::is_nil (kh.in ()))
           ACE_THROW_RETURN (Deployment::InstallationFailure (), 0);

         // Create component from home
         Components::CCMObject_var comp =
           kh->create_component (ACE_ENV_SINGLE_ARG_PARAMETER);
         ACE_TRY_CHECK;

         if (CORBA::is_nil (comp.in ()))
           ACE_THROW_RETURN (Deployment::InstallationFailure (), 0);

         if (this->component_map_.bind (impl_infos[i].component_instance_name.in (),
                                        Components::CCMObject::_duplicate (comp.in ())))
           ACE_TRY_THROW (Deployment::InstallationFailure ());

         // Set the return value.
         (*retv)[i].component_instance_name
           = impl_infos[i].component_instance_name.in ();

         (*retv)[i].component_ref = Components::CCMObject::_duplicate (comp.in ());

         // Deal with Component instance related Properties.
         // Now I am only concerning about the COMPOENTIOR and here is only
         // the hardcoded version of the configuration.

         const CORBA::ULong clen = impl_infos[i].component_config.length ();
         for (CORBA::ULong prop_len = 0; prop_len < clen; ++prop_len)
           {
             if (ACE_OS::strcmp (impl_infos[i].component_config[prop_len].name.in (),
                                 "ComponentIOR") == 0)
               {
                 if (CIAO::debug_level () > 1)
                   ACE_DEBUG ((LM_DEBUG, "Found property to write the IOR.\n"));
                 const char * path;
                 impl_infos[i].component_config[prop_len].value >>= path;

                 CORBA::String_var ior =
                   this->orb_->object_to_string (comp.in ()
                                                 ACE_ENV_ARG_PARAMETER);
                 ACE_TRY_CHECK;

                 if (CIAO::Utility::write_IOR (path, ior.in ()) != 0)
                   {
                     if (CIAO::debug_level () > 1)
                       ACE_DEBUG ((LM_DEBUG, "Failed to write the IOR.\n"));

                     ACE_TRY_THROW (CORBA::INTERNAL ());
                   }

               }
           }
       }
   }
  ACE_CATCHANY
   {
     ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                          "Container_Impl::install\t\n");
     ACE_RE_THROW;
   }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (0);

  return retv._retn ();
}

::Deployment::Properties *
CIAO::Container_Impl::properties (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ::Deployment::Properties *retval;

  ACE_NEW_THROW_EX (retval,
                    ::Deployment::Properties,
                    CORBA::INTERNAL ());
  ACE_CHECK_RETURN (0);

  *retval = this->properties_;

  return retval;
}

::Deployment::NodeApplication_ptr
CIAO::Container_Impl::get_node_application (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return ::Deployment::NodeApplication::_duplicate (this->nodeapp_.in ());
}

::Components::CCMHome_ptr
CIAO::Container_Impl::install_home (
    const ::Deployment::ComponentImplementationInfo & impl_info
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Deployment::UnknownImplId,
                   Deployment::ImplEntryPointNotFound,
                   Deployment::InstallationFailure,
                   Components::InvalidConfiguration))
{
  Components::CCMHome_var newhome =
    this->container_->ciao_install_home (impl_info.executor_dll.in (),
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


void
CIAO::Container_Impl::remove_home (const char * comp_ins_name
                                   ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::RemoveFailure))
{
  Components::CCMHome_ptr home;
  ACE_CString str (comp_ins_name);

  if (this->home_map_.find (str, home) != 0)
    ACE_THROW (CORBA::BAD_PARAM ());

  // @@TODO We should remove all components created by this home as well.
  // This is not implemented yet.

  this->container_->ciao_uninstall_home (home
                                         ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // If the previous calls failed, what should we do here??
  CORBA::release (home);

  // @@ Still need to remove the home if the previous operation fails?
  if (this->home_map_.unbind (str) == -1)
    ACE_THROW (::Components::RemoveFailure ());
}

::Components::CCMHomes *
CIAO::Container_Impl::get_homes (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  Components::CCMHomes * tmp;
  ACE_NEW_THROW_EX (tmp,
                    Components::CCMHomes (),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (0);

  Components::CCMHomes_var retval (tmp);

  const CORBA::ULong len = this->home_map_.current_size ();
  retval->length (len);


  CORBA::ULong i = 0;
  const Home_Iterator end = this->home_map_.end ();
  for (Home_Iterator iter (this->home_map_.begin ());
       iter != end;
       ++iter, ++i)
  {
    retval[i] = Components::CCMHome::_duplicate ( (*iter).int_id_);
  }

  return retval._retn ();
}

// Remove all homes and components
void
CIAO::Container_Impl::remove (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::RemoveFailure))
{
  // Remove all components first.
  this->remove_components (ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // Even if above operation failed we should still remove homes.
  const Home_Iterator end = this->home_map_.end ();
  for (Home_Iterator iter (this->home_map_.begin ());
       iter != end;
       ++iter)
    {
      this->container_->ciao_uninstall_home ( (*iter).int_id_
                                              ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      CORBA::release ( (*iter).int_id_);
    }

  this->home_map_.unbind_all ();

  //if (CIAO::debug_level () > 1)
  if (true)
    ACE_DEBUG ((LM_DEBUG, "Removed all homes and components from this container!\n"));
}

////////////////////////////////////////////////////////////////////////
//                      Internal helper functions.
////////////////////////////////////////////////////////////////////////

void
CIAO::Container_Impl::remove_components (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::RemoveFailure))
{
  // Remove all the components in the NodeApplication/Container
  // Release all component servant object.
  const Component_Iterator end = this->component_map_.end ();
  for (Component_Iterator iter (this->component_map_.begin ());
       iter != end;
       ++iter)
  {
    // Find the component home first, then call the remove_component
    // on the home.
    Components::CCMHome_ptr home;
    if (this->home_map_.find ( (*iter).ext_id_, home) != 0)
      ACE_THROW (CORBA::BAD_PARAM ());

    // This will call ccm_passivate on the component executor.
    home->remove_component ((*iter).int_id_);
    ACE_CHECK;

    CORBA::release ((*iter).int_id_);
  }

  this->component_map_.unbind_all ();
  // To this point the servant should have been destroyed. However,
  // if someone is still making calls on the servant, terrible thing
  // will happen.
}


// Below method is not used actually.
void
CIAO::Container_Impl::remove_component (const char * comp_ins_name
                                        ACE_ENV_ARG_DECL)
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
