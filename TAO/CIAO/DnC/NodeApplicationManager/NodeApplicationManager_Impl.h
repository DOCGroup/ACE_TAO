//$Id$
// -*- C++ -*-

//=============================================================================
/**
 *  @file    NodeApplicationManager_Impl.h
 *
 *  @author  Nanbor Wang <nanbor@cs.wustl.edu>
 *           Tao Lu <lu@dre.vanderbilt.edu>
 *
 *  This file contains implementation for the servant of
 *  Deployment::NodeApplicationManager. It contains the
 *  the functionality of Components::Deployment::ServerActivator interface.
 *
//=============================================================================


#ifndef CIAO_NODEAPPLICATIONMANAGER_IMPL_H
#define CIAO_NODEAPPLICATIONMANAGER_IMPL_H
#include /**/ "ace/pre.h"

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "../DeploymentS.h"
#include "CIAO_NodeApplicationManagerS.h"

#include "ciao/Object_Set_T.h"

namespace CIAO
{
  /**
   * @class NodeApplicationManager_Callback
   *
   * @brief A call back interface for NodeApplication.
   *
   * Notice that this implementation is *NOT* thread-safe.
   */

  //@@ Need to generate the export file. --Tao
  class CIAO_NODEAPPLICATIONMANAGER_Export NodeApplicationManager_Callback_Impl
    : public virtual POA_CIAO::NodeApplicationManager_Callback,
      public virtual PortableServer::RefCountServantBase
  {
  public:
    friend class NodeApplicationManager_Impl;

    /// Constructor.
    NodeApplicationManager_Callback_Impl (CORBA::ORB_ptr o,
					  PortableServer::POA_ptr p,
					  Deployment::NodeApplicationManager_ptr nam,
					  const Components::ConfigValues &cofig);

    /// Destructor.
    ~NodeApplicationManager_Callback_Impl ();

    /// Get the containing POA.  This operation does *not*
    /// increase the reference count of the POA.
    virtual PortableServer::POA_ptr _default_POA (void);

    /// Record the NodeApplication reference returned by the newly
    /// spawned NodeApplication and give it back a reference to CIAO_NodeApplicationManager.
    Deployment::NodeApplicationManager_ptr
    register_node_application (Deployment::NodeApplication_ptr na,
			       Components::ConfigValues_out config
			       ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

  protected:
    Deployment::NodeApplication_ptr get_nodeapp_ref (void);

    CORBA::ORB_var orb_;

    PortableServer::POA_var poa_;

    /// This servant only lives as long as a call to the
    /// create_node_application method in NodeApplicationManager, so there's
    /// no need to duplicate the object reference here.
    Deployment::NodeApplicationManager_ptr nam_;

    Deployment::NodeApplication_var na_;

    Components::ConfigValues_var config_;
  };

  /**
   * @class NodeApplicationManager_Impl
   */
  class CIAO_NODEAPPLICATIONMANAGER_Export NodeApplicationManager_Impl
    : public virtual POA_Deployment::NodeApplicationManager,
      public virtual PortableServer::RefCountServantBase
  {
  public:
    /// Constructor
    NodeApplicationManager_Impl (CORBA::ORB_ptr o,
				 PortableServer::POA_ptr p);

    /// Destructor
    virtual ~NodeApplicationManager_Impl (void);


    /*===========================================================
     * Below are operations from the NodeApplicationManager
     *
     *============================================================*/

    //@@ The return type is NodeApplication_ptr actually.
    //   Mappings between configureVals and Properpies needs to be fixed. --Tao
    virtual Deployment::Application_ptr
    startLaunch (const Deployment::Properties & configProperty,
		 Deployment::Connections_out providedReference,
		 CORBA::Boolean start
		 ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException
		       , Deployment::ResourceNotAvailable
		       , Deployment::StartError
		       , Deployment::InvalidProperty));

    //
    virtual void destroyApplication (Deployment::Application_ptr app
				     ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException
		       , Deployment::StopError));


    /*===========================================================
     * Below are operations from the old ServerActivatro
     *
     *============================================================*/
    /// Get the containing POA.  This operation does *not*
    /// increase the reference count of the POA.
    virtual PortableServer::POA_ptr _default_POA (void);

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

