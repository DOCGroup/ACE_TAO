//$Id$
/**========================================================
 *
 * @file   NodeApplication_Impl.h
 *
 * @Brief  This file contains the implementation of
 *         the NodeApplication interface.
 *
 * @auther Tao Lu <lu@dre.vanderbilt.edu>
 *========================================================*/

#ifndef NODEAPPLICATION_IMPL_H
#define NODEAPPLICATION_IMPL_H
#include /**/ "ace/pre.h"

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Synch_Traits.h"
#include "Object_Set_T.h"
#include "Container_Base.h"
#include "Deployment_CoreS.h"

/**
 *
 * @class NodeApplication_Impl
 *
 * @brief This class implements the NodeApplication interface.
 * This interface is semantically very simillar to container
 * in the old DnC spec. However this class will also be used
 * as a Server for hosting home/component.
 *
 * Assumptions:
 * 1. There is only 1 container for all components/homes associating
 *    with 1 NodeApplication
 **/

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

  class CIAO_SERVER_Export NodeApplication_Impl
    : public virtual POA_Deployment::NodeApplication
  {
  public:

    // Default constructor.
    NodeApplication_Impl (CORBA::ORB_ptr o,
			  PortableServer::POA_ptr p);

    // Default destructor.
    virtual ~NodeApplication_Impl (void);


    /**
     * @method finishLaunch
     *
     * @brief This operation dose 2 things.
     *        1. Get the external connction (facet and Event source)
     *           and connect them to the local receptacle/event sink.
     *        2. If the start augment is true, start the Components.
     * @Note:
     * The connection containes the object ref of the provided object
     * reference (facet/event consumer) of components from other NodeApplications.
     * However the name field stores the name of the port on the local component.
     *
     */
    virtual void
    finishLaunch (const Deployment::Connections & providedReference,
		  CORBA::Boolean start
		  ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
		       Deployment::StartError,
		       Deployment::InvalidConnection));

    /**
     * @method start
     */
    virtual void
    start (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
		       Deployment::StartError));

    /*-------------  CIAO specific helper operations (idl)--------
     *
     *-----------------------------------------------------------*/
    // Access the readonly attribute.
    virtual ::Deployment::Properties *
    properties (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

    virtual ::Components::CCMHome_ptr
    install_home (const ::Deployment::Properties  & properties
		  ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       Deployment::UnknownImplId,
                       Deployment::ImplEntryPointNotFound,
                       Deployment::InstallationFailure,
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

    //@@ This interface is supposed to be used by NodeApplicationManager
    //   to set objref of NAM. Some pre-configuration might happen as well.
    virtual CORBA::Long init (const ::Deployment::Properties & properties
			      ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                      Components::InvalidConfiguration));


    /*-------------  CIAO specific helper functions (C++)---------
     *
     *-----------------------------------------------------------*/

    // Get the containing POA.  This operation does *not*
    // increase the reference count of the POA.
    virtual PortableServer::POA_ptr _default_POA (void);

    // Get the object reference of the NodeApplicationManager.
    // This might comes in handy later.
    virtual ::CORBA::Object_ptr
    get_node_application_manager (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

    protected:
    /// parse The Properties
    void parse_config_values (const ::Deployment::Properties & properties,
                              struct home_installation_info &component_install_info
                              ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       Deployment::UnknownImplId,
                       Deployment::ImplEntryPointNotFound,
                       Components::InvalidConfiguration));

    // Keep a pointer to the managing ORB serving this servant.
    CORBA::ORB_var orb_;

    // Keep a pointer to the managing POA.
    PortableServer::POA_var poa_;

    /// Internal container implementation.
    CIAO::Container *container_;

    // Cached properties
    Deployment::Properties properties_;

    // And a reference to the NodeApplicationManager that created us.
    ::CORBA::Object_var node_app_manager_;

    // Note: only the NodeApplication really holds the home/component
    //       -->No duplicated() when returning these.

    // Keep a list of managed CCMHome.
    Object_Set<Components::CCMHome, Components::CCMHome_var> home_set_;

    // Synchronize access to the object set.
    // This will be needed in the case when component/home run in different thread
    TAO_SYNCH_MUTEX lock_;

    // Keep a list of managed CCMComponent.
    // Object_Set<Components::CCMComponent, Components::CCMComponent_var> compoene_set_;
  };
}

#if defined (__ACE_INLINE__)
# include "NodeApplication_Impl.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif /* NODEAPPLICATION_IMPL_H */
