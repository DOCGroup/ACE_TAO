// -*- C++ -*-

//=============================================================================
/**
 *  @file    Assembly_Impl.h
 *
 *  $Id$
 *
 *  This file contains implementations for
 *  Components::Deployment::AssemblyFactory and
 *  Components::Deployment::Assembly interface.
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
   * @class AssemblyFactory_Impl
   *
   * @brief Servant implementation for Components::Deployment::AssemblyFactory
   *
   * This class implements the Components::Deployment::AssemblyFactory
   * interface as defined by the CCM spcification.  As the interface
   * implies, this is actually part of the deployment interface and is
   * used to manage the lifecycle of containers running on the server.
   */
  class CIAO_SERVER_Export AssemblyFactory_Impl
    : public virtual POA_Components::Deployment::AssemblyFactory,
      public virtual PortableServer::RefCountServantBase
  {
  public:
    /// Constructor
    AssemblyFactory_Impl (CORBA::ORB_ptr o);

    /// Destructor
    virtual ~AssemblyFactory_Impl (void);

    /// Get the containing POA.  This operation does *not*
    /// increase the reference count of the POA.
    virtual PortableServer::POA_ptr _default_POA (void);

    /// Initialize the AssemblyFactory.
    int init (ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

    /// Components::Deployment::Assembly defined attributes/operations.
    virtual ::Components::Cookie * create (const char * assembly_loc
                                           ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       Components::Deployment::InvalidLocation,
                       Components::CreateFailure));

    virtual ::Components::Deployment::Assembly_ptr lookup (Components::Cookie * c
                                                           ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       Components::Deployment::InvalidAssembly));

    virtual void destroy (Components::Cookie * c
                          ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       Components::Deployment::InvalidAssembly,
                       Components::RemoveFailure));

  protected:
    /// Keep a pointer to the managing ORB serving this servant.
    CORBA::ORB_var orb_;

    /// Keep a pointer to the managing POA.
    PortableServer::POA_var poa_;

    /// Keep a list of managed Assembly objects.
    CIAO::Active_Objref_Map assembly_map_;
  };

  /**
   * @class Assembly_Impl
   *
   * @brief Servant implementation for Components::Deployment::Assembly
   *
   * This class implements the Components::Deployment::Assembly
   * interface as defined by the CCM spcification.  As the interface
   * implies, this is actually part of the deployment interface and is
   * used to manage the lifecycle of containers running on the server.
   */
  class CIAO_SERVER_Export Assembly_Impl
    : public virtual POA_Components::Deployment::Assembly,
      public virtual PortableServer::RefCountServantBase
  {
  public:
    /// Constructor
    Assembly_Impl (CORBA::ORB_ptr o);

    /// Destructor
    virtual ~Assembly_Impl (void);

    /// Get the containing POA.  This operation does *not*
    /// increase the reference count of the POA.
    virtual PortableServer::POA_ptr _default_POA (void);

    /// Initialize the Assembly.
    int init (ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

    /// Components::Deployment::Assembly defined attributes/operations.

    virtual void build (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

    virtual void tear_down (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       Components::RemoveFailure));

    virtual ::Components::Deployment::AssemblyState
    get_state (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

  protected:
    /// Keep a pointer to the managing ORB serving this servant.
    CORBA::ORB_var orb_;

    /// Keep a pointer to the managing POA.
    PortableServer::POA_var poa_;
  };
}

#if defined (__ACE_INLINE__)
# include "Assembly_Impl.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* CIAO_SERVERACTIVATOR_IMPL_H */
