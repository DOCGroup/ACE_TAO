// $Id$

#include "Home_Handler.h"
#include "ciao/Logger/Log_Macros.h"
#include "ciao/Base/CIAO_PropertiesC.h"
#include "ciao/Containers/Container_BaseC.h"
#include "CIAO_State.h"

namespace CIAO
{
  Home_Handler_i::Home_Handler_i (void)
  {
    CIAO_TRACE ("Home_Handler_i::Home_Handler_i");
  }
  
  // Destructor 
  Home_Handler_i::~Home_Handler_i (void)
  {
    CIAO_TRACE ("Home_Handler_i::~Home_Handler_i");
  }
    
  void Home_Handler_i::close (void)
  {
    this->instances_.clear ();
  }

  char * 
  Home_Handler_i::instance_type (void)
  {
    CIAO_TRACE ("Home_Handler_i::instance_type");
    return CORBA::string_dup ("edu.dre.vanderbilt.dre.CCM.Home");
  }
    
  void
  Home_Handler_i::install_instance (const ::Deployment::DeploymentPlan &plan,
                                    ::CORBA::ULong instanceRef,
                                    ::CORBA::Any_out instance_reference)
  {
    CIAO_TRACE ("Home_Handler_i::install_instance");
    
    ::CORBA::Any *retval_tmp = 0;
    ACE_NEW_THROW_EX (retval_tmp,
                      CORBA::Any (),
                      CORBA::NO_MEMORY ());
    instance_reference = retval_tmp;
    
    const ::Deployment::InstanceDeploymentDescription &idd (plan.instance[instanceRef]);
    const ::Deployment::MonolithicDeploymentDescription &mdd (plan.implementation[idd.implementationRef]);
    
    DAnCE::Utility::PROPERTY_MAP *pmap = 0;
    
    ACE_NEW_THROW_EX (pmap,
                      DAnCE::Utility::PROPERTY_MAP (idd.configProperty.length () +
                                                    mdd.execParameter.length ()),
                      CORBA::NO_MEMORY ());
    
    Deployment_Common::Instance_Info info;
    info.name = idd.name.in ();
    info.pmap.reset (pmap);

    DAnCE::Utility::build_property_map (*pmap,
                                        mdd.execParameter);
    DAnCE::Utility::build_property_map (*pmap,
                                        idd.configProperty);
    
    CORBA::String_var exec_art, exec_entry, svnt_art, svnt_entry, cont_id;
    
    using namespace CIAO::Deployment;
    CORBA::Any val;
    const char *tmp = 0;
    
    if ((pmap->find (SVNT_ENTRYPT, val)) == 0)
      {
        val >>= tmp;
        svnt_entry = tmp;
        CIAO_DEBUG (9, (LM_TRACE, CLINFO
                        "Home_Handler_i::install_instance - "
                        "Found Servant entrypoint %C\n", svnt_entry.in ()));
      }
    else
      {
        CIAO_ERROR (1, (LM_ERROR, CLINFO
                        "Home_Handler_i::install_instance - "
                        "Error: No Servant entrypoint (%C) provided, aborting installation\n",
                        SVNT_ENTRYPT));
        throw ::Deployment::InvalidComponentExecParameter (idd.name.in (),
                                                           "No servant entrypoint identified.");
      }

    if (pmap->find (SVNT_ARTIFACT, val) == 0)
      {
        val >>= tmp;
        CIAO_DEBUG (9, (LM_TRACE, CLINFO
                        "Home_Handler_i::install_instance - "
                        "Found Servant artifact %C\n", tmp));
        svnt_art = Deployment_Common::get_implementation (tmp, plan);
      }
    else
      {
        CIAO_ERROR (1, (LM_ERROR, CLINFO
                        "Home_Handler_i::install_instance - "
                        "Error: No Servant artifact provided, aborting installation\n"));
        throw ::Deployment::InvalidComponentExecParameter (idd.name.in (),
                                       "No servant artifact identified.");
      }


    if (pmap->find (EXEC_ARTIFACT, val) == 0)
      {
        val >>= tmp;
        exec_art = Deployment_Common::get_implementation (tmp, plan);
        CIAO_DEBUG (9, (LM_TRACE, CLINFO
                        "Home_Handler_i::install_instance - "
                        "Found executor artifact: %C\n", exec_art.in ()));
      }
    else
      {
        CIAO_ERROR (1, (LM_ERROR, CLINFO
                        "Home_Handler_i::install_instance - "
                        "Error: No Executor artifact provided, aborting installation\n"));
        throw ::Deployment::InvalidComponentExecParameter (idd.name.in (),
                                       "No executory artifact identified.\n");
      }
      
    if (pmap->find (HOME_FACTORY, val) == 0)
      {
        val >>= tmp;
        exec_entry = tmp;
        CIAO_DEBUG (9, (LM_TRACE, CLINFO
                        "Home_Handler_i::install_instance - "
                        "Found executor entrypoint: %C\n", exec_entry.in ()));
      }
    else
      {
        CIAO_ERROR (1, (LM_ERROR, CLINFO
                        "Home_Handler_i::install_instance - "
                        "Error: No Executor artifact provided, aborting installation\n"));
        throw ::Deployment::InvalidComponentExecParameter (idd.name.in (),
                                       "No executor entrypoint provided\n");
      }
      
    if (pmap->find (CONTAINER_ID, val) == 0)
      {
        val >>= tmp;
        cont_id = tmp;
        CIAO_DEBUG (9, (LM_TRACE, CLINFO
                        "Home_Handler_i::install_instance - "
                        "Found executor entrypoint: %C\n", exec_entry.in ()));
      }
    else
      {
        CIAO_ERROR (4, (LM_INFO, CLINFO
                        "Home_Handler_i::install_instance - "
                        "No container provided, using default container for instance <%C>\n",
                        idd.name.in  ()));
        cont_id = "";
      }
      
    ::CIAO::Container_var container = DEPLOYMENT_STATE::instance ()->fetch_container (cont_id);
      
    if (CORBA::is_nil (container))
      {
        CIAO_ERROR (1, (LM_INFO, CLINFO
                        "Home_Handler_i::install_instance - "
                        "Container with Id <%C> not installed, aborting.\n",
                        cont_id.in ()));
        throw ::Deployment::InvalidComponentExecParameter (idd.name.in (),
                                       "Invalid container\n");
      }
      
    Components::CCMHome_var home_ref;
      
    try
      {
        home_ref = container->install_home (exec_art,
                                            exec_entry,
                                            svnt_art,
                                            svnt_entry,
                                            idd.name.in ());

        ::Components::ConfigValues attr_config;
        Deployment_Common::create_attribute_configuration (idd.configProperty,
                                                           attr_config);
        
        container->set_attributes (home_ref.in (), attr_config);
      }
    catch (::CORBA::Exception &ex)
      {
        CIAO_ERROR (1, (LM_ERROR, CLINFO
                        "Home_Handler_i::install_instance - "
                        "Caught CORBA exception <%C>\n",
                        ex._info ().c_str ()));
        throw ::Deployment::StartError (idd.name.in (),
                                        ex._info ().c_str ());
      }
    catch (...)
      {
        CIAO_ERROR (1, (LM_ERROR, CLINFO
                        "Home_Handler_i::install_instance - "
                        "Caught unknown C++ exception\n"));
        throw ::Deployment::StartError (idd.name.in (),
                                        "Unknown C++ exception\n");
      }
      
    if (CORBA::is_nil (home_ref))
      {
        CIAO_ERROR (1, (LM_ERROR, CLINFO
                        "Home_Handler_i::install_instance - "
                        "Container provided nil object reference for home.\n"));
        throw ::Deployment::StartError (idd.name.in (),
                                        "Container provided nil object reference");
      }
      
    this->instances_[idd.name.in ()] = info;

    DEPLOYMENT_STATE::instance ()->add_home (idd.name.in (), cont_id, home_ref.in ());

      
    *instance_reference <<= home_ref;
  }
  
  
  void
  Home_Handler_i::activate_instance (const ::Deployment::DeploymentPlan & ,
                                     ::CORBA::ULong ,
                                     const ::CORBA::Any &)
  {
    CIAO_TRACE ("Home_Handler_i::activate_instance");
    // no activation needed.
  }

