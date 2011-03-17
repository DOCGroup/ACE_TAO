// $Id$
#include "Configured.h"

#include "tao/AnyTypeCode/ExceptionA.h"
#include "dance/Logger/Log_Macros.h"
#include "dance/Deployment/Deployment_StartErrorC.h"
#include "dance/DAnCE_DeploymentInterceptorsC.h"
#include "dance/DAnCE_LocalityManagerC.h"
#include "dance/DAnCE_Utility.h"
#include "dance/LocalityManager/Scheduler/Deployment_Event.h"
#include "dance/LocalityManager/Scheduler/Plugin_Manager.h"

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

    DANCE_DEBUG (DANCE_LOG_TRACE,
                 (LM_TRACE, DLINFO
                  ACE_TEXT ("Instance_Configured::invoke - ")
                  ACE_TEXT ("Invoking instance_configured on handler for type <%C>\n"),
                  this->instance_type_.c_str ()));
    handler->instance_configured (this->plan_,
                                  this->instanceRef_);
    DANCE_DEBUG (DANCE_LOG_DETAILED_TRACE,
                 (LM_TRACE, DLINFO
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
