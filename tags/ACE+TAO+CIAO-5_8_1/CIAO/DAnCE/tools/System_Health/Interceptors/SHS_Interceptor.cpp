// $Id$

#include "SHS_Interceptor.h"
#include "Deployment/Deployment_StartErrorC.h"
#include "DAnCE/DAnCE_PropertiesC.h"
#include "DAnCE/DAnCE_Utility.h"
#include "DAnCE/Logger/Log_Macros.h"
#include "System_Health/SHS_DataModelC.h"
#include "System_Health/Interceptors/SHS_CORBA_Transport.h"

namespace DAnCE
{
  // Implementation skeleton constructor
  SHS_Interceptor::SHS_Interceptor (void)
  {
  }

  // Implementation skeleton destructor
  SHS_Interceptor::~SHS_Interceptor (void)
  {
  }

  void
  SHS_Interceptor::configure (const ::Deployment::Properties &props )
  {
    DAnCE::SHS::SHS_Transport *tmp (0);
    
    ACE_NEW_THROW_EX (tmp,
                      SHS::SHS_CORBA_Transport (),
                      CORBA::NO_MEMORY ());
    
    this->shs_transport_.reset (tmp);
    
    this->shs_transport_->configure (props);
  }
  
  void
  SHS_Interceptor::instance_post_install (const ::Deployment::DeploymentPlan &plan,
                                          ::CORBA::ULong instance_index,
                                          const ::CORBA::Any &/*reference*/,
                                          const ::CORBA::Any &exception)
  {
    ::DAnCE::SHS::Status_Update update;
    CORBA::ULong mdd_idx = plan.instance[instance_index].implementationRef;

    update.id = plan.instance[instance_index].name.in ();
    update.type = 
      DAnCE::Utility::get_instance_type (plan.implementation[mdd_idx].execParameter);
    
    if (exception.type() != ::CORBA::_tc_null)
      {
        std::string result;
        DAnCE::Utility::stringify_exception_from_any (exception,
                                                      result);
        
        update.new_status = DAnCE::SHS::INST_ERROR;
        update.instance_info.length (1);
        update.instance_info[0].name = ::DAnCE::SHS::Constants::SHS_DIAGNOSTIC;
        update.instance_info[0].value <<= CORBA::Any::from_string (result.c_str (), 0);
      }
    else
      {
        update.new_status = DAnCE::SHS::INST_INSTALLED;
      }
    
    if (this->shs_transport_.get ())
      this->shs_transport_->push_event (update);
  }

  void
  SHS_Interceptor::instance_post_connect (const ::Deployment::DeploymentPlan &plan,
                                         ::CORBA::ULong connection,
                                         const ::CORBA::Any &exception)
  {
    ::DAnCE::SHS::Status_Update update;

    update.id = plan.connection[connection].name.in ();
    update.type = DAnCE::SHS::Constants::SHS_CONNECTION;
      
    if (exception.type() != ::CORBA::_tc_null)
      {
        std::string result;
        DAnCE::Utility::stringify_exception_from_any (exception,
                                                      result);
        
        update.new_status = DAnCE::SHS::INST_ERROR;
        update.instance_info.length (1);
        update.instance_info[0].name = ::DAnCE::SHS::Constants::SHS_DIAGNOSTIC;
        update.instance_info[0].value <<= CORBA::Any::from_string (result.c_str (), 0);
      }
    else
      {
        update.new_status = DAnCE::SHS::INST_ACTIVE;
      }

    if (this->shs_transport_.get ())
      this->shs_transport_->push_event (update);
  }
  
