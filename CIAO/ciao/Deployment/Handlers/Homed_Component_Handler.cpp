// $Id$

#include "Homed_Component_Handler.h"
#include "ccm/CCM_KeylessCCMHomeC.h"
#include "ciao/Logger/Log_Macros.h"
#include "ciao/Base/CIAO_PropertiesC.h"
#include "DAnCE/Deployment/Deployment_StartErrorC.h"
#include "CIAO_Handler_Common.h"
#include "CIAO_State.h"

namespace CIAO
{
  Homed_Component_Handler_i::Homed_Component_Handler_i ()
  {
    CIAO_TRACE ("Homed_Component_Handler_i::Homed_Component_Handler_i");
  }
  
  // Destructor 
  Homed_Component_Handler_i::~Homed_Component_Handler_i (void)
  {
    CIAO_TRACE ("Homed_Component_Handler_i::~Homed_Component_Handler_i");
  }
    
  char * 
  Homed_Component_Handler_i::instance_type (void)
  {
    CIAO_TRACE ("Homed_Component_Handler_i::instance_type");
    return CORBA::string_dup ("edu.vanderbilt.dre.CCM.HomedComponent");
  }
    
  void
  Homed_Component_Handler_i::install_instance (const ::Deployment::DeploymentPlan &plan,
                                               ::CORBA::ULong instanceRef,
                                               ::CORBA::Any_out instance_reference)
  {
    CIAO_TRACE ("Homed_Component_Handler_i::install_instance");

    CORBA::Any *any = 0;
    ACE_NEW_THROW_EX (any,
                      CORBA::Any (),
                      CORBA::NO_MEMORY ());
    
    instance_reference = any;
    
    const ::Deployment::InstanceDeploymentDescription &idd (plan.instance[instanceRef]);
    const ::Deployment::MonolithicDeploymentDescription &mdd (plan.implementation[idd.implementationRef]);
    
    CIAO_DEBUG (7, (LM_DEBUG, CLINFO
                    "Homed_Component_Handler_i::install_instance - "
                    "Starting deployment of <%C>\n",
                    plan.instance[instanceRef].name.in ()));
    DAnCE::Utility::PROPERTY_MAP *pmap;
    
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
    
    
    using namespace CIAO::Deployment;
    CORBA::Any val;
    const char *tmp = 0;

    if (pmap->find (COMPONENT_HOME, val) == 0)
        {
          val >>= tmp;
          CIAO_DEBUG (9, (LM_TRACE, CLINFO
                          "Homed_Component_Handler_i::install_instance - "
                          "Found component home ID %C\n", tmp));
        }
      else
        {
          CIAO_ERROR (1, (LM_ERROR, CLINFO
                          "Homed_Component_Handler_i::install_instance - "
                          "Error: No home ID  provided, aborting installation\n"));
          throw ::Deployment::StartError (idd.name.in (),
                                         "No Home identified.");
        }

    Components::CCMHome_var tmp_home = DEPLOYMENT_STATE::instance ()->fetch_home (tmp);
    
    Components::KeylessCCMHome_var home = Components::KeylessCCMHome::_narrow (tmp_home);
    if (CORBA::is_nil (home))
      {
        CIAO_ERROR (1, (LM_ERROR, CLINFO
                        "Homed_Component_Handler_i::install_instance - "
                        "Object reference for Home <%C> is not available for component <%C>\n",
                        tmp, idd.name.in ()));
        throw ::Deployment::StartError (idd.name.in (),
                                        "Home ID is not available");
      }
    
    const char *cont_id = 
      DEPLOYMENT_STATE::instance ()->instance_to_container (tmp);
    
    ::CIAO::Container_var container = DEPLOYMENT_STATE::instance ()->fetch_container (cont_id);

    Components::CCMObject_var ref;
    try
      {
        ref = home->create_component ();
        
        ::Components::ConfigValues attr_config;
        Deployment_Common::create_attribute_configuration (idd.configProperty,
                                                           attr_config);
        
        
        container->set_attributes (ref.in (),
                                   attr_config);
      }
    catch (const CORBA::Exception &ex)
      {
        CIAO_ERROR (1, (LM_ERROR, CLINFO
                        "Homed_Component_Handler_i::install_instance - "
                        "Caught CORBA exception while installing component <%C>: %C\n",
                        idd.name.in (),
                        ex._info ().c_str ()));
        throw ::Deployment::StartError (idd.name.in (),
                                      ex._info ().c_str ());
      }
    catch (...)
      {
        CIAO_ERROR (1, (LM_ERROR, CLINFO
                        "Homed_Component_Handler_i::install_instance - "
                        "Caught C++ exception while installing component <%C>\n",
                        idd.name.in ()));
        throw ::Deployment::StartError (idd.name.in (),
                                        "Unknown C++ exception");
      }
    
    if (CORBA::is_nil (ref.in ()))
      {
        CIAO_ERROR (1, (LM_ERROR, CLINFO
                        "Homed_Component_Handler_i::install_instance - "
                        "Received nil object reference from create on Home <%C> "
                        "while creating component <%C>\n",
                        tmp,
                        idd.name.in ()));
        throw ::Deployment::StartError  (idd.name.in (),
                                         "Nil reference from home on create");
      }
    
    
    CIAO_DEBUG (4, (LM_INFO, CLINFO
                    "Homed_Component_Handler_i::install_instance - "
                    "Successfully created component <%C>\n",
                    idd.name.in ()));
    
    this->instances_[idd.name.in ()] = info;
    
    DEPLOYMENT_STATE::instance ()->add_component (idd.name.in (),
                                                  cont_id,
                                                  ref.in ());    
    (*instance_reference) <<= ref;
  }

