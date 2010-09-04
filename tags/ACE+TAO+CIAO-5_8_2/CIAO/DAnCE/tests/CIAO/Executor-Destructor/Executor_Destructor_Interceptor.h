// -*- C++ -*-
// $Id$

/**
 * @file Executor_Destructor_Interceptor.h
 *
 * Base, no-op interceptor.  
 **/

// TAO_IDL - Generated from 
// be/be_codegen.cpp:1571

#ifndef EXEC_DEST_INT
#define EXEC_DEST_INT

#include "Exec_Dest_export.h"

#include "DAnCE/DAnCE_DeploymentInterceptorsC.h"
#include "DAnCE/DAnCE_DeploymentInterceptors_Base_Impl.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/LocalObject.h"

#include <set>
#include <string>

namespace DAnCE
{
  /**
   * @class DAnCE_Executor_Destructor
   * @brief Base, no-op interceptor implementation. 
   */
  class Executor_Destructor_Interceptor_Export Executor_Destructor
    : public virtual ::DAnCE::DeploymentInterceptor_Base
  {
  public:
    // Constructor 
    Executor_Destructor (void);
  
    // Destructor 
    virtual ~Executor_Destructor (void);
  
    virtual
      void configure (const ::Deployment::Properties & config);
  
    virtual
      void preprocess_plan (::Deployment::DeploymentPlan & plan);
  
    virtual
      void pre_install (::Deployment::DeploymentPlan & plan,
                        ::CORBA::ULong instanceRef);
      
    virtual
      void post_configured (const ::Deployment::DeploymentPlan & plan,
                            ::CORBA::ULong instanceRef,
                            const ::CORBA::Any & exception_thrown);
  
    virtual
      void post_activate (const ::Deployment::DeploymentPlan & plan,
                          ::CORBA::ULong instanceRef,
                          const ::CORBA::Any & exception_thrown);
  
    virtual
      void post_passivate (const ::Deployment::DeploymentPlan & plan,
                           ::CORBA::ULong instanceRef,
                           const ::CORBA::Any & exception_thrown);
  
    virtual
      void post_remove (const ::Deployment::DeploymentPlan & plan,
                        ::CORBA::ULong instanceRef,
                        const ::CORBA::Any & exception_thrown);
    
  private:
    std::set < std::string > instance_ids_;
  };
}

extern "C"
{
  ::DAnCE::DeploymentInterceptor_ptr
  Executor_Destructor_Interceptor_Export create_Executor_Destructor (void);
}


#endif /* DANCE_DEPLOYMENTINTERCEPTORSI_H_  */
