// -*- C++ -*-

//=============================================================================
/**
 *  @file    Container_Impl.h
 *
 *  $Id$
 *
 *  This file contains implementation for the servant of
 *  Components::Deployment::Container interface.
 *
 *  @author Nanbor Wang <nanbor@cs.wustl.edu>
 */
//=============================================================================


#ifndef CIAO_CONTAINER_IMPL_H
#define CIAO_CONTAINER_IMPL_H
#include /**/ "ace/pre.h"

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "CCM_DeploymentS.h"
#include "Container_Base.h"
#include "Object_Set_T.h"

namespace CIAO
{

  struct home_installation_info
  {
    CORBA::String_var executor_dll_;

    //@@ I added executer_entrypt_ to do install home in a single step. --Tao
    CORBA::String_var executor_entrypt_;

    CORBA::String_var servant_dll_;

    //This is a bit awkward here, since the entrypt of the servant dll is
    //already defined in the CIDL generated code. Supposebably we can derive
    //this name from the executor name if we can follow some naming agreement.
    //                 --Tao
    CORBA::String_var servant_entrypt_;
  };

  /**
   * @class Container_Impl
   *
   * @brief Servant implementation for Components::Deployment::Container
   *
   * This class implements the Components::Deployment::Container
   * interface as defined by the CCM specification.  As the interface
   * implies, this is actually part of the deployment interface and is
   * used to manage the lifecycle of the installed components and
   * homes.
   */
  class CIAO_SERVER_Export Container_Impl
    : public virtual POA_Components::Deployment::Container,
      public virtual PortableServer::RefCountServantBase
  {
  public:
    /// Constructor
    Container_Impl (CORBA::ORB_ptr o,
                    PortableServer::POA_ptr p,
                    CORBA::Object_ptr nodeapp);

    /// Destructor
    virtual ~Container_Impl (void);

    /// Get the containing POA.  This operation does *not*
    /// increase the reference count of the POA.
    virtual PortableServer::POA_ptr _default_POA (void);

    /// Initialize the container with a name.
    int init (const ::Deployment::Properties &properties
              ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

    virtual ::Deployment::Properties *
    con_properties (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

    virtual ::CORBA::Object_ptr
    get_node_application (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

    virtual ::Components::CCMHome_ptr
    install_home (const ::Deployment::Properties  & properties
		  ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       Components::Deployment::UnknownImplId,
                       Components::Deployment::ImplEntryPointNotFound,
                       Components::Deployment::InstallationFailure,
                       Components::InvalidConfiguration));

    virtual void remove_home (Components::CCMHome_ptr href
                              ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       Components::RemoveFailure));

    virtual ::Components::CCMHomes *
    get_homes (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

    virtual void remove (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       Components::RemoveFailure));

    /// Set the cached object reference.
    void set_objref (Components::Deployment::Container_ptr o
                     ACE_ENV_ARG_DECL_WITH_DEFAULTS);

    /// Get the cached object reference.  This operation will invoke
    /// _this if there's no cached reference available.  Notice that
    /// this method does *NOT* increase the reference count of the
    /// cached reference.
    Components::Deployment::Container_ptr get_objref (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS);

protected:
    /// parse The Properties
    void parse_config_values (const ::Deployment::Properties & properties,
                              struct home_installation_info &component_install_info
                              ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       Components::Deployment::UnknownImplId,
                       Components::Deployment::ImplEntryPointNotFound,
                       Components::InvalidConfiguration));


    /// Keep a pointer to the managing ORB serving this servant.
    CORBA::ORB_var orb_;

    /// Keep a pointer to the managing POA.
    PortableServer::POA_var poa_;

    /// Internal container implementation.
    CIAO::Container *container_;

    /// Cached ConfigValues.
    Deployment::Properties con_properties_;

    /// Cached Container reference (of ourselves.)
    Components::Deployment::Container_var objref_;

    /// Cached NodeApplication ref.
    CORBA::Object_var nodeapp_;

    /// Synchronize access to the object set.
    TAO_SYNCH_MUTEX lock_;

    /// Keep a list of managed CCMHome.
    Object_Set<Components::CCMHome, Components::CCMHome_var> home_set_;
  };
}

#if defined (__ACE_INLINE__)
# include "Container_Impl.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif /* CIAO_CONTAINER_IMPL_H */