  void
  Homed_Component_Handler_i::activate_instance (const ::Deployment::DeploymentPlan &plan,
                                     ::CORBA::ULong instanceRef,
                                     const ::CORBA::Any &)
  {
    CIAO_TRACE ("Homed_Component_Handler_i::activate_instance");
    
    const char *name = plan.instance[instanceRef].name.in ();
    
    CIAO_DEBUG (8, (LM_TRACE, CLINFO
                    "Homed_Component_Handler_i::activate_instance - "
                    "Starting activation of component instance <%C>\n",
                    name));
    const char *container = 
      DEPLOYMENT_STATE::instance ()->instance_to_container (name);
    
    CIAO::Container_var cont = 
      DEPLOYMENT_STATE::instance ()->fetch_container (container);
    
    Components::CCMObject_var comp =
      DEPLOYMENT_STATE::instance ()->fetch_component (name);
    
    try
      {
        cont->activate_component (comp.in ());
      }
    catch (CORBA::Exception &ex)
      {
        CIAO_ERROR (1, (LM_ERROR, CLINFO
                        "Homed_Component_Handler_i::activate_instance - "
                        "Caught CORBA Exception while activating instance <%C>: %C\n",
                        name,
                        ex._info ().c_str ()));
        throw ::Deployment::StartError (name,
                                        ex._info ().c_str ());
      }
    catch (...)
      {
        CIAO_ERROR (1, (LM_ERROR, CLINFO
                        "Homed_Component_Handler_i::activate_instance - "
                        "Caught C++ Exception while activating instance <%C>\n",
                        name));
        throw ::Deployment::StartError (name,
                                        "Unknown C++ exception during activation");
      }
  }

  void
  Homed_Component_Handler_i::passivate_instance (const ::Deployment::DeploymentPlan &plan,
                                           ::CORBA::ULong instanceRef,
                                           const ::CORBA::Any &)
  {
    CIAO_TRACE ("Homed_Component_Handler_i::passivate_instance");
    
    const char *name = plan.instance[instanceRef].name.in ();
    
    CIAO_DEBUG (8, (LM_TRACE, CLINFO
                    "Homed_Component_Handler_i::passivate_instance - "
                    "Starting passivation of component instance <%C>\n",
                    name));
    const char *container = 
      DEPLOYMENT_STATE::instance ()->instance_to_container (name);
    
    CIAO::Container_var cont = 
      DEPLOYMENT_STATE::instance ()->fetch_container (container);
    
    Components::CCMObject_var comp =
      DEPLOYMENT_STATE::instance ()->fetch_component (name);
    
    try
      {
        cont->passivate_component (comp.in ());
      }
    catch (CORBA::Exception &ex)
      {
        CIAO_ERROR (1, (LM_ERROR, CLINFO
                        "Homed_Component_Handler_i::passivate_instance - "
                        "Caught CORBA Exception while passivating instance <%C>: %C\n",
                        name,
                        ex._info ().c_str ()));
        throw ::Deployment::StopError (name,
                                        ex._info ().c_str ());
      }
    catch (...)
      {
        CIAO_ERROR (1, (LM_ERROR, CLINFO
                        "Homed_Component_Handler_i::passivate_instance - "
                        "Caught C++ Exception while passivating instance <%C>\n",
                        name));
        throw ::Deployment::StartError (name,
                                        "Unknown C++ exception during passivation");
      }
  }
    
