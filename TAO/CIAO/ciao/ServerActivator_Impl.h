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
#include "CIAO_ServersS.h"
#include "Object_Set_T.h"

namespace CIAO
{
  /**
   * @class Activator_Callback
   *
   * @brief A call back interface for ComponentServer.
   *
   * Notice that this implementation is *NOT* thread-safe.
   */
  class CIAO_SERVER_Export Activator_Callback_Impl
    : public virtual POA_CIAO::Activator_Callback,
      public virtual PortableServer::RefCountServantBase
  {
  public:
    friend class ServerActivator_Impl;

    /// Constructor.
    Activator_Callback_Impl (CORBA::ORB_ptr o,
                             PortableServer::POA_ptr p,
                             Components::Deployment::ServerActivator_ptr s,
                             const Components::ConfigValues &cofig);

    /// Destructor.
    ~Activator_Callback_Impl ();

    /// Get the containing POA.  This operation does *not*
    /// increase the reference count of the POA.
    virtual PortableServer::POA_ptr _default_POA (void);

    /// Record the ComponentServer reference returned by the newly
    /// spawned ComponentServer and give it back a reference to ServerActivator.
    Components::Deployment::ServerActivator_ptr
    register_component_server (Components::Deployment::ComponentServer_ptr svr,
                               Components::ConfigValues_out config
                               ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

  protected:
    Components::Deployment::ComponentServer_ptr get_server_ref (void);

    CORBA::ORB_var orb_;

    PortableServer::POA_var poa_;

    /// This servant only lives as long as a call to the
    /// create_component_server method in ServerActivator, so there's
    /// no need to duplicate the object reference here.
    Components::Deployment::ServerActivator_ptr activator_;

    Components::Deployment::ComponentServer_var server_;

    Components::ConfigValues_var config_;
  };

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
    ServerActivator_Impl (CORBA::ORB_ptr o,
                          PortableServer::POA_ptr p);

    /// Destructor
    virtual ~ServerActivator_Impl (void);

    /// Get the containing POA.  This operation does *not*
    /// increase the reference count of the POA.
    virtual PortableServer::POA_ptr _default_POA (void);

    /**
     * Initialize the ServerActivator.  The currently implementation
     * of ServerActivator can only activate CIAO's own
     * ComponentServer processes.  You should specify the location
     * (pathname) of the ComponentServer and the delay
     * ServerActivator should wait (in second) for ComponentServer to
     * call back.
     *
     * @param server_location A null-termiated char * string pointing
     *    to the pathname of a ComponentServer executable.

     * @param delay instructs how long (in second) a ServerActivator
     *   should wait for a newly spawned ComponentServer to pass back
     *   its IOR.
     *
     * @param installation_ior A null-terminated char * string
     * pointing to the stringified IOR of a ComponentInstallation that
     * ComponentServer can use to location component implementation DLL.
     *
     * @param default_svcconf A null-terminated char * string pointing
     * to the pathname of the default svcconf file a ComponentServer
     * should use (specified using -ORBSvcConfFile) if no hint is given.
     *
     * @param svcconf_map_file A null-terminated char * string
     * pointing to the pathname of a (hint, svcconf pathname) map.
     */
    int init (const char *server_location,
              CORBA::ULong delay,
              const char *installation_ior,
              const char *default_svcconf,
              const char *svcconf_map_file
              ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

    /// Components::Deployment::ServerActivator defined attributes/operations.

    virtual Components::Deployment::ComponentServer_ptr
    create_component_server (const Components::ConfigValues & config
                             ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       Components::CreateFailure,
                       Components::InvalidConfiguration));

    virtual void remove_component_server (Components::Deployment::ComponentServer_ptr server
                                          ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       Components::RemoveFailure));

    virtual Components::Deployment::ComponentServers *
    get_component_servers (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

    /// Return the cached ServerActivator object reference.  Do not
    /// release this object reference, or put it into an var.
    Components::Deployment::ServerActivator_ptr
    _ciao_get_objref (void);

    const char *_ciao_get_ior (void);

  protected:
    /**
     * Contains the information extracted from
     * create_component_server's ConfigValue argument that
     * ServerActivation cares.
     */
    typedef struct CS_config
    {
      CORBA::String_var svcconf_hint_; // ConfigValue::name: CIAO-svcconf-id
    } CompServ_Config_Info;

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
                             CompServ_Config_Info &config_info
                             ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       Components::InvalidConfiguration));


  protected:
    /// location of the ComponentServer.
    CORBA::String_var server_path_;

    /// Keep a pointer to the managing ORB serving this servant.
    CORBA::ORB_var orb_;

    /// Keep a pointer to the managing POA.
    PortableServer::POA_var poa_;

    /// Child poa that uses active object map.
    PortableServer::POA_var callback_poa_;

    /// Cached the IOR string to ComponentInstallation Interface
    CORBA::String_var installation_ior_;

    /// Specify the time in second ServerActivator will wait for a
    /// child ComponentServer to callback.  Default is 5 second.
    CORBA::ULong spawn_delay_;

    /// Cache a object reference to this servant.
    Components::Deployment::ServerActivator_var objref_;

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
    Object_Set<Components::Deployment::ComponentServer, Components::Deployment::ComponentServer_var> cs_set_;
  };
}

#if defined (__ACE_INLINE__)
# include "ServerActivator_Impl.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* CIAO_SERVERACTIVATOR_IMPL_H */