  void
  Home_Handler_i::passivate_instance (const ::Deployment::DeploymentPlan & ,
                                      ::CORBA::ULong ,
                                      const ::CORBA::Any &)
  {
    CIAO_TRACE ("Home_Handler_i::passivate_instance");
    // no passivation needed.
  }
  
  void
  Home_Handler_i::remove_instance (const ::Deployment::DeploymentPlan & plan,
                                   ::CORBA::ULong instanceRef,
                                   const ::CORBA::Any &)
  {
    CIAO_TRACE ("Home_Handler_i::remove_instance");
    
    const char *name = plan.instance[instanceRef].name.in ();
    Deployment_Common::INSTANCES::iterator instance
      = this->instances_.find (name);
    
    if (instance == this->instances_.end ())
      {
        CIAO_ERROR (1, (LM_ERROR, CLINFO 
                        "Home_Handler_i::remove_instance - "
                        "Instructed to remove unknown home instance <%C>\n",
                        name));
        throw ::Deployment::StopError (name,
                                       "Wrong instance handler for home instance\n");
      }
    
    CIAO_DEBUG (8, (LM_DEBUG, CLINFO
                    "Home_Handler_i::remove_instance - "
                    "Attempting removal of home instance <%C>\n",
                    name));
    
    CORBA::Any val;
    const char *cont_id = 0;
    
    if (instance->second.pmap->find (Deployment::CONTAINER_ID, val) == 0)
      {
        val >>= cont_id;
        CIAO_DEBUG (9, (LM_TRACE, CLINFO
                        "Home_Handler_i::remove_instance - "
                        "Found home container: %C\n", cont_id));
      }
    else
      {
        CIAO_ERROR (4, (LM_INFO, CLINFO
                        "Home_Handler_i::remove_instance - "
                        "No container provided, using default container for instance <%C>\n",
                        name));
        cont_id = "";
      }

    ::CIAO::Container_var container =
      DEPLOYMENT_STATE::instance ()->fetch_container (cont_id);
      
    if (CORBA::is_nil (container))
      {
        CIAO_ERROR (1, (LM_INFO, CLINFO
                        "Home_Handler_i::remove_instance - "
                        "Container with Id <%C> not installed, aborting.\n",
                        cont_id));
        throw ::Deployment::StopError (name,
                                       "Invalid container\n");
      }
    
    Components::CCMHome_var ref
      = DEPLOYMENT_STATE::instance ()->fetch_home (name);  

    try
      {
        container->uninstall_home (ref);
      }
    catch (CORBA::Exception &ex)
      {
        CIAO_ERROR (1, (LM_ERROR, CLINFO
                        "Home_Handler_i::remove_instance - "
                        "Caught CORBA exception <%C>\n",
                        ex._info ().c_str ()));
        throw ::Deployment::StopError (name,
                                       ex._info ().c_str ());
      }
    catch (...)
      {
        CIAO_ERROR (1, (LM_ERROR, CLINFO
                        "Home_Handler_i::remove_instance - "
                        "Caught unknown C++ exception\n"));
        throw ::Deployment::StopError (name,
                                       "Unknown C++ exception\n");
      }
      
    this->instances_.erase (instance);
      
    CIAO_DEBUG (4, (LM_INFO, CLINFO
                    "Home_Handler_i::remove_instance - "
                    "Instance <%C> successfully removed\n",
                    name));
  }

