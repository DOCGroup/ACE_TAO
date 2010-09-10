/**
 * @file Action_Base.h
 * @author William R. Otte <wotte@dre.vanderbilt.edu>
 * $Id$
 */

#ifndef ACTION_BASE_H
#define ACTION_BASE_H

#include "LocalityManager/Scheduler/Deployment_Event.h"
#include "LocalityManager/Scheduler/Plugin_Manager.h"

namespace DAnCE
{
  class Deployment_Scheduler_Export Action_Base
    : public virtual Deployment_Event
  {
  public:
    Action_Base (Event_Future holder,
                 const char *name,
                 const char *instance_type);
    
    virtual ~Action_Base (void);
    
    virtual int call (void);
    
  protected:
    virtual void invoke_pre_interceptor (Plugin_Manager::INTERCEPTORS::const_iterator &) = 0;
    
    virtual void invoke (::DAnCE::InstanceDeploymentHandler_ptr) = 0;
    
    virtual void invoke_post_interceptor (Plugin_Manager::INTERCEPTORS::const_iterator &) = 0;
    
    virtual void create_unexpected_exception (const std::string &,
                                              const std::string &) = 0;
    
    virtual void create_valid_result (Event_Result &) = 0;
    
    ::CORBA::Any_var instance_excep_;
  };
}

#endif
