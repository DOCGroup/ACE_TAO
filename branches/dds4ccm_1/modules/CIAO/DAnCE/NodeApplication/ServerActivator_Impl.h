// -*- C++ -*-

//=============================================================================
/**
 *  @file    ServerActivator_Impl.h
 *
 *  $Id$
 *
 * @Brief  Implementation of Deployment::ServerActivator
 *
 * @author Erwin Gottlieb <eg@prismtech.com>
 */
//=============================================================================

#ifndef SERVERACTIVATOR_IMPL_H_
#define SERVERACTIVATOR_IMPL_H_

#include "ace/Map_Manager.h"
#include "tao/ORB.h"
#include "orbsvcs/orbsvcs/CosNamingC.h"

#include "Cdmw/CDMW_IDLS.h"
#include "ComponentServer/ComponentServerS.h"
#include "NodeApplication_Export.h"

namespace DAnCE
  {

  class NodeApplication_Export ServerActivator_Impl : public virtual POA_Components::Deployment::ServerActivator
    {
    public:
      ServerActivator_Impl (CORBA::ORB_ptr orb,
                            PortableServer::POA_ptr poa_,
                            Components::Deployment::ComponentInstallation_ptr compInst);
      virtual ~ServerActivator_Impl();


      //
      // IDL:omg.org/Components/Deployment/ServerActivator/create_component_server:1.0
      //
      /**
       * Creates a ComponentServer object on the host on which the
       * ServerActivator is located.
       *
       * @param  config  ConfigValues for creation of ComponentServer.
       *                 For CDMW CCM, this sequence should contain a
       *                 PROCESS_DESTINATION ConfigValue which specify the
       *                 destination of an already started ComponentServer process.
       *
       * @throws  CreateFailure
       *          INVALID_PROCESS_DESTINATION: raised if the destination
       *          specified in PROCESS_DESTINATION ConfigValue is not valid.<br>
       *          COMPONENT_SERVER_NOT_CREATED: raised if the ComponentServer could
       *          not be contacted (it should be started by PlatformManagement before
       *          the call to ServerActivator).<br>
       *          COMPONENT_SERVER_ALREADY_CREATED: raised if this method has already
       *          been called with the same PROCESS_DESTINATION ConfigValue.
       * @throws  InvalidConfiguration
       *          raised if the config parameter doesn't contain a single
       *          PROCESS_DESTINATION ConfigValue.
       */
      virtual ::Components::Deployment::ComponentServer_ptr create_component_server (
        const ::Components::ConfigValues & config
      );

      //
      // IDL:omg.org/Components/Deployment/ServerActivator/remove_component_server:1.0
      //
      virtual void remove_component_server (
        ::Components::Deployment::ComponentServer_ptr server
      );

      //
      // IDL:omg.org/Components/Deployment/ServerActivator/get_component_servers:1.0
      //
      virtual ::Components::Deployment::ComponentServers * get_component_servers (
      );

    private:
      typedef ACE_Map_Manager<ACE_CString, ::Components::Deployment::ComponentServer_var, ACE_Null_Mutex > TCompServers;
      TCompServers servers_;

      CORBA::ORB_var orb_;
      PortableServer::POA_var poa_;
      CosNaming::NamingContext_var naming_;
      Components::Deployment::ComponentInstallation_var compInst_;

    private:
      void initializeComponentServer (::Components::Deployment::ComponentServer_ptr server);
    };
};

#endif /*SERVERACTIVATOR_IMPL_H_*/
