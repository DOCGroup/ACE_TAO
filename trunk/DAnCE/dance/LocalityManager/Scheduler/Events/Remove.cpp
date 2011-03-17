// $Id$
#include "Remove.h"

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
  Remove_Instance::Remove_Instance (::Deployment::DeploymentPlan & plan,
                                    ::CORBA::ULong instanceRef,
                                    const CORBA::Any &reference,
                                    const char *inst_type,
                                    Event_Future holder)
    : Deployment_Event (holder,
                        plan.instance[instanceRef].name.in (),
                        inst_type),
      Action_Base (holder,
                   plan.instance[instanceRef].name.in (),
                   inst_type),
      plan_ (plan),
      instanceRef_ (instanceRef),
      reference_ (reference)
  {
  }

  Remove_Instance::~Remove_Instance (void)
  {
  }

  void
  Remove_Instance::invoke_pre_interceptor (Plugin_Manager::INTERCEPTORS::const_iterator &)
  {
    DANCE_TRACE ("Remove_Instance::invoke_pre_interceptor");

    //no-op
  }


  void
  Remove_Instance::invoke (::DAnCE::InstanceDeploymentHandler_ptr handler)
  {
    DANCE_TRACE ("Remove_Instance::invoke");

    DANCE_DEBUG (DANCE_LOG_TRACE,
                 (LM_TRACE, DLINFO
                  ACE_TEXT ("Remove_Instance::invoke - ")
                  ACE_TEXT ("Invoking remove_instance on handler for type <%C>\n"),
                  this->instance_type_.c_str ()));
    handler->remove_instance (this->plan_,
                              this->instanceRef_,
                              this->reference_);
    DANCE_DEBUG (DANCE_LOG_DETAILED_TRACE,
                 (LM_TRACE, DLINFO
                  ACE_TEXT ("Remove_Instance::invoke - ")
                  ACE_TEXT ("remove_instance completed\n")));
  }


  void
  Remove_Instance::invoke_post_interceptor (Plugin_Manager::INTERCEPTORS::const_iterator &i)
  {
    DANCE_TRACE ("Remove_Instance::invoke_post_interceptor");

    (*i)->post_remove (this->plan_,
                       this->instanceRef_,
                       this->instance_excep_.in ());
  }


  void
  Remove_Instance::create_unexpected_exception (const std::string &name,
                                                 const std::string &reason)
  {
    DANCE_TRACE ("Remove_Instance::create_unexpected_exception");

    ::Deployment::StopError ex_tmp (name.c_str (),
                                    reason.c_str ());
    this->instance_excep_ =
      DAnCE::Utility::create_any_from_exception (ex_tmp);
  }

  void
  Remove_Instance::create_valid_result (Event_Result &)
  {
    DANCE_TRACE ("Remove_Instance::create_valid_result");
    // no-op
  }
}
