// $Id$

#include "Container_Impl.h"
#include "ciao/CCM_StandardConfiguratorC.h"
#include "ciao/CCM_KeylessCCMHomeC.h"

#include "orbsvcs/CosNamingC.h"

#if !defined (__ACE_INLINE__)
# include "Container_Impl.inl"
#endif /* __ACE_INLINE__ */

CIAO::Container_Impl::~Container_Impl ()
{
}

PortableServer::POA_ptr
CIAO::Container_Impl::_default_POA (void)
{
  CIAO_TRACE ("CIAO::Container_Impl::_default_POA");
  return PortableServer::POA::_duplicate (this->poa_.in ());
}

  ///////////////////////////////////////////////////////////////

CORBA::Long
CIAO::Container_Impl::init (const CORBA::PolicyList *policies)
{
  CIAO_TRACE ("CIAO::Container_Impl::init");
  // @@ Initialize container and create the internal container
  // implementation that actually interacts with installed
  // homes/components.

  // @@ We will need a container factory here later on when we support
  // more kinds of container implementations.

  // @@Jai, what is the condition to create an upgradeable container?
  // Where is it getting created and how? Need to address that.

  if (this->static_entrypts_maps_ == 0)
    {
      this->container_.reset (new CIAO::Session_Container (this->orb_.in (), this));
    }
  else
    {
      this->container_.reset (new CIAO::Session_Container (this->orb_.in (), 
                                                           this,
                                                           1,
                                                           this->static_entrypts_maps_));
    }

  return this->container_->init (0, policies);
}


