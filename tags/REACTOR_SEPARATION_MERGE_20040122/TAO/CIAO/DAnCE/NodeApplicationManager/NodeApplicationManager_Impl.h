//$Id$
// -*- C++ -*-

//=============================================================================
/**
 *  @file    NodeApplicationManager_Impl.h
 *
 *  @author  Tao Lu <lu@dre.vanderbilt.edu>
 *
 *  This file contains implementation for the servant of
 *  Deployment::NodeApplicationManager.
 */
//=============================================================================


#ifndef CIAO_NODEAPPLICATIONMANAGER_IMPL_H
#define CIAO_NODEAPPLICATIONMANAGER_IMPL_H
#include /**/ "ace/pre.h"

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ciao/DeploymentS.h"
#include "CIAO_NAM_Export.h"
#include "ciao/NodeApp_CB_Impl.h"
#include "ImplementationInfo.h"
#include "ciao/CIAO_common.h"

namespace CIAO
{
  /**
   * @class NodeApplicationManager_Impl
   */
  class CIAO_NAM_Export NodeApplicationManager_Impl
    : public virtual POA_Deployment::NodeApplicationManager,
      public virtual PortableServer::RefCountServantBase
  {
  public:
    /// Constructor
    NodeApplicationManager_Impl (CORBA::ORB_ptr o,
                                 PortableServer::POA_ptr p);

    /*===========================================================
     * Below are operations from the NodeApplicationManager
     *
     *============================================================*/

    //@@ The return type is NodeApplication_ptr actually.
    virtual Deployment::Application_ptr
    startLaunch (const Deployment::Properties & configProperty,
                    Deployment::Connections_out providedReference,
                                 CORBA::Boolean start
                        ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                              Deployment::ResourceNotAvailable,
                              Deployment::StartError,
                              Deployment::InvalidProperty));

    //@@ Destroy the whole applicaton.
    virtual void destroyApplication (Deployment::Application_ptr
                                     ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                              Deployment::StopError));

    /**
     * Initialize the NodeApplicationManager.  The currently implementation
     * of CIAO_NodeApplicationManager can only activate CIAO's own
     * NodeApplication processes.  You should specify the location
     * (pathname) of the NodeApplication and the delay
     * CIAO_NodeApplicationManager should wait (in second) for NodeApplication to
     * call back.
     *
     * @param nodeapp_location A null-termiated char * string pointing
     *    to the pathname of a NodeApplication executable.
     *
     * @param delay instructs how long (in second) a CIAO_NodeApplicationManager
     *   should wait for a newly spawned NodeApplication to pass back
     *   its IOR.
     *
     * @param plan has all the information needed for the NodeAppManager to
     *        construct the application locally. Note, this includes the properties
     *        of the NodeApplication. For example, ORB config options etc.
     *
     * @param callback_poa contains child poa created for the callback interface.
     *
     * @return NodeApplicationManager_ptr.
     **/
    virtual Deployment::NodeApplicationManager_ptr
    init (const char *nodeapp_location,
          const CORBA::ULong delay,
          const Deployment::DeploymentPlan & plan,
          const PortableServer::POA_ptr callback_poa
          ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       Deployment::InvalidProperty));

    //@@ Note: This method doesn't do duplicate.
    Deployment::NodeApplicationManager_ptr get_nodeapp_manager (void);

  protected:
    /// Destructor
    virtual ~NodeApplicationManager_Impl (void);

    // Internal help function to create new NodeApplicationProcess
    virtual Deployment::NodeApplication_ptr
    create_node_application (const ACE_CString & options
                             ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       Deployment::ResourceNotAvailable,
                       Deployment::StartError,
                       Deployment::InvalidProperty));

    // Helper function to get the connection.
    virtual Deployment::Connections *
    create_connections (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       Deployment::ResourceNotAvailable,
                       Deployment::StartError,
                       Deployment::InvalidProperty));

    /**
     * Find the configurations in the plan and prepare them to pass to the
     * NodeApplication, from command line probably.
     */
    void parse_config_value (ACE_CString & str
                             ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       Deployment::InvalidProperty));

  protected:
    // location of the Nodeapplication
    ACE_CString nodeapp_path_;

    // Keep a pointer to the managing ORB serving this servant.
    CORBA::ORB_var orb_;

    // Keep a pointer to the managing POA.
    PortableServer::POA_var poa_;

    // ObjectRef of ourself which will be needed by the callback
    Deployment::NodeApplicationManager_var objref_;

    // Child poa that uses active object map.
    PortableServer::POA_var callback_poa_;

    // Cache a object reference of the underlying NodeApplication
    // Since I have decided to have only 1 NA in NAM so no map is needed.
    Deployment::NodeApplication_var nodeapp_;

    // Cached plan (This should be the part of the whole plan local to this node)
    // The plan will be initialized when init is called.
    Deployment::DeploymentPlan plan_;

    // Specify the time in second NodeApplicationManager will wait for a
    // child NodeApplication to callback.  Default is 5 second.
    CORBA::ULong spawn_delay_;

    // Extracted commandline options to pass to the NodeApplication.
    CORBA::String_var nodeapp_command_op_;

    // A map of the component created on this node.
    typedef ACE_Hash_Map_Manager_Ex<ACE_CString,
                                    Components::CCMObject_var,
                                    ACE_Hash<ACE_CString>,
                                    ACE_Equal_To<ACE_CString>,
                                    ACE_Null_Mutex> CCMComponent_Map;
    typedef CCMComponent_Map::iterator Component_Iterator;
    CCMComponent_Map component_map_;

    // Synchronize access to the object set.
    TAO_SYNCH_MUTEX lock_;
  };
}

#if defined (__ACE_INLINE__)
# include "NodeApplicationManager_Impl.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif /* CIAO_NODEAPPLICATIONMANAGER_IMPL_H */
