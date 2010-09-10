// $Id$
/**
 * @file Standard_Error.h
 * @author William R. Otte <wotte@dre.vanderbilt.edu>
 *
 * Interceptors which implement a sandard (first failure) error handling mechanism.
 */

#ifndef DEPLOYMENT_INTERCEPTORS_H
#define DEPLOYMENT_INTERCEPTORS_H

#include "DAnCE/DAnCE_DeploymentInterceptors_Base_Impl.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/LocalObject.h"
#include "DAnCE_Error_Interceptors_export.h"

namespace DAnCE
{
  class DAnCE_Error_Interceptors_Export Standard_Error
    : public virtual DAnCE::DeploymentInterceptor_Base
  {
  public:
    /// Constructor
    Standard_Error (void);

    /// Destructor
    virtual ~Standard_Error (void);

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
  };
}

extern "C"
{
  ::DAnCE::DeploymentInterceptor_ptr
  DAnCE_Error_Interceptors_Export create_DAnCE_Standard_Error (void);
}

#endif