Deployment::ComponentInfos *
CIAO::Container_Impl::install (
    const ::Deployment::ContainerImplementationInfo & container_impl_info
  )
{
  CIAO_TRACE ("CIAO::Container_Impl::install");
  Deployment::ComponentInfos_var retv;
  try
   {
     ACE_NEW_THROW_EX (retv,
                       Deployment::ComponentInfos,
                       CORBA::NO_MEMORY ());

     // Get the ComponentImplementationInfos from the
     // ContainerImplementationInfo
     // to avoid too long syntax representation
     const ::Deployment::ComponentImplementationInfos impl_infos =
       container_impl_info.impl_infos;

     CORBA::ULong const len = impl_infos.length ();
     retv->length (len);
     REC_POL_MAP rec_pol_map;

     for (CORBA::ULong i = 0; i < len; ++i)
       {
         const CORBA::ULong cplen = impl_infos[i].component_config.length ();
         for (CORBA::ULong cp_len = 0; cp_len < cplen; ++cp_len)
           {
             if (impl_infos[i].component_config[cp_len].
                 value.type ()->kind () == CORBA::tk_string)
               {
                 const char* policy_set_id;
                 ACE_CString receptacle_name;
                 ACE_CString instance_name;
                 impl_infos[i].component_config[cp_len].value >>=
                   policy_set_id;
                 bool result = this->configurator_.policy_exists (
                   policy_set_id);
                 if (result == true)
                   {
                     receptacle_name = impl_infos[i].component_config[cp_len].
                       name.in ();
                     instance_name = impl_infos[i].
                       component_instance_name.in ();
                     receptacle_name += "_";
                     receptacle_name += instance_name;
                     CORBA::PolicyList_var policies =
                       this->configurator_.find_policies_by_name (
                         policy_set_id);
                     CORBA::PolicyList temp_policies (0);
                     if (policies != 0)
                       {
                         temp_policies = *policies;
                       }
                     rec_pol_map.bind (receptacle_name, temp_policies);
                   }
               }
           }

         // Install home
         Components::CCMHome_var home =
           this->install_home (impl_infos[i]);

         Components::KeylessCCMHome_var kh =
           Components::KeylessCCMHome::_narrow (home.in ());

         if (CORBA::is_nil (kh.in ()))
           throw Deployment::InstallationFailure ();

         // Create component from home
         Components::CCMObject_var comp = kh->create_component ();

         if (CORBA::is_nil (comp.in ()))
           throw Deployment::InstallationFailure ();

         if (this->component_map_.bind
                (impl_infos[i].component_instance_name.in (),
                 Components::CCMObject::_duplicate (comp.in ())))
           {
             ACE_DEBUG ((LM_DEBUG,
                         "CIAO (%P|%t) Container_Impl.cpp -"
                         "CIAO::Container_Impl::install -"
                         "error in binding component "
                         "instance name [%s] into the component map \n",
                         impl_infos[i].component_instance_name.in ()));
             throw Deployment::InstallationFailure ();
           }

         // Set the return value.
         (*retv)[i].component_instance_name
           = impl_infos[i].component_instance_name.in ();

         (*retv)[i].component_ref =
             Components::CCMObject::_duplicate (comp.in ());

         // Deal with Component instance related Properties.
         // Now I am only concerning about the COMPOENTIOR and attribute
         // configuration initialization.

         // I need to map Properties to Components::ConfigValues
         ::Components::ConfigValues comp_attributes;
         comp_attributes.length (0);

         const CORBA::ULong clen = impl_infos[i].component_config.length ();
         for (CORBA::ULong prop_len = 0; prop_len < clen; ++prop_len)
           {
             // Set up the ComponentIOR attribute
             if (ACE_OS::strcmp
                  (impl_infos[i].component_config[prop_len].name.in (),
                   "ComponentIOR") == 0)
               {
                 const char * path;
                 impl_infos[i].component_config[prop_len].value >>= path;

                 CORBA::String_var ior =
                   this->orb_->object_to_string (comp.in ());

                 if (CIAO::Utility::write_IOR (path, ior.in ()) != 0)
                   {
                     if (CIAO::debug_level () > 1)
                       ACE_DEBUG ((LM_DEBUG, "Failed to write the IOR.\n"));

                     throw CORBA::INTERNAL ();
                   }
               }

             // Set up the naming service attribute
             if (ACE_OS::strcmp
                  (impl_infos[i].component_config[prop_len].name.in (),
                   "RegisterNaming") == 0)
               {
                 const char * naming_context;
                 impl_infos[i].
                    component_config[prop_len].value >>= naming_context;

                 // Register the component with the naming service
                 ACE_DEBUG ((LM_DEBUG,
                             "Register component with naming service.\n"));
                 bool result =
                   register_with_ns (
                           naming_context,
                           this->orb_.in (),
                           Components::CCMObject::_duplicate (comp.in ())
                         );

                 if (!result)
                   {
                     ACE_DEBUG ((LM_DEBUG,
                                 "Failed to register with naming service.\n"));
                   }
                 else
                   {
                     if (this->naming_map_.bind
                         (impl_infos[i].component_instance_name.in (),
                          ACE_CString (naming_context)))
                       {
                         ACE_DEBUG ((LM_DEBUG,
                                     "CIAO (%P|%t) Container_Impl.cpp -"
                                     "CIAO::Container_Impl::install -"
                                     "error in binding component "
                                     "instance name [%s] into the naming map \n",
                                     impl_infos[i].component_instance_name.in ()));
                         throw Deployment::InstallationFailure ();
                       }
                   }


               }

             // Initialize attributes through StandardConfigurator interface
             // @@Todo: Currently I have to manually map
             // the Deployment::Properties to
             // Components::ConfigValues, we should use a
             // common data structure in
             // the future. - Gan
             CORBA::ULong cur_len = comp_attributes.length ();
             comp_attributes.length (cur_len + 1);

             Components::ConfigValue *item = new OBV_Components::ConfigValue ();
             item->name (impl_infos[i].component_config[prop_len].name.in ());
             CORBA::Any tmp = impl_infos[i].component_config[prop_len].value;
             item->value (tmp);

             comp_attributes[cur_len] = item;
           }

         if (comp_attributes.length () != 0)
         {
           //std_configurator.set_configuration
           ::Components::StandardConfigurator_var std_configurator =
             comp->get_standard_configurator ();

           std_configurator->set_configuration (comp_attributes);
         }
       }
     this->container_->set_receptacle_policy_map (rec_pol_map);
   }
  catch (const CORBA::Exception& ex)
   {
     ex._tao_print_exception ("Container_Impl::install\t\n");
     throw;
   }

  return retv._retn ();
}

::Deployment::Properties *
CIAO::Container_Impl::properties ()
{
  CIAO_TRACE ("CIAO::Container_Impl::properties");
  ::Deployment::Properties *retval = 0;

  ACE_NEW_THROW_EX (retval,
                    ::Deployment::Properties,
                    CORBA::NO_MEMORY ());

  *retval = this->properties_;

  return retval;
}

::Deployment::NodeApplication_ptr
CIAO::Container_Impl::get_node_application ()
{
  CIAO_TRACE ("CIAO::Container_Impl::get_node_application");
  return ::Deployment::NodeApplication::_duplicate (this->nodeapp_.in ());
}

