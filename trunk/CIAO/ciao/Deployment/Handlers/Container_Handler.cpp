// $Id$

#include "Container_Handler.h"
#include "ciao/Logger/Log_Macros.h"
#include "ciao/Containers/Session/Session_Container.h"
#include "ciao/ComponentServer/Server_init.h"

#include "CIAO_State.h"

namespace CIAO
{
  Container_Handler_i::Container_Handler_i (const DAnCE::Utility::PROPERTY_MAP &,
                                            CORBA::ORB_ptr orb,
                                            PortableServer::POA_ptr poa)
    : orb_ (CORBA::ORB::_duplicate (orb)),
      poa_ (PortableServer::POA::_duplicate (poa))
            
  {
    CIAO_TRACE ("Container_Handler_i::Container_Handler_i");
    CIAO::Server_init (orb_);
    
    // For the time being, we are going to go ahead and construct a container.
    ::Deployment::DeploymentPlan plan;
    plan.instance.length (1);
    plan.instance[0].name = "";
    
    ::CORBA::Any_var any;
    this->install_instance (plan, 0, any.out ());
  }
  
  // Destructor 
  Container_Handler_i::~Container_Handler_i (void)
  {
    CIAO_TRACE ("Container_Handler_i::~Container_Handler_i");
    
    try
      {
        // For the time being, we are going to go ahead and delete the default container
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
}

