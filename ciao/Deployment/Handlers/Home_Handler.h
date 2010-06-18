
// -*- C++ -*-
// $Id$

/**
 * @file Home_Handler.h
 * @author William R. Otte <wotte@dre.vanderbilt.edu>
 * 
 * Installation handler implementation for spawning LocalityManagers. 
 */

#ifndef CIAO_HOME_HANDLER_H
#define CIAO_HOME_HANDLER_H

#include "DAnCE/DAnCE_LocalityManagerS.h"
#include "DAnCE/DAnCE_Utility.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ciao/Deployment/Handlers/CIAO_Locality_Handler_Export.h"
#include "tao/LocalObject.h"

#include "ciao/Deployment/Handlers/CIAO_Handler_Common.h"

namespace CIAO
{
  class CIAO_Locality_Handler_Export Home_Handler_i
    : public virtual DAnCE::InstanceDeploymentHandler,
      public virtual ::CORBA::LocalObject
  {
  public:
    // Constructor 
    Home_Handler_i (void);
  
    // Destructor 
    virtual ~Home_Handler_i (void);
    
    virtual
      char * instance_type (void);
  
    virtual
      void install_instance (const ::Deployment::DeploymentPlan & plan,
                             ::CORBA::ULong instanceRef,
                             ::CORBA::Any_out instance_reference);
    
    virtual
      void activate_instance (const ::Deployment::DeploymentPlan & ,
                              ::CORBA::ULong ,
                              const ::CORBA::Any &);
    virtual
      void passivate_instance (const ::Deployment::DeploymentPlan & ,
                               ::CORBA::ULong ,
                               const ::CORBA::Any &);

    virtual
      void remove_instance (const ::Deployment::DeploymentPlan & plan,
                            ::CORBA::ULong instanceRef,
                            const ::CORBA::Any & instance_reference);

    virtual 
      void provide_endpoint_reference (const ::Deployment::DeploymentPlan & plan,
                                       ::CORBA::ULong connectionRef,
                                       ::CORBA::Any_out endpoint_reference);  

    virtual
      void connect_instance (const ::Deployment::DeploymentPlan & plan,
                             ::CORBA::ULong connectionRef,
                             const ::CORBA::Any & provided_reference);
  
    virtual
      void disconnect_instance (const ::Deployment::DeploymentPlan & plan,
                                ::CORBA::ULong connectionRef);

    virtual 
      void instance_configured (const ::Deployment::DeploymentPlan & plan,
                                ::CORBA::ULong instanceRef);    

    virtual
      void configure(const Deployment::Properties&);

  private:
    Deployment_Common::INSTANCES instances_;
  };
}
#endif
