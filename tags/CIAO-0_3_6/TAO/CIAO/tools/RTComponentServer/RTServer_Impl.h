// -*- C++ -*-

//=============================================================================
/**
 *  @file    RTServer_Impl.h
 *
 *  $Id$
 *
 *  This file contains real-time extended implementations for the
 *  servants of
 *         Components::Deployment::ComponentServer, and
 *         Components::Deplayment::Container.
 *
 *  It also contains the real-time extended version of
 *  CIAO::Session_Container.
 *
 *  Currently, they are only used in the RTComponentServer
 *  implementation.
 *
 *  @author Nanbor Wang <nanbor@cs.wustl.edu>
 */
//=============================================================================


#ifndef CIAO_RTSERVER_IMPL_H
#define CIAO_RTSERVER_IMPL_H
#include /**/ "ace/pre.h"

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ComponentServer_Impl.h"
#include "Container_Impl.h"
#include "Container_Base.h"
#include "RTConfig_Manager.h"

namespace CIAO
{
  namespace RTServer
  {
    /**
     * @class RTComponentServer
     *
     * @brief Real-time extention servant implementation of
     *        Components::Deployment::ComponentServer
     *
     * This class implements the Components::Deployment::ComponentServer
     * interface as defined by the CCM spcification.  As the interface
     * implies, this is actually part of the deployment interface and is
     * used to manage the lifecycle of containers running on the server.
     */
    class RTComponentServer_Impl
      : public virtual CIAO::ComponentServer_Impl
    {
    public:
      /// Constructor
      RTComponentServer_Impl (CORBA::ORB_ptr o,
                              RTCORBA::RTORB_ptr rto,
                              PortableServer::POA_ptr p);

      /// Destructor
      virtual ~RTComponentServer_Impl (void);

      /// Initialize the ComponentServer with a name.
      int init (::Components::ConfigValues &options
                ACE_ENV_ARG_DECL_WITH_DEFAULTS)
        ACE_THROW_SPEC ((CORBA::SystemException,
                         Components::InvalidConfiguration));

      virtual ::Components::Deployment::Container_ptr
      create_container (const Components::ConfigValues & config
                        ACE_ENV_ARG_DECL_WITH_DEFAULTS)
        ACE_THROW_SPEC ((CORBA::SystemException,
                         Components::CreateFailure,
                         Components::InvalidConfiguration));

    protected:
      void parse_server_config_values (const Components::ConfigValues &options
                                       ACE_ENV_ARG_DECL_WITH_DEFAULTS)
        ACE_THROW_SPEC ((CORBA::SystemException,
                         Components::InvalidConfiguration));

      // Real-time resource manager
      CIAO::RTResource_Config_Manager resource_manager_;

      // Real-time Policy_Set_Manager
      CIAO::RTPolicy_Set_Manager policy_set_manager_;
    };

    /**
     * @class RTContainer
     *
     * @brief Real-time extention servant implementation of
     *        Components::Deployment::Container
     */
    class RTContainer_Impl
      : public virtual CIAO::Container_Impl
    {
    public:
    /// Constructor
      RTContainer_Impl (CORBA::ORB_ptr o,
                        PortableServer::POA_ptr p,
                        Components::Deployment::ComponentServer_ptr server,
                        CIAO::RTPolicy_Set_Manager &policyset_manager);

      /// Destructor
      virtual ~RTContainer_Impl (void);

      /// Initialize the container with a name.
      int init (const Components::ConfigValues &options,
                Components::Deployment::ComponentInstallation_ptr installation
                ACE_ENV_ARG_DECL_WITH_DEFAULTS)
        ACE_THROW_SPEC ((CORBA::SystemException,
                         Components::InvalidConfiguration));

    protected:
      void parse_container_config_values (const Components::ConfigValues &options
                                          ACE_ENV_ARG_DECL_WITH_DEFAULTS)
        ACE_THROW_SPEC ((CORBA::SystemException,
                         Components::InvalidConfiguration));

      // Hold a reference to the policy manager that we use to acquire
      // the policy list.
      CIAO::RTPolicy_Set_Manager &policyset_manager_;

      // Effective policies
      CORBA::PolicyList_var Policies_;
    };

    class RTSession_Container
      : public CIAO::Session_Container
    {
    public:
      RTSession_Container (CORBA::ORB_ptr o);

      virtual ~RTSession_Container (void);

      /// Initialize the container with a name.
      virtual int init (const char *name = 0,
                        const CORBA::PolicyList *add_policies = 0
                        ACE_ENV_ARG_DECL_WITH_DEFAULTS)
        ACE_THROW_SPEC ((CORBA::SystemException));
    };
  }
}

#if defined (__ACE_INLINE__)
# include "RTServer_Impl.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif /* CIAO_RTSERVER_IMPL_H */
