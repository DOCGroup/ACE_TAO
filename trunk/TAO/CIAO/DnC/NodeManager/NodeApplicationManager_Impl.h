// -*- C++ -*-

//=============================================================================
/**
 *  @file NodeApplicationManager_Impl.h
 *
 *  $Id$
 *
 *  This file contains implementation for the servant of
 *  Deployment::NodeApplicationManager. 
 *  @author Arvind S. Krishna <arvindk@dre.vanderbilt.edu>
 */
//=============================================================================


#ifndef CIAO_NODEAPPLICATIONMANAGER_IMPL_H
#define CIAO_NODEAPPLICATIONMANAGER_IMPL_H
#include /**/ "ace/pre.h"

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "DeploymentS.h"

namespace CIAO
{
  /**
   * @class NodeApplicationManager_Impl
   */

  class NodeApplicationManager_Impl
    : public virtual POA_Deployment::NodeApplicationManager,
      public virtual PortableServer::RefCountServantBase
  {
  public:
    /// Constructor
    NodeApplicationManager_Impl (CORBA::ORB_ptr o,
				 PortableServer::POA_ptr p);

    /// Destructor
    virtual ~NodeApplicationManager_Impl (void);

    //@@ The return type is NodeApplication_ptr actually.
    //   Mappings between configureVals and Properpies needs to be fixed. --Tao
    virtual
      Deployment::Application_ptr
      startLaunch (const Deployment::Properties &configProperty,
                   Deployment::Connections_out providedReference,
                   CORBA::Boolean start
                   ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
		       Deployment::ResourceNotAvailable,
		       Deployment::StartError,
		       Deployment::InvalidProperty));


    virtual void destroyApplication (Deployment::Application_ptr app
				     ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
		       Deployment::StopError));

    /// Get the containing POA.  This operation does *not*
    /// increase the reference count of the POA.
    virtual PortableServer::POA_ptr _default_POA (void);

  protected:
    /// Keep a pointer to the managing ORB serving this servant.
    CORBA::ORB_var orb_;

    /// Keep a pointer to the managing POA.
    PortableServer::POA_var poa_;

    /// Cache a object reference to this servant.
    Deployment::NodeApplicationManager_var objref_;

    /// Cache the ior of the previous reference
    CORBA::String_var ior_;

    /// Synchronize access to the object set.
    TAO_SYNCH_MUTEX lock_;
  };
}

#include /**/ "ace/post.h"
#endif /* CIAO_NODEAPPLICATIONMANAGER_IMPL_H */
