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


#ifndef CIAO_ASSEMBLY_IMPL_H
#define CIAO_ASSEMBLY_IMPL_H
#include "ace/pre.h"

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Active_Map_Manager_T.h"
#include "CCM_DeploymentS.h"
#include "../XML_Helpers/Assembly_Spec.h"
#include "Deployment_Configuration.h"
#include "Assembly_Visitors.h"

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
  class AssemblyFactory_Impl
    : public virtual POA_Components::Deployment::AssemblyFactory,
      public virtual PortableServer::RefCountServantBase
  {
  public:
    /// Constructor
    AssemblyFactory_Impl (CORBA::ORB_ptr o,
                          PortableServer::POA_ptr p);

    /// Destructor
    virtual ~AssemblyFactory_Impl (void);

    /// Get the containing POA.  This operation does *not*
    /// increase the reference count of the POA.
    virtual PortableServer::POA_ptr _default_POA (void);

    /// Initialize the AssemblyFactory with a configuration file.
    /// @sa CIAO::Deployment_Configuration
    int init (const char *config_file
              ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

    /// Components::Deployment::Assembly defined attributes/operations.
    virtual ::Components::Cookie * create_assembly (const char * assembly_loc
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
    ACE_Active_Map_Manager<Components::Deployment::Assembly_var> assembly_map_;

    /// Deployment Configuration Info.
    CIAO::Deployment_Configuration deployment_config_;
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
  class Assembly_Impl
    : public virtual POA_Components::Deployment::Assembly,
      public virtual PortableServer::RefCountServantBase
  {
  public:
    /// Constructor
    Assembly_Impl (CORBA::ORB_ptr o,
                   PortableServer::POA_ptr p,
                   Assembly_Spec *spec,
                   Deployment_Configuration &config);

    /// Destructor
    virtual ~Assembly_Impl (void);

    /// Get the containing POA.  This operation does *not*
    /// increase the reference count of the POA.
    virtual PortableServer::POA_ptr _default_POA (void);

    /**
     * Initialize the Assembly with an Assembly_Spec and a reference
     * to a Deployment_Configuration object.  The Assembly class
     * assumes the ownership of Assembly_Spec.
     */
    int init (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

    /// Components::Deployment::Assembly defined attributes/operations.

    virtual void build (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       Components::CreateFailure));

    virtual void tear_down (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       Components::RemoveFailure));

    virtual ::Components::Deployment::AssemblyState
    get_state (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

  protected:
    /**
     * Connection establishment helper functions
     */
    /// @{
    void make_connection (Assembly_Connection::Connect_Info *info
                          ACE_ENV_ARG_DECL_WITH_DEFAULTS);
    CORBA::Object_ptr
    resolve_interface (Assembly_Connection::IF_Resolver_Info *info
                       ACE_ENV_ARG_DECL_WITH_DEFAULTS);
    Components::CCMObject_ptr
    resolve_component (Assembly_Connection::IF_Resolver_Info *info
                       ACE_ENV_ARG_DECL_WITH_DEFAULTS);
    Components::CCMHome_ptr
    resolve_home (Assembly_Connection::IF_Resolver_Info *info
                  ACE_ENV_ARG_DECL_WITH_DEFAULTS);
    Components::EventConsumerBase_ptr
    resolve_consumer (Assembly_Connection::IF_Resolver_Info *info
                      ACE_ENV_ARG_DECL_WITH_DEFAULTS);
    /// @}

    /// Keep a pointer to the managing ORB serving this servant.
    CORBA::ORB_var orb_;

    /// Keep a pointer to the managing POA.
    PortableServer::POA_var poa_;

    /// Current State
    ::Components::Deployment::AssemblyState state_;

    /// <Debug> My instance number.
    CORBA::ULong serial_number_;

    /// <Debug> instance counter.
    static CORBA::ULong assembly_count_;

    /// Deployment Configuration Info.
    Deployment_Configuration &deployment_config_;

    /// Assembly Specification.
    Assembly_Spec *assembly_spec_;

    Assembly_Context assembly_context_;
  };
}

#if defined (__ACE_INLINE__)
# include "Assembly_Impl.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* CIAO_ASSEMBLY_IMPL_H */
