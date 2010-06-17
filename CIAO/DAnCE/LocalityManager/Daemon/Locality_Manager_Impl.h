// -*- C++ -*-
// $Id$

/**
 * @file Locality_Manager_Impl.h
 * @author William R. Otte <wotte@dre.vanderbilt.edu>
 **/

// TAO_IDL - Generated from 
// be/be_codegen.cpp:1521

#ifndef DANCE_LOCALITYMANAGERI_PDD6RP_H_
#define DANCE_LOCALITYMANAGERI_PDD6RP_H_

#include "DAnCE/DAnCE_LocalityManagerS.h"
#include "DAnCE/DAnCE_DeploymentInterceptorsC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/LocalObject.h"


#include "LocalityManager/Daemon/Locality_Manager_Impl_Export.h"
#include <map>
#include <vector>
#include <list>
#include <string>

namespace DAnCE
{

  class Locality_Manager_Impl_Export LocalityManager_i
    : public virtual POA_DAnCE::LocalityManager
  {
  public:
    // Constructor 
    LocalityManager_i (const ACE_TString &uuid,
                       CORBA::ORB_ptr orb,
                       PortableServer::POA_ptr poa);
  
    void init (Deployment::Properties *prop);
    // Destructor 
    virtual ~LocalityManager_i (void);
  
    virtual
      ::Deployment::Properties * configuration (void);
  
    virtual
      ::Deployment::ApplicationManager_ptr preparePlan (
                          const ::Deployment::DeploymentPlan & plan,
                          ::Deployment::ResourceCommitmentManager_ptr resourceCommitment);
  
    virtual
      void destroyManager (::Deployment::ApplicationManager_ptr manager);
  
    virtual
      void shutdown (void);
  
    virtual
      void finishLaunch (const ::Deployment::Connections & providedReference,
                         ::CORBA::Boolean start);
  
    virtual
      void start (void);
  
    virtual
      ::Deployment::Application_ptr startLaunch (const ::Deployment::Properties & configProperty,
                                                 ::Deployment::Connections_out providedReference);
  
    virtual
      void destroyApplication (::Deployment::Application_ptr app);
    
  private:
    ACE_TString uuid_;
    CORBA::ORB_var orb_;
    PortableServer::POA_var poa_;
    
    typedef std::list< CORBA::ULong > INSTANCE_LIST;

    struct Handler
    {
      Handler (void) {};
      Handler (::DAnCE::InstanceDeploymentHandler_ptr h)
        : handler_ (h) {};

      ::DAnCE::InstanceDeploymentHandler_var handler_;
      INSTANCE_LIST instances_;
    };

    typedef std::map <std::string, 
                      Handler> HANDLER_TABLE;
    
    HANDLER_TABLE instance_handlers_;
    
    typedef std::vector< std::string > HANDLER_ORDER;
    
    HANDLER_ORDER handler_order_;
    
    typedef std::map < CORBA::ULong, CORBA::Any_var > REFERENCE_MAP;
    
    REFERENCE_MAP instance_references_;
      
    ::Deployment::DeploymentPlan plan_;
    
    ::DAnCE::InstanceInstallation_var ii_interceptor_;
    
    ::Deployment::Properties_var props_;
  };

}
#endif /* DANCE_LOCALITYMANAGERI_H_  */