     * @param delay instructs how long (in second) a CIAO_NodeApplicationManager
     *   should wait for a newly spawned NodeApplication to pass back
     *   its IOR.
     *
     * @param installation_ior A null-terminated char * string
     * pointing to the stringified IOR of a ComponentInstallation that
     * CIAO_NodeApplicationManager can use to location component implementation DLL.
     *
     * @param default_svcconf A null-terminated char * string pointing
     * to the pathname of the default svcconf file a NodeApplication
     * should use (specified using -ORBSvcConfFile) if no hint is given.
     *
     * @param svcconf_map_file A null-terminated char * string
     * pointing to the pathname of a (hint, svcconf pathname) map.
     *
     * @Note: Need to add the initialization of Plan Here!!!! --Tao
     */
    int init (const char *nodeapp_location,
              CORBA::ULong delay,
              const char *installation_ior,
              const char *default_svcconf,
              const char *svcconf_map_file
	      // Need to add an augment of plan here!!! --Tao
              ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException
		       , Deployment::InvalidProperty));

    /// Return the cached NodeApplicationManager object reference.  Do not
    /// release this object reference, or put it into an var.
    /// @@ I dont understand the comment above --Tao
    Deployment::NodeApplicationManager_ptr
    _ciao_get_objref (void);

    // Helper function.
    const char *_ciao_get_ior (void);

  protected:
    /**
     * Contains the information extracted from
     * create_component_server's ConfigValue argument that
     * ServerActivation cares.
     */
    typedef struct CS_config
    {
      // ConfigValue::name: CIAO-svcconf-id
      CORBA::String_var svcconf_hint_;

      // ConfigValue::Name: CIAO-rtcad-filename
      CORBA::String_var rtcad_filename_;
    } NodeApp_Config_Info;  //CompServ_Config_Info;

    // Internal help function to create new NodeApplicationProcess
    virtual Deployment::NodeApplication_ptr
    create_node_application (const Components::ConfigValues & config
			       ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException
		       , Deployment::StartError
		       , Deployment::InvalidProperty));

    /*  Not sure if this will be useful here.
    virtual Deployment::NodeApplications *
    get_node_applications (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));
    */

    /**
     * Initialize svc_conf_map_ with the supplied filename.
     */
    int init_svcconf_map (const char *map_file);

    /**
     * Returns the pathname to the effective svc.conf file based on
     * "hint."  It may return the default svc.conf file when there's
     * no hint (hint == 0) and we do have a default svc.conf path.
     */
    const char *lookup_svcconf_pathname (const char *hint) const;

    /**
     * Parse the ConfigValue arguement of the function
     * create_component_server and extractg the information into
     */
    void parse_config_value (const Components::ConfigValues &options,
                             NodeApp_Config_Info &config_info
                             ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       Components::InvalidConfiguration));


  protected:
    /// location of the Nodeapplication
    CORBA::String_var nodeapp_path_;

    /// Keep a pointer to the managing ORB serving this servant.
    CORBA::ORB_var orb_;

    /// Keep a pointer to the managing POA.
    PortableServer::POA_var poa_;

    /// Child poa that uses active object map.
    PortableServer::POA_var callback_poa_;

    /// Cached the IOR string to ComponentInstallation Interface
    CORBA::String_var installation_ior_;

    /// Specify the time in second NodeApplicationManager will wait for a
    /// child NodeApplication to callback.  Default is 5 second.
    CORBA::ULong spawn_delay_;

    /// Cache a object reference to this servant.
    Deployment::NodeApplicationManager_var objref_;

    /// Cache the ior of the previous reference
    CORBA::String_var ior_;

    /// Synchronize access to the object set.
    TAO_SYNCH_MUTEX lock_;

    /// Default svc.conf filename.
    ACE_CString default_svcconf_file_;

    typedef ACE_Hash_Map_Manager_Ex<ACE_CString,
                                    ACE_CString,
                                    ACE_Hash<ACE_CString>,
                                    ACE_Equal_To<ACE_CString>,
                                    ACE_Null_Mutex> SVCCONF_MAP;

    /// Map of (canonical svc.conf id, svc.conf) mapping
    SVCCONF_MAP svc_conf_map_;

    /// Keep a list of managed ComponentServer objects.
    Object_Set<Deployment::NodeApplication, Deployment::NodeApplication_var> na_set_;

    /// Cached plan (This should be the part of the whole plan local to this node)
    /// The plan will be initialized when init is called.
    Deployment::DeploymentPlan_var plan_;
  };
}

#if defined (__ACE_INLINE__)
# include "CIAO_NodeApplicationManager_Impl.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif /* CIAO_NODEAPPLICATIONMANAGER_IMPL_H */