  void 
  SHS_Interceptor::instance_post_configured (const ::Deployment::DeploymentPlan & plan,
                                            ::CORBA::ULong instance_index,
                                            const ::CORBA::Any &exception )
  {
    ::DAnCE::SHS::Status_Update update;
    CORBA::ULong mdd_idx = plan.instance[instance_index].implementationRef;

    update.id = plan.instance[instance_index].name.in ();
    update.type = 
      DAnCE::Utility::get_instance_type (plan.implementation[mdd_idx].execParameter);
    
    if (exception.type() != ::CORBA::_tc_null)
      {
        std::string result;
        DAnCE::Utility::stringify_exception_from_any (exception,
                                                      result);
        
        update.new_status = DAnCE::SHS::INST_ERROR;
        update.instance_info.length (1);
        update.instance_info[0].name = ::DAnCE::SHS::Constants::SHS_DIAGNOSTIC;
        update.instance_info[0].value <<= CORBA::Any::from_string (result.c_str (), 0);
      }
    else
      {
        update.new_status = DAnCE::SHS::INST_PASSIVE;
      }

    if (this->shs_transport_.get ())
      this->shs_transport_->push_event (update);
  }
  
  
  void
  SHS_Interceptor::instance_post_activate (const ::Deployment::DeploymentPlan & plan,
                                       ::CORBA::ULong instance_index,
                                       const ::CORBA::Any & exception)
  {
    ::DAnCE::SHS::Status_Update update;
    CORBA::ULong mdd_idx = plan.instance[instance_index].implementationRef;

    update.id = plan.instance[instance_index].name.in ();
    update.type = 
      DAnCE::Utility::get_instance_type (plan.implementation[mdd_idx].execParameter);
    
    if (exception.type() != ::CORBA::_tc_null)
      {
        std::string result;
        DAnCE::Utility::stringify_exception_from_any (exception,
                                                      result);
        
        update.new_status = DAnCE::SHS::INST_ERROR;
        update.instance_info.length (1);
        update.instance_info[0].name = ::DAnCE::SHS::Constants::SHS_DIAGNOSTIC;
        update.instance_info[0].value <<= CORBA::Any::from_string (result.c_str (), 0);
      }
    else
      {
        update.new_status = DAnCE::SHS::INST_ACTIVE;
      }

    if (this->shs_transport_.get ())
      this->shs_transport_->push_event (update);
  }
  
  
  void
  SHS_Interceptor::instance_post_passivate (const ::Deployment::DeploymentPlan & plan,
                                        ::CORBA::ULong instance_index,
                                        const ::CORBA::Any & exception)
  {
    ::DAnCE::SHS::Status_Update update;
    CORBA::ULong mdd_idx = plan.instance[instance_index].implementationRef;

    update.id = plan.instance[instance_index].name.in ();
    update.type = 
      DAnCE::Utility::get_instance_type (plan.implementation[mdd_idx].execParameter);
    
    if (exception.type() != ::CORBA::_tc_null)
      {
        std::string result;
        DAnCE::Utility::stringify_exception_from_any (exception,
                                                      result);
        
        update.new_status = DAnCE::SHS::INST_ERROR;
        update.instance_info.length (1);
        update.instance_info[0].name = ::DAnCE::SHS::Constants::SHS_DIAGNOSTIC;
        update.instance_info[0].value <<= CORBA::Any::from_string (result.c_str (), 0);
      }
    else
      {
        update.new_status = DAnCE::SHS::INST_PASSIVE;
      }

    if (this->shs_transport_.get ())
      this->shs_transport_->push_event (update);
  }
  
  
  void
  SHS_Interceptor::instance_post_remove (const ::Deployment::DeploymentPlan & plan,
                                     ::CORBA::ULong instance_index,
                                     const ::CORBA::Any & exception)
  {
    ::DAnCE::SHS::Status_Update update;
    CORBA::ULong mdd_idx = plan.instance[instance_index].implementationRef;

    update.id = plan.instance[instance_index].name.in ();
    update.type = 
      DAnCE::Utility::get_instance_type (plan.implementation[mdd_idx].execParameter);
    
    if (exception.type() != ::CORBA::_tc_null)
      {
        std::string result;
        DAnCE::Utility::stringify_exception_from_any (exception,
                                                      result);
        
        update.new_status = DAnCE::SHS::INST_ERROR;
        update.instance_info.length (1);
        update.instance_info[0].name = ::DAnCE::SHS::Constants::SHS_DIAGNOSTIC;
        update.instance_info[0].value <<= CORBA::Any::from_string (result.c_str (), 0);
      }
    else
      {
        update.new_status = DAnCE::SHS::INST_DEFUNCT;
      }

    if (this->shs_transport_.get ())
      this->shs_transport_->push_event (update);
  }
 }

extern "C"
{
  ::DAnCE::DeploymentInterceptor_ptr 
  create_DAnCE_SHS_Interceptor (void)
  {
    return new DAnCE::SHS_Interceptor ();
  }
}
