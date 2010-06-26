// -*- C++ -*-
// $Id$

/**
 * @file DAnCE_DeploymentInterceptors_Base_Impl.h
 *
 * Base, no-op interceptor.  
 **/

// TAO_IDL - Generated from 
// be/be_codegen.cpp:1571

#ifndef DANCE_DEPLOYMENTINTERCEPTORSI_DHYD9F_H_
#define DANCE_DEPLOYMENTINTERCEPTORSI_DHYD9F_H_

#include "DAnCE/DAnCE_DeploymentInterceptorsC.h"
#include "DAnCE/DAnCE_stub_export.h"
#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/LocalObject.h"

namespace DAnCE
{
  /**
   * @class DAnCE_DeploymentInterceptor_Base
   * @brief Base, no-op interceptor implementation. 
   */
  class DANCE_STUB_Export DeploymentInterceptor_Base
    : public virtual DAnCE::DeploymentInterceptor,
      public virtual ::CORBA::LocalObject
  {
  public:
    // Constructor 
    DeploymentInterceptor_Base (void);
  
    // Destructor 
    virtual ~DeploymentInterceptor_Base (void);
  
    virtual
      void configure (
                      const ::Deployment::Properties & config);
  
    virtual
      void preprocess_plan (
                            ::Deployment::DeploymentPlan & plan);
  
    virtual
      void instance_pre_install (
                                 ::Deployment::DeploymentPlan & plan,
                                 ::CORBA::ULong instanceRef);
  
    virtual
      void instance_post_install (
                                  const ::Deployment::DeploymentPlan & plan,
                                  ::CORBA::ULong instance_index,
                                  const ::CORBA::Any & instance_reference,
                                  const ::CORBA::Any & exception_thrown);

    virtual
      void post_endpoint_reference (
                                    const ::Deployment::DeploymentPlan & plan,
                                    ::CORBA::ULong connectionRef,
                                    const ::CORBA::Any & endpoint_reference,
                                    const ::CORBA::Any & exception_thrown);
    
    virtual
      void instance_pre_connect (
                                 ::Deployment::DeploymentPlan & plan,
                                 ::CORBA::ULong connection_index,
                                 ::CORBA::Any & provided_reference);
  
    virtual
      void instance_post_connect (
                                  const ::Deployment::DeploymentPlan & plan,
                                  ::CORBA::ULong connectionRef,
                                  const ::CORBA::Any & exceptionThrown);

    virtual
      void instance_post_configured (
                                     const ::Deployment::DeploymentPlan & plan,
                                     ::CORBA::ULong instanceRef,
                                     const ::CORBA::Any & exception_thrown);
  
    virtual
      void instance_post_activate (
                                   const ::Deployment::DeploymentPlan & plan,
                                   ::CORBA::ULong instanceRef,
                                   const ::CORBA::Any & exception_thrown);
  
    virtual
      void instance_post_passivate (
                                    const ::Deployment::DeploymentPlan & plan,
                                    ::CORBA::ULong instanceRef,
                                    const ::CORBA::Any & exception_thrown);
  
    virtual
      void instance_post_remove (
                                 const ::Deployment::DeploymentPlan & plan,
                                 ::CORBA::ULong instanceRef,
                                 const ::CORBA::Any & exception_thrown);
  };
}


#endif /* DANCE_DEPLOYMENTINTERCEPTORSI_H_  */
