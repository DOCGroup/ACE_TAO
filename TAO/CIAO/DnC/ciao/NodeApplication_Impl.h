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

#include "tao/ORB.h"
#include "ace/Synch.h"
#include "ace/Synch_Traits.h"
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
 * 2. Now the implementation is not thread safe.
 **/

namespace CIAO
{
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

    // Initialize the NodeApplication
    virtual CORBA::Long init (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

    // Start install homes and components.
    virtual ::Deployment::ComponentInfos *
    install (const ::Deployment::ImplementationInfos & impl_infos
	     ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
		       ::Deployment::UnknownImplId,
		       ::Deployment::ImplEntryPointNotFound,
		       ::Deployment::InstallationFailure,
		       ::Components::InvalidConfiguration));

    // Access the readonly attribute.
    virtual ::Deployment::Properties *
    properties (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

    virtual ::Components::CCMHome_ptr
    install_home (const ::Deployment::ImplementationInfo & impl_info
		  ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       Deployment::UnknownImplId,
                       Deployment::ImplEntryPointNotFound,
                       Deployment::InstallationFailure,
                       Components::InvalidConfiguration));

    /**
     * @@Note: I don't know how to remove a home right now.
     *         I assume that user will only call remove instead.
     *         This is true at least for DnC run time.
     *
     * Right now, in this implementation I assumpe that there will be
     * same number of homes as the components even if the components
     * are of the same type. I don't think that we have the modeling
     * side support of this either. So bear me if you think I avoid
     * the real thinking for easiness.
     */
    virtual void remove_home (const char * comp_ins_name
                              ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       Components::RemoveFailure));

    // Remove everything inside including all components and homes.
    // User must be sure that no connection is active before calling this!!
    virtual void remove (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       Components::RemoveFailure));

    virtual ::Components::CCMHomes *
    get_homes (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

    //@@ This interface is supposed to be used by NodeApplicationManager
    //   to set objref of NAM. Some pre-configuration might happen as well.
    /*virtual CORBA::Long init (const ::Deployment::Properties & properties
			      ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                      Components::InvalidConfiguration));
    */

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

    // This is a helper method to clean up components
    // should only be called when we are sure that there is no
    // active connection on this component.
    virtual void remove_components (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       Components::RemoveFailure));

    virtual void remove_component (const char * comp_ins_name
				   ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
		       Components::RemoveFailure));

    // This function is a helper for start call.
    typedef void (Components::CCMObject::*Funct_Ptr)
      (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS);

    virtual void start_i (Funct_Ptr functor
			  ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
		       Deployment::StartError));


    // To store all created CCMHome object
    typedef ACE_Hash_Map_Manager_Ex<ACE_CString,
                                    Components::CCMHome_ptr,
                                    ACE_Hash<ACE_CString>,
                                    ACE_Equal_To<ACE_CString>,
                                    ACE_Null_Mutex> CCMHome_Map;
    typedef CCMHome_Map::iterator Home_Iterator;
    CCMHome_Map home_map_;

    // To sotre all created Component object.
    typedef ACE_Hash_Map_Manager_Ex<ACE_CString,
                                    Components::CCMObject_ptr,
                                    ACE_Hash<ACE_CString>,
                                    ACE_Equal_To<ACE_CString>,
                                    ACE_Null_Mutex> CCMComponent_Map;
    typedef CCMComponent_Map::iterator Component_Iterator;
    CCMComponent_Map component_map_;

    // Keep a pointer to the managing ORB serving this servant.
    CORBA::ORB_var orb_;

    // Keep a pointer to the managing POA.
    PortableServer::POA_var poa_;

    // Internal container implementation.
    CIAO::Container *container_;

    // Cached properties
    Deployment::Properties properties_;

    // And a reference to the NodeApplicationManager that created us.
    ::CORBA::Object_var node_app_manager_;

    // Synchronize access to the object set.
    // This will be needed in the case when component/home run in different thread
    // TAO_SYNCH_MUTEX lock_;

    //======================================================

    // Note: only the NodeApplication really holds the home/component
    //       -->No duplicated() when returning these.

    // Keep a list of managed CCMHome.

    //@@ As I have stated in the idl we are not going to use properties for now.
    // parse The Properties
    /*void parse_config_values (const ::Deployment::Properties & properties,
                              struct home_installation_info &component_install_info
                              ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       Deployment::UnknownImplId,
                       Deployment::ImplEntryPointNotFound,
                       Components::InvalidConfiguration));
    */

    // Synchronize access to the object set.
    // This will be needed in the case when component/home run in different thread
    // TAO_SYNCH_MUTEX lock_;

    // Keep a list of managed CCMComponent.
    // Object_Set<Components::CCMComponent, Components::CCMComponent_var> compoene_set_;
  };
}

#if defined (__ACE_INLINE__)
# include "NodeApplication_Impl.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif /* NODEAPPLICATION_IMPL_H */
