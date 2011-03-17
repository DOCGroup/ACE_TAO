// $Id$
#include "Endpoint.h"

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
  Endpoint_Reference::Endpoint_Reference (::Deployment::DeploymentPlan & plan,
                                          ::CORBA::ULong connectionRef,
                                          const char *inst_type,
                                          Event_Future holder)
    : Deployment_Event (holder,
                        plan.connection[connectionRef].name.in (),
                        inst_type),
      Action_Base (holder,
                   plan.connection[connectionRef].name.in (),
                   inst_type),
      plan_ (plan),
      connectionRef_ (connectionRef)
  {
    CORBA::Any *tmp;
    ACE_NEW_THROW_EX (tmp,
                      ::CORBA::Any (),
                      CORBA::NO_MEMORY ());
    ref_ = tmp;
  }

  Endpoint_Reference::~Endpoint_Reference (void)
  {
  }

  void
  Endpoint_Reference::invoke_pre_interceptor (Plugin_Manager::INTERCEPTORS::const_iterator &)
  {
    DANCE_TRACE ("Endpoint_Reference::invoke_pre_interceptor");

    // no-op
  }


  void
  Endpoint_Reference::invoke (::DAnCE::InstanceDeploymentHandler_ptr handler)
  {
    DANCE_TRACE ("Endpoint_Reference::invoke");

    DANCE_DEBUG (DANCE_LOG_TRACE,
                 (LM_TRACE, DLINFO
                  ACE_TEXT ("Endpoint_Reference::invoke - ")
                  ACE_TEXT ("Invoking provide_endpoint_reference on handler\n")));

    handler->provide_endpoint_reference (this->plan_,
                                         this->connectionRef_,
                                         this->ref_.out ());
    DANCE_DEBUG (DANCE_LOG_DETAILED_TRACE,
                 (LM_TRACE, DLINFO
                  ACE_TEXT ("Endpoint_Reference::invoke - ")
                  ACE_TEXT ("provide_endpoint_reference successful completed\n")));
  }


  void
  Endpoint_Reference::invoke_post_interceptor (Plugin_Manager::INTERCEPTORS::const_iterator &i)
  {
    DANCE_TRACE ("Endpoint_Reference::invoke_post_interceptor");

    (*i)->post_endpoint_reference (this->plan_,
                                   this->connectionRef_,
                                   this->ref_.in (),
                                   this->instance_excep_.in ());
  }


  void
  Endpoint_Reference::create_unexpected_exception (const std::string &name,
                                                 const std::string &reason)
  {
    DANCE_TRACE ("Endpoint_Reference::create_unexpected_exception");

    ::Deployment::StartError ex_tmp (name.c_str (),
                                     reason.c_str ());
    this->instance_excep_ =
      DAnCE::Utility::create_any_from_exception (ex_tmp);
  }

  void
  Endpoint_Reference::create_valid_result (Event_Result &result)
  {
    DANCE_TRACE ("Endpoint_Reference::create_valid_result");
    result.contents_ = this->ref_._retn ();
    // no-op
  }
}
