// $Id$
#include "Configured.h"

#include "tao/AnyTypeCode/ExceptionA.h"
#include "Logger/Log_Macros.h"
#include "Deployment/Deployment_StartErrorC.h"
#include "DAnCE/DAnCE_DeploymentInterceptorsC.h"
#include "DAnCE/DAnCE_LocalityManagerC.h"
#include "DAnCE/DAnCE_Utility.h"
#include "LocalityManager/Scheduler/Deployment_Event.h"
#include "LocalityManager/Scheduler/Plugin_Manager.h"

namespace DAnCE
{
  Instance_Configured::Instance_Configured (::Deployment::DeploymentPlan & plan,
                                  ::CORBA::ULong instanceRef,
                                  const char *inst_type,
                                  Event_Future holder)
    : Deployment_Event (holder,  
                        plan.instance[instanceRef].name.in (),
                        inst_type),
      Action_Base (holder,  
                   plan.instance[instanceRef].name.in (),
                   inst_type),
      plan_ (plan),
      instanceRef_ (instanceRef)
  {
  }
    
  Instance_Configured::~Instance_Configured (void)
  {
  }
  

  void
  Instance_Configured::invoke_pre_interceptor (Plugin_Manager::INTERCEPTORS::const_iterator &)
  {
    DANCE_TRACE ("Instance_Configured::invoke_pre_interceptor");
    
    //no-op
  }
  
    
  void
  Instance_Configured::invoke (::DAnCE::InstanceDeploymentHandler_ptr handler)
  {
    DANCE_TRACE ("Instance_Configured::invoke");
    
    DANCE_DEBUG (10, (LM_TRACE, DLINFO
                      ACE_TEXT ("Instance_Configured::invoke - ")
                      ACE_TEXT ("Invoking instance_configured on handler for type <%C>\n"),
                      this->instance_type_.c_str ()));
    handler->instance_configured (this->plan_,
                                  this->instanceRef_);
    DANCE_DEBUG (10, (LM_TRACE, DLINFO
                      ACE_TEXT ("Instance_Configured::invoke - ")
                      ACE_TEXT ("instance_configured completed\n")));
  }
  
  
  void
  Instance_Configured::invoke_post_interceptor (Plugin_Manager::INTERCEPTORS::const_iterator &i)
  {
    DANCE_TRACE ("Instance_Configured::invoke_post_interceptor");
    
    (*i)->post_configured (this->plan_,
                                    this->instanceRef_,
                                    this->instance_excep_.in ());
  }
  
  
  void
  Instance_Configured::create_unexpected_exception (const std::string &name,
                                                 const std::string &reason)
  {
    DANCE_TRACE ("Instance_Configured::create_unexpected_exception");
    
    ::Deployment::StartError ex_tmp (name.c_str (),
                                     reason.c_str ());
    this->instance_excep_ =
      DAnCE::Utility::create_any_from_exception (ex_tmp);
  }
  
  void
  Instance_Configured::create_valid_result (Event_Result &)
  {
    DANCE_TRACE ("Instance_Configured::create_valid_result");
    // no-op
  }
}
