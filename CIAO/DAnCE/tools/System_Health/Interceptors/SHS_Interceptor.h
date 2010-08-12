// $Id$
/**
 * @file SHS_Interceptor.h
 * @author William R. Otte <wotte@dre.vanderbilt.edu>
 *
 * Interceptors which implement a Best Effort error handling mechanism.
 */

#ifndef DEPLOYMENT_INTERCEPTORS_H
#define DEPLOYMENT_INTERCEPTORS_H

#include "DAnCE/DAnCE_DeploymentInterceptors_Base_Impl.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Auto_Ptr.h"
#include "tao/LocalObject.h"
#include "DAnCE_SHS_Interceptors_export.h"

#include "System_Health/Interceptors/SHS_Transport.h"

namespace DAnCE
{
  class DAnCE_SHS_Interceptors_Export SHS_Interceptor
    : public virtual DAnCE::DeploymentInterceptor_Base
  {
  public:
    // Constructor 
    
    SHS_Interceptor (void);
  
    // Destructor 
    virtual ~SHS_Interceptor (void);
  
    virtual
      void post_install (const ::Deployment::DeploymentPlan & plan,
                                  ::CORBA::ULong index,
                                  const ::CORBA::Any & reference,
                                  const ::CORBA::Any & exception_thrown);

    virtual
      void post_connect (const ::Deployment::DeploymentPlan & plan,
                                  ::CORBA::ULong connectionRef,
                                  const ::CORBA::Any & exceptionThrown);


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
    virtual
      void configure(const Deployment::Properties&);
    
  private:
    auto_ptr <DAnCE::SHS::SHS_Transport> shs_transport_;
  };
}

extern "C"
{
  ::DAnCE::DeploymentInterceptor_ptr 
  DAnCE_SHS_Interceptors_Export create_DAnCE_SHS_Interceptor (void);
}

#endif
