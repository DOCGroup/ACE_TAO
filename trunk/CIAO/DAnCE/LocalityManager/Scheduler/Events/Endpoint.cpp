// $Id$
#include "Endpoint.h"

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
  Endpoint_Reference::Endpoint_Reference (::Deployment::DeploymentPlan & plan,
                                          ::CORBA::ULong connectionRef,
                                          const char *inst_type,
                                          Event_Future holder)
    : Deployment_Event (holder,  inst_type),
      plan_ (plan),
      connectionRef_ (connectionRef)
  {
  }
    
  Endpoint_Reference::~Endpoint_Reference (void)
  {
  }
  
  int
  Endpoint_Reference::call (void)
  {
    const char *name = this->plan_.connection[this->connectionRef_].name.in ();
    
    DANCE_DEBUG (10, (LM_TRACE, DLINFO
                      ACE_TEXT ("Endpoint_Reference::call - ")
                      ACE_TEXT ("Entering Endpoint_Reference for connection <%C>\n"),
                      name));;

    try
      {
        if (this->instance_type_.c_str () == 0)
          {
            throw ::Deployment::StartError (name, 
                                           "Invalid instance type\n");
          }

        ::DAnCE::InstanceDeploymentHandler_var handler = 
            PLUGIN_MANAGER::instance ()->fetch_installation_handler (this->instance_type_.c_str ());
        
        if (CORBA::is_nil (handler))
          {
            throw ::Deployment::StartError (name,
                                           "Unable to load appropriate instance handler");
          }
              
        DANCE_DEBUG (10, (LM_TRACE, DLINFO
                          ACE_TEXT ("Endpoint_Reference::call - ")
                          ACE_TEXT ("Invoking activate_instance on handler for type <%C>\n"),
                          this->instance_type_.c_str ()));
        CORBA::Any_var ref;
        handler->provide_endpoint_reference (this->plan_,
                                             this->connectionRef_,
                                             ref.out ());
        DANCE_DEBUG (10, (LM_TRACE, DLINFO
                          ACE_TEXT ("Endpoint_Reference::call - ")
                          ACE_TEXT ("endpoint_reference completed for <%C>\n"),
                          name));
        
        Event_Result result (name, false, ref._retn ());
        this->holder_.set (result);
      }
    catch (CORBA::Exception &ex)
      {
        DANCE_ERROR (3, (LM_ERROR, DLINFO
                         ACE_TEXT ("Endpoint_Reference::call - ")
                         ACE_TEXT ("Caught CORBA exception while getting endpoint ")
                         ACE_TEXT ("%u:<%C>\n"),
                         this->connectionRef_,
                         name));
        
        ::Deployment::InvalidConnection ex_tmp (name,
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
    
    return 0;
  }
}
