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
    : Deployment_Event (holder,  inst_type),
      plan_ (plan),
      instanceRef_ (instanceRef)
  {
  }
    
  Instance_Configured::~Instance_Configured (void)
  {
  }
  
  int
  Instance_Configured::call (void)
  {
    DANCE_DEBUG (10, (LM_TRACE, DLINFO
                      ACE_TEXT ("Instance_Configured::call - ")
                      ACE_TEXT ("Entering Instance_Configured\n")));
    
    const char *name = this->plan_.instance[this->instanceRef_].name.in ();
    
    try
      {
        ::Deployment::MonolithicDeploymentDescription &mdd = 
          this->plan_.implementation[this->plan_.instance[this->instanceRef_].implementationRef];

        const char *inst_type = 
          DAnCE::Utility::get_instance_type (mdd.execParameter);
        
        if (inst_type == 0)
          {
            throw ::Deployment::StartError (name, 
                                           "Invalid instance type\n");
          }

        ::DAnCE::InstanceDeploymentHandler_var handler = 
            PLUGIN_MANAGER::instance ()->fetch_installation_handler (inst_type);
        
        if (CORBA::is_nil (handler))
          {
            throw ::Deployment::StartError (name,
                                           "Unable to load appropriate instance handler");
          }
              
        DANCE_DEBUG (10, (LM_TRACE, DLINFO
                          ACE_TEXT ("Instance_Configured::call - ")
                          ACE_TEXT ("Invoking activate_instance on handler for type <%C>\n"),
                          inst_type));
        handler->instance_configured (this->plan_,
                                      this->instanceRef_);
        DANCE_DEBUG (10, (LM_TRACE, DLINFO
                          ACE_TEXT ("Instance_Configured::call - ")
                          ACE_TEXT ("instance_configured completed\n")));
        
      }
    catch (CORBA::Exception &ex)
      {
        DANCE_ERROR (3, (LM_ERROR, DLINFO
                         ACE_TEXT ("Instance_Configured::call - ")
                         ACE_TEXT ("Caught CORBA exception while starting instance ")
                         ACE_TEXT ("%u:<%C>\n"),
                         this->instanceRef_,
                         name));
        
        ::Deployment::StartError ex_tmp (name,
                                         ex._info ().c_str ());
        Event_Result result (name, true);

        CORBA::Any *tmp = 0;
        ACE_NEW_NORETURN (tmp,
                          CORBA::Any);
        
        if (tmp)
          {
            result.contents_ = tmp;
            *tmp <<= ex;
          }

        this->holder_.set (result);
        return -1;
      }
    
    Event_Result result (name, false);

    this->holder_.set (result);
    
    return 0;
  }
}
