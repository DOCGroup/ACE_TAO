// -*- C++ -*-
//
// $Id$


#ifndef DEPLOYMENT_NODEAPPLICATIONMANAGER_REPLY_HANDLER_H
#define DEPLOYMENT_NODEAPPLICATIONMANAGER_REPLY_HANDLER_H

#include "DAnCE/Deployment/Deployment_NodeApplicationManagerS.h"
#include "DAnCE/Deployment/Deployment_NodeApplicationC.h"
#include "DomainApplicationManager_Export.h"
#include "DomainApplicationManager_AMH_Impl.h" // A base class
#include "ace/Thread_Mutex.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class DomainApplicationManager_Export Deployment_AMI_NodeApplicationManagerHandler_i
  : public virtual POA_Deployment::AMI_NodeApplicationManagerHandler
{
public:
  /// Constructor 
  Deployment_AMI_NodeApplicationManagerHandler_i (CIAO::DomainApplicationManager_AMH_Impl *dam);

  /// Destructor 
  virtual ~Deployment_AMI_NodeApplicationManagerHandler_i (void);

  virtual
  void startLaunch (
      ::Deployment::Application_ptr ami_return_val,
      const ::Deployment::Connections & providedReference);

  ::Deployment::NodeApplication_ptr get_node_app (void);

  ::Deployment::Connections get_connections (void);

  /// Unused operations

  virtual
  void perform_redeployment (
      ::Deployment::Application_ptr ami_return_val,
      const ::Deployment::Connections & providedReference);
  
  virtual
  void perform_redeployment_excep (::Messaging::ExceptionHolder *);
  
  virtual
  void reset_plan (void);
  
  virtual
  void reset_plan_excep (::Messaging::ExceptionHolder *);
  
  virtual
  void set_shared_components (void);
  
  virtual
  void set_shared_components_excep (::Messaging::ExceptionHolder *);
  
  virtual
  void set_priority (::CORBA::Long);
  
  virtual
  void set_priority_excep (::Messaging::ExceptionHolder *);
   
  virtual
  void startLaunch_excep (::Messaging::ExceptionHolder *);
  
  virtual
  void destroyApplication (void);
  
  virtual
  void destroyApplication_excep (::Messaging::ExceptionHolder *);

private:
  ::Deployment::NodeApplication_var retn_node_app_;

  ::Deployment::Connections retn_connections_;

  TAO_SYNCH_MUTEX lock_;

  ::CIAO::DomainApplicationManager_AMH_Impl * dam_;
};


#endif /* DEPLOYMENT_NODEAPPLICATIONMANAGER_REPLY_HANDLER_H  */

