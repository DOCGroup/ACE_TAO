//$Id$
/**========================================================
 *
 * @file   NodeApplication_Impl.h
 *
 * @Brief  This file contains the implementation of
 *         the NodeApplication interface.
 *
 * @auther Tao Lu <lu@dre.vanderbilt.edu>
 * @author Gan Deng <gan.deng@vanderbilt.edu>
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
#include "Server_init.h"
#include "CIAO_common.h"

using CIAO::Utility::write_IOR;

/**
 *
 * @class NodeApplication_Impl
 *
 * @brief This class implements the NodeApplication interface.
 * This interface is semantically very simillar to container
 * in the old DnC spec. However this class will also be used
 * as a Server for hosting home/component. This way we reduce the
 * complexity of the framework by omitting the componentserver layer.
 *
 * @@TODO add configuration capabilities. Threading is one of them.
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

    // This function is a helper for start call. Bala's
    // Idea of adding those pre/post activate calls doesn't work
    // with the new sepc.
    //@@ TODO.   Come up with new ways of synchronized initialization process.
    typedef void (Components::CCMObject::*Funct_Ptr)
      (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS);

    virtual void start_i (Funct_Ptr functor
                          ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       Deployment::StartError));

    // Keep a pointer to the managing ORB serving this servant.
    CORBA::ORB_var orb_;

    // Keep a pointer to the managing POA.
    PortableServer::POA_var poa_;

    // And a reference to the NodeApplicationManager that created us.
    ::CORBA::Object_var node_app_manager_;
  };
}

#if defined (__ACE_INLINE__)
# include "NodeApplication_Impl.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif /* NODEAPPLICATION_IMPL_H */
