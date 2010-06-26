// $Id$

#include "Container_Handler.h"
#include "tao/ORB_Core.h"
#include "ciao/Logger/Log_Macros.h"
#include "ciao/Containers/Session/Session_Container.h"
#include "ciao/Base/Server_init.h"
#include "DAnCE/DAnCE_Utility.h"
#include "DAnCE/DAnCE_PropertiesC.h"
#include "CIAO_State.h"

namespace CIAO
{
  Container_Handler_i::Container_Handler_i (void)
  {
    CIAO_TRACE ("Container_Handler_i::Container_Handler_i");
  }
  
  // Destructor 
  Container_Handler_i::~Container_Handler_i (void)
  {
    CIAO_TRACE ("Container_Handler_i::~Container_Handler_i");
    
    try
      {
        // For the time being, we are going to go ahead and delete
        // the default container
        ::Deployment::DeploymentPlan plan;
        plan.instance.length (1);
        plan.instance[0].name = "";
        
        CORBA::Any any;
        this->remove_instance (plan, 0, any);
      }
    catch (...)
      { //swallow
      }
  }
    
  char * 
  Container_Handler_i::instance_type (void)
  {
    CIAO_TRACE ("Container_Handler_i::instance_type");
    return CORBA::string_dup ("edu.dre.vanderbilt.dre.DAnCE.CCM.Container");
  }
    
  void
  Container_Handler_i::install_instance (const ::Deployment::DeploymentPlan &plan,
                                         ::CORBA::ULong instanceRef,
                                         ::CORBA::Any_out instance_reference)
  {
    CIAO_TRACE ("Container_Handler_i::install_instance");
    
    CIAO::Session_Container *cont (0);
    const char *name (plan.instance[instanceRef].name.in ());
    CORBA::PolicyList policies (0);
    
    CIAO_DEBUG (6, (LM_DEBUG, CLINFO 
                    "Container_Handler_i::install_instance - "
                    "Creating container with id %C\n",
                    name));
    ACE_NEW_THROW_EX (cont,
                      CIAO::Session_Container (this->orb_,
                                               this->poa_,
                                               0 /* not needed */,
                                               false /* we're always dynanic for the moment */,
                                               0, /*always dynanic */
                                               name,
                                               0 /* no additional policies at this moment */),
                      CORBA::NO_MEMORY ());
    
    CIAO_DEBUG (8, (LM_DEBUG, CLINFO 
                    "Container_Handler_i::install_instance - "
                    "Container <%C> successfully created\n",
                    name));

    CIAO::Container_var container_ref (cont);

    DEPLOYMENT_STATE::instance ()->add_container (name,
                                                  cont);
    
    ::CORBA::Any_ptr outref;
    ACE_NEW_THROW_EX (outref,
                      ::CORBA::Any (),
                      CORBA::NO_MEMORY ());
    
    (*outref) <<= container_ref;
    
    instance_reference = outref;
  }
  
  void
  Container_Handler_i::activate_instance (const ::Deployment::DeploymentPlan & ,
                                     ::CORBA::ULong ,
                                     const ::CORBA::Any &)
  {
    CIAO_TRACE ("Container_Handler_i::activate_instance");
    // no activation needed.
  }

  void
  Container_Handler_i::passivate_instance (const ::Deployment::DeploymentPlan & ,
                                      ::CORBA::ULong ,
                                      const ::CORBA::Any &)
  {
    CIAO_TRACE ("Container_Handler_i::passivate_instance");
    // no passivation needed.
  }
    
  void 
  Container_Handler_i::provide_endpoint_reference (const ::Deployment::DeploymentPlan &,
                                                   ::CORBA::ULong,
                                                   ::CORBA::Any_out)
  {
    CIAO_TRACE ("Container_Handler_i::provide_endpoint_reference");
    CIAO_ERROR (1, (LM_ERROR, CLINFO 
                    "Container_Handler_i::provide_endpoint_reference - "
                    "Unable to provide any endpoints.\n"));
    throw CORBA::NO_IMPLEMENT ();
  }

