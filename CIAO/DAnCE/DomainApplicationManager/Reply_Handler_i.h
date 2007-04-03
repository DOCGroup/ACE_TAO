// -*- C++ -*-
//
// $Id$


#ifndef DEPLOYMENT_NODEAPPLICATIONMANAGER_REPLY_HANDLER_H
#define DEPLOYMENT_NODEAPPLICATIONMANAGER_REPLY_HANDLER_H

#include "DAnCE/Deployment/Deployment_NodeApplicationManagerS.h"
#include "DAnCE/Deployment/Deployment_NodeApplicationC.h"
#include "DomainApplicationManager_Export.h"


#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */


class DomainApplicationManager_Export Deployment_AMI_NodeApplicationManagerHandler_i
  : public virtual POA_Deployment::AMI_NodeApplicationManagerHandler
{
public:
  /// Constructor 
  Deployment_AMI_NodeApplicationManagerHandler_i (void);
  
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
  void perform_redeployment_excep (
      ::Messaging::ExceptionHolder * excep_holder);
  
  virtual
  void reset_plan (
      void);
  
  virtual
  void reset_plan_excep (
      ::Messaging::ExceptionHolder * excep_holder);
  
  virtual
  void set_shared_components (
      void);
  
  virtual
  void set_shared_components_excep (
      ::Messaging::ExceptionHolder * excep_holder);
  
  virtual
  void set_priority (
      ::CORBA::Long ami_return_val);
  
  virtual
  void set_priority_excep (
      ::Messaging::ExceptionHolder * excep_holder);
   
  virtual
  void startLaunch_excep (
      ::Messaging::ExceptionHolder * excep_holder);
  
  virtual
  void destroyApplication (
      void);
  
  virtual
  void destroyApplication_excep (
      ::Messaging::ExceptionHolder * excep_holder);

private:
  ::Deployment::NodeApplication_var retn_node_app_;

  ::Deployment::Connections retn_connections_;

  ACE_Thread_Mutex lock_;

};


#endif /* DEPLOYMENT_NODEAPPLICATIONMANAGER_REPLY_HANDLER_H  */