  void
  Homed_Component_Handler_i::remove_instance (const ::Deployment::DeploymentPlan & plan,
                                              ::CORBA::ULong instanceRef,
                                              const ::CORBA::Any &)
  {
    CIAO_TRACE ("Homed_Component_Handler_i::remove_instance");
    
    const char *name = plan.instance[instanceRef].name.in ();
    Deployment_Common::INSTANCES::iterator instance
      = this->instances_.find (name);
    
    if (instance == this->instances_.end ())
      {
        CIAO_ERROR (1, (LM_ERROR, CLINFO 
                        "Homed_Component_Handler_i::remove_instance - "
                        "Instructed to remove unknown homed component instance <%C>\n",
                        name));
        throw ::Deployment::StopError (name,
                                       "Wrong instance handler for homed component instance\n");
      }
    
    CIAO_DEBUG (8, (LM_DEBUG, CLINFO
                    "Homed_Component_Handler_i::remove_instance - "
                    "Attempting removal of homed component instance <%C>\n",
                    name));
        
    using namespace CIAO::Deployment;
    CORBA::Any val;
    const char *tmp;

    if (instance->second.pmap->find (COMPONENT_HOME, val) == 0)
        {
          val >>= tmp;
          CIAO_DEBUG (9, (LM_TRACE, CLINFO
                          "Homed_Component_Handler_i::remove_instance - "
                          "Found component home ID %C\n", tmp));
        }
      else
        {
          CIAO_ERROR (1, (LM_ERROR, CLINFO
                          "Homed_Component_Handler_i::remove_instance - "
                          "Error: No home ID  provided, aborting removal\n"));
          throw ::Deployment::StopError (name,
                                         "No Home identified.");
        }

    Components::CCMHome_var home = DEPLOYMENT_STATE::instance ()->fetch_home (tmp);
    
    if (CORBA::is_nil (home))
      {
        CIAO_ERROR (1, (LM_ERROR, CLINFO
                        "Homed_Component_Handler_i::remove_instance - "
                        "Object reference for Home <%C> is not available for component <%C>\n",
                        tmp, name));
        throw ::Deployment::StopError (name,
                                       "Home ID is not available");
      }
    
      Components::CCMObject_var ref
        = DEPLOYMENT_STATE::instance ()->fetch_component (name);

    try
      {
        home->remove_component (ref.in ());
      }
    catch (const CORBA::Exception &ex)
      {
        CIAO_ERROR (1, (LM_ERROR, CLINFO
                        "Homed_Component_Handler_i::remove_instance - "
                        "Caught CORBA exception while removing component <%C>: %C\n",
                        name,
                        ex._info ().c_str ()));
        throw ::Deployment::StopError (name,
                                       ex._info ().c_str ());
      }
    catch (...)
      {
        CIAO_ERROR (1, (LM_ERROR, CLINFO
                        "Homed_Component_Handler_i::remove_instance - "
                        "Caught C++ exception while removing component <%C>\n",
                        name));
        throw ::Deployment::StopError (name,
                                       "Unknown C++ exception");
      }
    
    CIAO_DEBUG (4, (LM_INFO, CLINFO
                    "Homed_Component_Handler_i::remove_instance - "
                    "Component <%C> successfully removed\n"));
    
    this->instances_.erase (instance);
    
    DEPLOYMENT_STATE::instance ()->remove_component (name);
  }
  
  void 
  Homed_Component_Handler_i::provide_endpoint_reference (const ::Deployment::DeploymentPlan & plan,
                                                         ::CORBA::ULong connectionRef,
                                                         ::CORBA::Any_out endpoint_reference)
  {
    CIAO_TRACE ("Homed_Component_Handler_i::provide_endpoint_reference");
    
    DEPLOYMENT_STATE::instance ()->connection_handler.provide_endpoint_reference (plan,
                                                                                  connectionRef,
                                                                                  endpoint_reference);
  }
    
  void
  Homed_Component_Handler_i::connect_instance (const ::Deployment::DeploymentPlan & plan,
                                               ::CORBA::ULong connectionRef,
                                               const ::CORBA::Any & provided_reference)
  {
    CIAO_TRACE ("Homed_Component_Handler_i::connect_instance");
    
    DEPLOYMENT_STATE::instance ()->connection_handler.connect_instance (plan,
                                                                        connectionRef,
                                                                        provided_reference);
  }
  
    
  void
  Homed_Component_Handler_i::disconnect_instance (const ::Deployment::DeploymentPlan & plan,
                                                  ::CORBA::ULong connectionRef)
  {
    CIAO_TRACE ("Homed_Component_Handler_i::disconnect_instance");
    DEPLOYMENT_STATE::instance ()->connection_handler.disconnect_instance (plan,
                                                                           connectionRef);
  }
  
  void
  Homed_Component_Handler_i::instance_configured (const ::Deployment::DeploymentPlan & plan,
                                            ::CORBA::ULong instanceRef)
  {
    CIAO_TRACE ("Homed_Component_Handler_i::instance_configured");
    
    Components::CCMObject_var ref = 
      DEPLOYMENT_STATE::instance ()->fetch_component (plan.instance[instanceRef].name.in ());
    
    try
      {
        ref->configuration_complete ();
      }
    catch (CORBA::Exception &ex)
      {
        CIAO_ERROR (1, (LM_ERROR, CLINFO
                        "Homed_Component_Handler_i::instance_configured - "
                        "Caught CORBA Exception: %C\n",
                        ex._info ().c_str ()));
        throw ::Deployment::StartError (plan.instance[instanceRef].name.in (),
                                        ex._info ().c_str ());
      }
    catch (...)
      {
        CIAO_ERROR (1, (LM_ERROR, CLINFO
                        "Homed_Component_Handler_i::instance_configured - "
                        "Caught C++ Exception\n"));
        throw ::Deployment::StartError (plan.instance[instanceRef].name.in (),
                                        "Unknown C++ exception\n");
                        
      }    
  }

  void
  Homed_Component_Handler_i::configure (const ::Deployment::Properties&)
  {
    
  }
}

extern "C"
{
  ::DAnCE::InstanceDeploymentHandler_ptr 
  CIAO_Locality_Handler_Export create_Homed_Component_Handler (void)
  {
    return new CIAO::Homed_Component_Handler_i ();
  }
}
