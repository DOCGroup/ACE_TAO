// -*- C++ -*-
// $Id$

/**
 * @file Locality_Manager_Handler_Impl.h
 * @author William R. Otte <wotte@dre.vanderbilt.edu>
 * 
 * Installation handler implementation for spawning LocalityManagers. 
 */

#ifndef DANCE_LOCALITYMANAGERI_MIGAYG_H_
#define DANCE_LOCALITYMANAGERI_MIGAYG_H_

#include "DAnCE/DAnCE_LocalityManagerS.h"
#include "DAnCE/DAnCE_Utility.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "LocalityManager/Handler/DAnCE_Locality_Handler_export.h"
#include "tao/LocalObject.h"

namespace DAnCE
{
  class DAnCE_LocalityActivator_i;

  class DAnCE_Locality_Handler_Export Locality_Handler_i
    : public virtual DAnCE::InstanceDeploymentHandler,
      public virtual ::CORBA::LocalObject
  {
  public:
    // Constructor 
    Locality_Handler_i (void);
  
    // Destructor 
    virtual ~Locality_Handler_i (void);
    
    virtual
      char * instance_type (void);
  
    virtual
      void install_instance (const ::Deployment::DeploymentPlan & plan,
                             ::CORBA::ULong instanceRef,
                             ::CORBA::Any_out instance_reference);
    
    virtual
      void activate_instance (const ::Deployment::DeploymentPlan & ,
                              ::CORBA::ULong ,
                              const ::CORBA::Any &) {};
    virtual
      void passivate_instance (const ::Deployment::DeploymentPlan & ,
                               ::CORBA::ULong ,
                               const ::CORBA::Any &) {};

    virtual
      void remove_instance (const ::Deployment::DeploymentPlan & plan,
                            ::CORBA::ULong instanceRef,
                            const ::CORBA::Any & instance_reference);

    virtual 
      void provide_endpoint_reference (const ::Deployment::DeploymentPlan &,
                                       ::CORBA::ULong,
                                       ::CORBA::Any_out) {};
  
    virtual
      void connect_instance (const ::Deployment::DeploymentPlan & plan,
                             ::CORBA::ULong connectionRef,
                             const ::CORBA::Any & provided_reference);
  
    virtual
      void disconnect_instance (const ::Deployment::DeploymentPlan & plan,
                                ::CORBA::ULong connectionRef);

    virtual 
      void instance_configured (const ::Deployment::DeploymentPlan &,
                                ::CORBA::ULong) {};    

    virtual
      void configure(const Deployment::Properties&);
    
    virtual void close (void);
     
  private:
    static const char *instance_type_;
    DAnCE_LocalityActivator_i *activator_;
  };
}

extern "C"
{
  ::DAnCE::InstanceDeploymentHandler_ptr 
  DAnCE_Locality_Handler_Export create_Locality_Handler (void);
}

#endif
