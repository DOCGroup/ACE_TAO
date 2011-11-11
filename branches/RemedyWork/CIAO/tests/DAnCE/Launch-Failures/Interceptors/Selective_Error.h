// $Id$
/**
 * @file Selective_Error.h
 * @author William R. Otte <wotte@dre.vanderbilt.edu>
 *
 * Interceptors which implement a selective error handling mechanism.
 */

#ifndef LAUNCHERROR_INTERCEPTORS_H
#define LAUNCHERROR_INTERCEPTORS_H

#include "dance/DAnCE_DeploymentInterceptors_Base_Impl.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/LocalObject.h"
#include "Selective_Error_Export.h"

namespace DAnCE
{
  class Selective_Error_Export Selective_Error
    : public virtual DAnCE::DeploymentInterceptor_Base
  {
  public:
    // Constructor

    Selective_Error (void);

    // Destructor
    virtual ~Selective_Error (void);

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
  Selective_Error_Export create_DAnCE_Selective_Error (void);
}

#endif