  void 
  Home_Handler_i::provide_endpoint_reference (const ::Deployment::DeploymentPlan &,
                                              ::CORBA::ULong,
                                              ::CORBA::Any_out)
  {
    CIAO_TRACE ("Home_Handler_i::provide_endpoint_reference");
    CIAO_ERROR (1, (LM_ERROR, CLINFO 
                    "Home_Handler_i::provide_endpoint_reference - "
                    "Unable to provide any endpoints.\n"));
    throw CORBA::NO_IMPLEMENT ();
  }
    
  void
  Home_Handler_i::connect_instance (const ::Deployment::DeploymentPlan &,
                                    ::CORBA::ULong,
                                    const ::CORBA::Any &)
  {
    CIAO_TRACE ("Home_Handler_i::connect_instance");
    CIAO_ERROR (1, (LM_ERROR, CLINFO
                    "Home_Handler_i::connect_instance - ",
                    "No connections allowed for homes.\n"));
    throw CORBA::NO_IMPLEMENT ();
  }
  
    
  void
  Home_Handler_i::disconnect_instance (const ::Deployment::DeploymentPlan &,
                                       ::CORBA::ULong)
  {
    CIAO_TRACE ("Home_Handler_i::disconnect_instance");
    CIAO_ERROR (1, (LM_ERROR, CLINFO
                    "Home_Handler_i::disconnect_instance - ",
                    "No connections allowed for homes.\n"));
    throw CORBA::NO_IMPLEMENT ();
  }
  
  
  void
  Home_Handler_i::instance_configured (const ::Deployment::DeploymentPlan &,
                                       ::CORBA::ULong)
  {
    CIAO_TRACE ("Home_Handler_i::instance_configured");
    // nothing to be done
  }

  void
  Home_Handler_i::configure ( const ::Deployment::Properties&)
  {
    
  }
}

extern "C"
{
  ::DAnCE::InstanceDeploymentHandler_ptr 
  CIAO_Locality_Handler_Export create_Home_Handler (void)
  {
    return new CIAO::Home_Handler_i ();
  }
}
