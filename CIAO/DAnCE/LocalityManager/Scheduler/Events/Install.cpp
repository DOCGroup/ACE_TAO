// $Id$
#include "Install.h"

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
  Install_Instance::Install_Instance (::Deployment::DeploymentPlan & plan,
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
    CORBA::Any *tmp;
    ACE_NEW_THROW_EX (tmp, 
                      ::CORBA::Any (),
                      CORBA::NO_MEMORY ());
    
    instance_ = tmp;
  }

  Install_Instance::~Install_Instance (void)
  {
  }
  
  void
  Install_Instance::invoke_pre_interceptor (Plugin_Manager::INTERCEPTORS::const_iterator &i)
  {
    DANCE_TRACE ("Install_Instance::invoke_pre_interceptor");
    
    (*i)->pre_install (this->plan_,
                                this->instanceRef_);
  }
  
    
  void
  Install_Instance::invoke (::DAnCE::InstanceDeploymentHandler_ptr handler)
  {
    DANCE_TRACE ("Install_Instance::invoke");
    
    DANCE_DEBUG (10, (LM_TRACE, DLINFO
                      ACE_TEXT ("Install_Instance::invoke - ")
                      ACE_TEXT ("Invoking install_instance on handler for type <%C>\n"),
                      this->instance_type_.c_str ()));
    
    handler->install_instance (this->plan_,
                               this->instanceRef_,
                               this->instance_.out ());

    DANCE_DEBUG (10, (LM_TRACE, DLINFO
                      ACE_TEXT ("Install_Instance::invoke - ")
                      ACE_TEXT ("install_instance completed\n")));
  }
  
  
  void
  Install_Instance::invoke_post_interceptor (Plugin_Manager::INTERCEPTORS::const_iterator &i)
  {
    DANCE_TRACE ("Install_Instance::invoke_post_interceptor");
    
    (*i)->post_install (this->plan_,
                                 this->instanceRef_,
                                 this->instance_.in (),
                                 this->instance_excep_.in ());
  }
  
  
  void
  Install_Instance::create_unexpected_exception (const std::string &name,
                                                 const std::string &reason)
  {
    DANCE_TRACE ("Install_Instance::create_unexpected_exception");
    
    ::Deployment::StartError ex_tmp (name.c_str (),
                                     reason.c_str ());
    this->instance_excep_ =
      DAnCE::Utility::create_any_from_exception (ex_tmp);
  }
  
  
  void
  Install_Instance::create_valid_result (Event_Result &result)
  {
    result.contents_ = this->instance_._retn ();
  }
}
