// -*- C++ -*-

//=============================================================================
/**
 *  @file    Assembly_Service_Impl.h
 *
 *  $Id$
 *
 *  @author Nanbor Wang <nanbor@cs.wustl.edu>
 */
//=============================================================================


#ifndef CIAO_ASSEMBLY_SERVICE_IMPL_H
#define CIAO_ASSEMBLY_SERVICE_IMPL_H
#include "ace/pre.h"

#include "Assembly_ServiceS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

namespace CIAO
{
  class Assembly_Service_Impl
    : public virtual POA_CIAO::Assembly_Service,
      public virtual PortableServer::RefCountServantBase
  {
  public:
    /// Constructor
    Assembly_Service_Impl (CORBA::ORB_ptr o,
                           PortableServer::POA_ptr p,
                           Components::Deployment::AssemblyFactory_ptr f);

    /// Destructor
    virtual ~Assembly_Service_Impl (void);

    /// Get the containing POA.  This operation does *not*
    /// increase the reference count of the POA.
    virtual PortableServer::POA_ptr _default_POA (void);

    virtual ::Components::Deployment::AssemblyFactory_ptr
    get_assemblyfactory (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

    virtual void shutdown (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

  protected:
    /// Keep a pointer to the managing ORB serving this servant.
    CORBA::ORB_var orb_;

    /// Keep a pointer to the managing POA.
    PortableServer::POA_var poa_;

    /// Cached AssemblyFactory reference.
    Components::Deployment::AssemblyFactory_var factory_;
  };
}

#include "ace/post.h"
#endif /* CIAO_ASSEMBLY_IMPL_H */