::Components::CCMHome_ptr
CIAO::Container_Impl::install_home (
    const ::Deployment::ComponentImplementationInfo & impl_info)
{
  CIAO_TRACE ("CIAO::Container_Impl::install_home");
  if (CIAO::debug_level () > 9)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "CIAO (%P|%t) Container_Impl.cpp -"
                  "CIAO::Container_Impl::install_home -"
                  "installing home for component "
                  "instance [%s] \n",
                  impl_info.component_instance_name.in ()));
    }

  Components::CCMHome_var newhome =
    this->container_->ciao_install_home (impl_info.executor_dll.in (),
                                         impl_info.executor_entrypt.in (),
                                         impl_info.servant_dll.in (),
                                         impl_info.servant_entrypt.in (),
                                         impl_info.component_instance_name.in ());

  if (CIAO::debug_level () > 9)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "CIAO (%P|%t) Container_Impl.cpp -"
                  "CIAO::Container_Impl::install_home -"
                  "success in installing home for component "
                  "instance [%s] \n",
                  impl_info.component_instance_name.in ()));
    }

  // We don't have to do _narrow since the generated code makes sure of
  // the object type for us
  // Bind the home in the map.
  if (this->home_map_.bind (impl_info.component_instance_name.in (),
                            Components::CCMHome::_duplicate (newhome.in ())))
    {
      ACE_ERROR ((LM_ERROR,
                  "CIAO (%P|%t) Container_Impl.cpp -"
                  "CIAO::Container_Impl::install_home -"
                  "error in binding home for component "
                  "instance [%s] \n",
                  impl_info.component_instance_name.in ()));
      throw Deployment::InstallationFailure ();
    }

  //Note: If the return value will be discarded, it must be kept in a var or
  //      release () will have to be called explicitly.
  return newhome._retn ();
}


void
CIAO::Container_Impl::remove_home (const char * comp_ins_name)
{
  CIAO_TRACE ("CIAO::Container_Impl::remove_home");

  Components::CCMHome_ptr home;
  ACE_CString str (comp_ins_name);

  if (this->home_map_.find (str, home) != 0)
    throw CORBA::BAD_PARAM ();

  // @@TODO We should remove all components created by this home as well.
  // This is not implemented yet.

  this->container_->ciao_uninstall_home (home);

  // If the previous calls failed, what should we do here??
  CORBA::release (home);

  // @@ Still need to remove the home if the previous operation fails?
  if (this->home_map_.unbind (str) == -1)
    throw ::Components::RemoveFailure ();
}

// Remove all homes and components
void
CIAO::Container_Impl::remove ()
{
  CIAO_TRACE ("CIAO::Container_Impl::remove");

  // Remove all components first.
  this->remove_components ();

  // Even if above operation failed we should still remove homes.
  const Home_Iterator end = this->home_map_.end ();
  for (Home_Iterator iter (this->home_map_.begin ());
       iter != end;
       ++iter)
    {
      this->container_->ciao_uninstall_home ( (*iter).int_id_);

      CORBA::release ( (*iter).int_id_);
    }

  this->home_map_.unbind_all ();

  if (CIAO::debug_level () > 3)
    ACE_DEBUG ((LM_DEBUG,
                "Removed all homes and components from this container!\n"));
}

////////////////////////////////////////////////////////////////////////
//                      Internal helper functions.
////////////////////////////////////////////////////////////////////////

void
CIAO::Container_Impl::remove_components ()
{
  CIAO_TRACE ("CIAO::Container_Impl::remove_components");

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
      throw CORBA::BAD_PARAM ();

    // This will call ccm_passivate on the component executor.
    home->remove_component (((*iter).int_id_).in ());

    //CORBA::release (((*iter).int_id_).in ());
  }

  this->component_map_.unbind_all ();
  // To this point the servant should have been destroyed. However,
  // if someone is still making calls on the servant, terrible thing
  // will happen.
}


// Below method is not used actually.
void
CIAO::Container_Impl::remove_component (const char * comp_ins_name)
{
  CIAO_TRACE ("CIAO::Container_Impl::remove_component");

  Components::CCMObject_var comp;
  Components::CCMHome_ptr home;

  ACE_CString naming_context;

  ACE_CString str (comp_ins_name);

  if (CIAO::debug_level () > 5)
    ACE_DEBUG ((LM_DEBUG, "CIAO::COntainer_Impl::remove_component: Removing comp_ins_name:: %s\n",
                str.c_str ()));

  /* Before we do remove component we have to inform the homeservant so
   * Component::ccm_passivate ()
   * constainer::uninstall_component () ->deactivate_object () will be called.
   *
   * ccm_remove will be called when the poa destroys the servant.
   */

  if (this->component_map_.find (str, comp) != 0)
    throw CORBA::BAD_PARAM ();

  if (this->home_map_.find (str, home) != 0)
    throw CORBA::BAD_PARAM ();

  // This will call ccm_passivate on the component executor.
  home->remove_component (comp.in ());

  // If the previous calls failed, what should we do here??

  // @@ Still need to remove the home if the previous operation fails?
  if (this->component_map_.unbind (str) == -1)
    throw ::Components::RemoveFailure ();

  if (this->naming_map_.find (str, naming_context) == 0)
    {

      bool const result =
        unregister_with_ns (
                            naming_context.c_str (),
                            this->orb_.in ()
                            );

      if (!result)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "Failed to unregister with naming service.\n"));
        }
      else
        {
          if (this->naming_map_.unbind (str) == -1)
            throw ::Components::RemoveFailure ();
        }
    }
}

