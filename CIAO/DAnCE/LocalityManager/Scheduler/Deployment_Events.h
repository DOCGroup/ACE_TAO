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

namespace DAnCE
{
  struct Event_Result
  {
    std::string id_;
    bool exception_;
    CORBA::Any_var contents_;
  };
    
  typedef ACE_Future< Event_Result > Event_Future;
  
  /**
   * @class Event_Handler
   * @brief Future observer that invokes a parameterized functor on the future
   */
  template <typename Functor>
  class Event_Handler
    : ACE_Future_Observer< Event_Result >
  {
  public:
    Event_Handler (Functor &specific_handler);
    
    virtual ~Event_Handler (void);

    virtual void update (const Event_Result &future);
    
    /// Indicate to the observer that there is an additional future
    /// it is waiting on
    void add_outstanding (void);
    
    /// Return the number of still outstanding future events.
    size_t count_outstanding (void);
  private:
    ACE_Atomic_Op < TAO_SYNCH_MUTEX, unsigned long > outstanding_;
    
    Functor &specific_handler_;
  };

  class Deployment_Event : 
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
      
  class Install_Instance :
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
 
  class Connect_Instance :
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

  class Remove_Instance :
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
#include "LocalityManager/Scheduler/Deployment_Events.h"
#endif

#include /**/ "ace/post.h"

#endif
