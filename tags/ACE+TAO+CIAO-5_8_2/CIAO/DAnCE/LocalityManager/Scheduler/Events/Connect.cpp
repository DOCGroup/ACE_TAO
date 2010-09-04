// $Id$
#include "Connect.h"

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
  Connect_Instance::Connect_Instance (::Deployment::DeploymentPlan & plan,
                                      ::CORBA::ULong connectionRef,
                                      const ::CORBA::Any &provided_ref,
                                      const char *inst_type,
                                      Event_Future holder)
    : Deployment_Event (holder, 
                        plan.connection[connectionRef].name.in (),
                        inst_type),
      Action_Base (holder, 
                   plan.connection[connectionRef].name.in (),
                   inst_type),
      plan_ (plan),
      connectionRef_ (connectionRef),
      provided_ref_ (provided_ref)
  {
  }
    
  Connect_Instance::~Connect_Instance (void)
  {
  }


  void
  Connect_Instance::invoke_pre_interceptor (Plugin_Manager::INTERCEPTORS::const_iterator &i)
  {
    DANCE_TRACE ("Connect_Instance::invoke_pre_interceptor");
    
    (*i)->pre_connect (this->plan_,
                                this->connectionRef_,
                                this->provided_ref_);
  }
  
    
  void
  Connect_Instance::invoke (::DAnCE::InstanceDeploymentHandler_ptr handler)
  {
    DANCE_TRACE ("Connect_Instance::invoke");
    
    DANCE_DEBUG (10, (LM_TRACE, DLINFO
                      ACE_TEXT ("Connect_Instance::invoke - ")
                      ACE_TEXT ("Invoking connect_instance on handler\n")));
    
    handler->connect_instance (this->plan_,
                               this->connectionRef_,
                               this->provided_ref_);
    
    DANCE_DEBUG (10, (LM_TRACE, DLINFO
                      ACE_TEXT ("Connect_Instance::invoke - ")
                      ACE_TEXT ("connect_instance successfull completed\n")));
  }
  
  
  void
  Connect_Instance::invoke_post_interceptor (Plugin_Manager::INTERCEPTORS::const_iterator &i)
  {
    DANCE_TRACE ("Connect_Instance::invoke_post_interceptor");
    
    (*i)->post_connect (this->plan_,
                                 this->connectionRef_,
                                 this->instance_excep_.in ());
  }
  
  
  void
  Connect_Instance::create_unexpected_exception (const std::string &name,
                                                 const std::string &reason)
  {
    DANCE_TRACE ("Connect_Instance::create_unexpected_exception");
    
    ::Deployment::InvalidConnection ex_tmp (name.c_str (),
                                            reason.c_str ());
    this->instance_excep_ =
      DAnCE::Utility::create_any_from_exception (ex_tmp);
  }
  
  void
  Connect_Instance::create_valid_result (Event_Result &)
  {
    DANCE_TRACE ("Connect_Instance::create_valid_result");
    // no-op
  }
}
