// -*- C++ -*-

//=============================================================================
/**
 *  @file    Daemon_Impl.h
 *
 *  $Id$
 *
 *  This file contains servant implementation for Deployment:NodeManager
 *  interface. In the current design of the NodeManager, as with the legacy
 *  implementation of CIAO, Each NodeManager corresponds to ONE NodeApplication
 *  Manager. Though, the name intuitively suggests that there be one NodeManager
 *  for every node, our design, allows the end-user to have multiple components
 *  run on the same node.
 *
 *  @author Arvind S. Krishna <arvindk@dre.vanderbilt.edu>
 */
//=============================================================================


#ifndef CIAO_NODEDAEMON_IMPL_H
#define CIAO_NODEDAEMON_IMPL_H
#include /**/ "ace/pre.h"

#include "NodeDaemonS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */


namespace CIAO
{
  /**
   * @class NodeDaemon_Impl
   *
   * @brief Servant implementation CIAO's daemon process control interface
   *
   * This class implements the CIAO:NodeDaemon interface.
   *
   */
  class NodeDaemon_Impl
    : public virtual POA_CIAO::NodeDaemon,
      public virtual PortableServer::RefCountServantBase
  {
  public:

    /// Constructor
    NodeDaemon_Impl (const char *name,
                     CORBA::ORB_ptr orb,
                     PortableServer::POA_ptr p,
                     const char * nodeapp_loc,
                     int spawn_delay
                     ACE_ENV_ARG_DECL_WITH_DEFAULTS)
		ACE_THROW_SPEC ((CORBA::SystemException));
    /// Destructor
    virtual ~NodeDaemon_Impl (void);


    /// Get the containing POA.  This operation does *not*
    /// increase the reference count of the POA.
    virtual PortableServer::POA_ptr _default_POA (void);

    /// CIAO::Daemon defined attributes/operations.
    virtual char * name (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

    virtual void shutdown (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

    virtual void joinDomain (const Deployment::Domain & domain,
                             Deployment::TargetManager_ptr manager,
                             Deployment::Logger_ptr log
                             ACE_ENV_ARG_DECL_WITH_DEFAULTS
                             )
      ACE_THROW_SPEC ((CORBA::SystemException));

    virtual void leaveDomain (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

    virtual ::Deployment::NodeApplicationManager_ptr
    preparePlan (const Deployment::DeploymentPlan &plan
                 ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       Deployment::StartError,
                       Deployment::PlanError));

    virtual void
    destroyManager (Deployment::NodeApplicationManager_ptr appManager
                    ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       Deployment::StopError));

  protected:
    // Keep a pointer to the managing ORB serving this servant.
    CORBA::ORB_var orb_;

    // Keep a pointer to the managing POA.
    PortableServer::POA_var poa_;

    // My Canonical name.
    CORBA::String_var name_;

    // NodeApplication location
    CORBA::String_var nodeapp_location_;

    // Cached callback POA.
    PortableServer::POA_var callback_poa_;

    // Spawn delay for the NodeAppMgr
    int spawn_delay_;

    // Cache reference of last NodeAppManager
    Deployment::NodeApplicationManager_var manager_;

  };
}
#if defined (__ACE_INLINE__)
# include "NodeDaemon_Impl.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif /* CIAO_NODEDAEMON_IMPL_H */
