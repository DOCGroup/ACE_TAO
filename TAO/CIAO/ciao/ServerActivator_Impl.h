// -*- C++ -*-

//=============================================================================
/**
 *  @file    ServerActivator_Impl.h
 *
 *  $Id$
 *
 *  This file contains implementation for the servant of
 *  Components::Deployment::ServerActivator interface.
 *
 *  @author Nanbor Wang <nanbor@cs.wustl.edu>
 */
//=============================================================================


#ifndef CIAO_SERVERACTIVATOR_IMPL_H
#define CIAO_SERVERACTIVATOR_IMPL_H
#include "ace/pre.h"

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "CCM_DeploymentS.h"

namespace CIAO
{
  /**
   * @class ServerActivator_Impl
   *
   * @brief Servant implementation for Components::Deployment::ServerActivator
   *
   * This class implements the Components::Deployment::ServerActivator
   * interface as defined by the CCM spcification.  As the interface
   * implies, this is actually part of the deployment interface and is
   * used to manage the lifecycle of containers running on the server.
   */
  class CIAO_SERVER_Export ServerActivator_Impl
    : public virtual POA_Components::Deployment::ServerActivator,
      public virtual PortableServer::RefCountServantBase
  {
  public:
    /// Constructor
    ServerActivator_Impl (CORBA::ORB_ptr o);

    /// Destructor
    virtual ~ServerActivator_Impl (void);

    /// Get the containing POA.  This operation does *not*
    /// increase the reference count of the POA.
    virtual PortableServer::POA_ptr _default_POA (void);

    /// Initialize the ServerActivator.
    int init (ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

    /// Components::Deployment::ServerActivator defined attributes/operations.

    virtual ::Components::Deployment::ComponentServer_ptr
    create_component_server (const Components::ConfigValues & config
                             ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       Components::CreateFailure,
                       Components::InvalidConfiguration));
    virtual void remove_component_server (Components::Deployment::ComponentServer_ptr server
                                          ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       Components::RemoveFailure));

    virtual ::Components::Deployment::ComponentServers *
    get_component_servers (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

  protected:
    /// Keep a pointer to the managing ORB serving this servant.
    CORBA::ORB_var orb_;

    /// Keep a pointer to the managing POA.
    PortableServer::POA_var poa_;

    /// Keep a list of managed ComponentServer objects.
    Object_Set<::Components::Deployment::ComponentServer> cs_set_;
  };
}

#if defined (__ACE_INLINE__)
# include "ServerActivator_Impl.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* CIAO_SERVERACTIVATOR_IMPL_H */