bool
CIAO::Container_Impl::register_with_ns (const char * s,
                                        CORBA::ORB_ptr orb,
                                        Components::CCMObject_ptr obj)
{
  CIAO_TRACE ("CIAO::Container_Impl::register_with_ns");

  try
    {
    // Obtain the naming service
      CORBA::Object_var naming_obj =
        orb->resolve_initial_references ("NameService");

      if (CORBA::is_nil (naming_obj.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           "DAnCE: (%P|%t) Unable to get the Naming Service.\n"),
                           false);

      CosNaming::NamingContextExt_var root =
        CosNaming::NamingContextExt::_narrow (naming_obj.in ());

      CosNaming::Name name (0);
      name.length (0);

      // Get the multicomponent naming context from the <naming_context>.
      // The convention of this <naming_context> input string is that
      // different naming context is separated by character '/', such as
      // "create a naming context A/B/C/D".
      ACE_CString tmp (s);
      char * naming_string = tmp.rep ();
      char seps[]   = "/:";

      char *token, *lastToken = 0;
      token = ACE_OS::strtok (naming_string, seps);

      for (CORBA::ULong i = 0; token != 0; ++i)
        {
            // While there still are tokens in the "naming_string"
            name.length (name.length () + 1);
            name[i].id = CORBA::string_dup (token);

            // Get next naming context
            lastToken = token;
            token = ACE_OS::strtok ( 0, seps );
        }

      if (name.length() > 1)
         {
            // Let's create the context path first
            name.length(name.length()-1);
            Utility::NameUtility::CreateContextPath (root.in (), name);
            name.length(name.length()+1);
            name[name.length()-1].id = CORBA::string_dup(lastToken);
         }

      // Bind the actual object
      Utility::NameUtility::BindObjectPath (root.in (), name, obj);

      return true;
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("CIAO (%P|%t) Container_Impl.cpp -"
                               "CIAO::Container_Impl::register_with_ns -"
                               "NodeApplication: failed to register "
                               "with naming service.");
      return false;
    }
  return true;
}

bool
CIAO::Container_Impl::unregister_with_ns (const char * obj_name,
                                          CORBA::ORB_ptr orb)
{
  CIAO_TRACE ("CIAO::Container_Impl::unregister_with_ns");

  try
    {
    // Obtain the naming service
      CORBA::Object_var naming_obj =
        orb->resolve_initial_references ("NameService");

      if (CORBA::is_nil (naming_obj.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to get the Naming Service.\n"),
                          false);

      CosNaming::NamingContext_var naming_context =
        CosNaming::NamingContext::_narrow (naming_obj.in ());

      CosNaming::Name name (0);
      name.length (0);

      // Get the multicomponent naming context from the <naming_context>.
      // The convention of this <naming_context> input string is that
      // different naming context is separated by character '/', such as
      // "create a naming context A/B/C/D".
      ACE_CString tmp (obj_name);
      char * naming_string = tmp.rep ();
      char seps[]   = "/:";

      char *token, *lastToken = 0;
      token = ACE_OS::strtok (naming_string, seps);

      for (CORBA::ULong i = 0; token != 0; ++i)
        {
            // While there still are tokens in the "naming_string"
            name.length (name.length () + 1);
            name[i].id = CORBA::string_dup (token);

            // Get next naming context
            lastToken = token;
            token = ACE_OS::strtok ( 0, seps );
        }

      // Unregister with the Name Server
      ACE_DEBUG ((LM_DEBUG,
                  "Unregister component with the name server : %s!\n",
                  obj_name));
      naming_context->unbind (name);

      return true;
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("CIAO (%P|%t) Container_Impl.cpp -"
                               "CIAO::Container_Impl::unregister_with_ns -"
                               "NodeApplication: failed to unregister "
                               "with naming service.");
      return false;
    }
  return true;
}