  void
  Container_Handler_i::remove_instance (const ::Deployment::DeploymentPlan & plan,
                                        ::CORBA::ULong instanceRef,
                                        const ::CORBA::Any &)
  {
    CIAO_TRACE ("Container_Handler_i::remove_instance");
    
    const char *name = plan.instance[instanceRef].name.in ();

    ::CIAO::Container_var cont = 
        DEPLOYMENT_STATE::instance ()->fetch_container (name);
    
    if (!cont)
      {
        CIAO_ERROR (1, (LM_ERROR, CLINFO,
                        "Container_Handler_i::remove_instance - "
                        "Error, no contianer with id <%C>",
                        name));
        throw ::Deployment::StopError (name,
                                       "No container with ID");
      }
       
    
    CIAO_DEBUG (8, (LM_TRACE, CLINFO
                    "Container_Handler_i::remove_instance - "
                    "Removing container with Id <%C>\n",
                    name));
    
    DEPLOYMENT_STATE::instance ()->remove_container (name);
    
    CIAO_DEBUG (5, (LM_TRACE, CLINFO
                    "Container_Handler_i::remove_instance - "
                    "Container with Id <%C> removed.\n",
                    name));
  }
    
  void
  Container_Handler_i::connect_instance (const ::Deployment::DeploymentPlan &,
                                         ::CORBA::ULong,
                                         const ::CORBA::Any &)
  {
    CIAO_TRACE ("Container_Handler_i::connect_instance");
    
    CIAO_ERROR (1, (LM_ERROR, CLINFO
                    "Container_Handler_i::connect_instance - ",
                    "No connections allowed for containers.\n"));
    throw CORBA::NO_IMPLEMENT ();
  }
  
    
  void
  Container_Handler_i::disconnect_instance (const ::Deployment::DeploymentPlan &,
                                            ::CORBA::ULong)
  {
    CIAO_TRACE ("Container_Handler_i::disconnect_instance");
    
    CIAO_ERROR (1, (LM_ERROR, CLINFO
                    "Container_Handler_i::disconnect_instance - ",
                    "No connections allowed for containers.\n"));
    throw CORBA::NO_IMPLEMENT ();
  }
  
  void
  Container_Handler_i::instance_configured (const ::Deployment::DeploymentPlan &,
                                            ::CORBA::ULong)
  {
    CIAO_TRACE ("Container_Handler_i::instance_configured");
    // nothing to be done
  }

  void
  Container_Handler_i::configure (const ::Deployment::Properties &props )
  {
    CIAO_DEBUG (6, (LM_DEBUG, CLINFO
                    "Container_Handler_i::configure - "
                    "Received %u properties for configuration\n",
                    props.length ()));

    DAnCE::Utility::get_property_value (DAnCE::ENTITY_POA,
                                        props,
                                        this->poa_);
    
    if (CORBA::is_nil (this->poa_))
      {
        CIAO_ERROR (1, (LM_ERROR, CLINFO
                        "Container_Handler_i::configure -"
                        "Unable to locate POA.\n"));
        throw ::Deployment::StartError ("CIAO Container Handler",
                                        "Unable to locate POA");
      }
    
    this->orb_ = TAO_ORB_Core_instance ()->orb ();
    
    if (CORBA::is_nil (this->orb_))
      {
        CIAO_ERROR (1, (LM_ERROR, CLINFO
                        "Container_Handler_i::configure -"
                        "Unable to locate ORB.\n"));
        throw ::Deployment::StartError ("CIAO Container Handler",
                                        "Unable to locate ORB");
      }
    
    CIAO::Server_init (orb_);
    
    // For the time being, we are going to go ahead and construct a container.
    ::Deployment::DeploymentPlan plan;
    plan.instance.length (1);
    plan.instance[0].name = "";
    
    ::CORBA::Any_var any;
    this->install_instance (plan, 0, any.out ());
  }
}

extern "C"
{
  ::DAnCE::InstanceDeploymentHandler_ptr 
  CIAO_Locality_Handler_Export create_Container_Handler (void)
  {
    return new CIAO::Container_Handler_i ();
  }
}
