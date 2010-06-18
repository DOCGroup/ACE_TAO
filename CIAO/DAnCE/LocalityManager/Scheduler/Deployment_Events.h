/**
 * @file Deployment_Events.h
 * @author William R. Otte <wotte@dre.vanderbilt.eud>
 *
 * $Id$
 *
 * This file contains all schedulable deployment events.
 */

#ifndef DEPLOYMENT_EVENTS_H
#define DEPLOYMENT_EVENTS_H

#include /**/ "ace/pre.h"

#include "ace/Method_Request.h"
#include "ace/Future.h"
#include "Deployment/Deployment_DeploymentPlanC.h"
#include "LocalityManager/Scheduler/Deployment_Scheduler_export.h"
#include "LocalityManager/Scheduler/Deployment_Event.h"

namespace DAnCE
{
  class Deployment_Scheduler_Export Deployment_Event : 
    public virtual ACE_Method_Request
  {
  public:
    Deployment_Event (Event_Future holder,
                      const char *instance_type);
    
    Event_Future get_future (void);
    
  protected:
    Event_Future holder_;
    std::string instance_type_;
  };
      
  class Deployment_Scheduler_Export Install_Instance :
    public virtual Deployment_Event
  {
  public:
    Install_Instance (const ::Deployment::DeploymentPlan & plan,
                      ::CORBA::ULong instanceRef,
                      const char *instance_type,
                      Event_Future holder);
    
    virtual ~Install_Instance (void);

    virtual int call (void);

  private:
    ::Deployment::DeploymentPlan plan_;
    ::CORBA::ULong instanceRef_;
  };
 
  class Deployment_Scheduler_Export Connect_Instance :
    public virtual Deployment_Event
  {
  public:
    Connect_Instance (const ::Deployment::DeploymentPlan & plan,
                      ::CORBA::ULong connectionRef,
                      const ::CORBA::Any &provided_ref,
                      const char *instance_type,
                      Event_Future holder);
    
    virtual ~Connect_Instance (void);

    virtual int call (void);

  private:
    ::Deployment::DeploymentPlan plan_;
    ::CORBA::ULong connectionRef_;
    ::CORBA::Any provided_ref_;
  };

  class Deployment_Scheduler_Export Remove_Instance :
    public virtual Deployment_Event
  {
  public:
    Remove_Instance (::Deployment::DeploymentPlan & plan,
                     ::CORBA::ULong instanceRef,
                     const CORBA::Any &reference,
                     const char *instance_type,
                     Event_Future holder);
    
    virtual ~Remove_Instance (void);

    virtual int call (void);

  private:
    ::Deployment::DeploymentPlan & plan_;
    ::CORBA::ULong instanceRef_;
    ::CORBA::Any reference_;
  };
}

#if defined (__ACE_INLINE__)
#include "LocalityManager/Scheduler/Deployment_Events.inl"
#endif

#include /**/ "ace/post.h"

#endif
