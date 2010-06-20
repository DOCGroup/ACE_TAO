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
    : Deployment_Event (holder, inst_type),
      plan_ (plan),
      connectionRef_ (connectionRef),
      provided_ref_ (provided_ref)
  {
  }
    
  Connect_Instance::~Connect_Instance (void)
  {
  }

  
  int
  Connect_Instance::call (void)
  {
    const char *name = this->plan_.connection[this->connectionRef_].name.in ();
    
    DANCE_DEBUG (10, (LM_TRACE, DLINFO
                      ACE_TEXT ("Connect_Instance::call - ")
                      ACE_TEXT ("Entering Connect_Instance for connection %C\n"),
                      name));
    try
      {
        DANCE_DEBUG (10, (LM_TRACE, DLINFO
                          ACE_TEXT ("Connect_Instance::call - ")
                          ACE_TEXT ("Invoking pre-conneciton interceptors\n")));
        
        const Plugin_Manager::CONN_INTERCEPTORS &conn_int = 
          PLUGIN_MANAGER::instance ()->fetch_connection_interceptors ();

        for (Plugin_Manager::CONN_INTERCEPTORS::const_iterator i = conn_int.begin ();
             i != conn_int.end (); 
             ++i)
          {
            (*i)->instance_pre_connect (this->plan_,
                                        this->connectionRef_,
                                        this->provided_ref_);
          }
        
        ::CORBA::Any_var instance_excep;
                
        try
          {
            ::DAnCE::InstanceDeploymentHandler_var handler = 
              PLUGIN_MANAGER::instance ()->fetch_installation_handler (this->instance_type_.c_str ());
            
            if (CORBA::is_nil (handler))
              {
                throw ::Deployment::StartError (name,
                                               "Unable to load appropriate instance handler");
              }
            
            DANCE_DEBUG (10, (LM_TRACE, DLINFO
                              ACE_TEXT ("Connect_Instance::call - ")
                              ACE_TEXT ("Invoking connect_instance on handler\n")));
            
            handler->connect_instance (this->plan_,
                                       this->connectionRef_,
                                       this->provided_ref_);
            
            DANCE_DEBUG (10, (LM_TRACE, DLINFO
                              ACE_TEXT ("Connect_Instance::call - ")
                              ACE_TEXT ("connect_instance successfull completed\n")));
          }
        catch (CORBA::Exception &ex_tmp)
          {
            DANCE_ERROR (3, (LM_ERROR, DLINFO
                             ACE_TEXT ("Connect_Instance::call - ")
                             ACE_TEXT ("CORBA Exception propagated from connect_instance\n")));
            
            CORBA::Any *tmp = 0;
            ACE_NEW_THROW_EX (tmp,
                              CORBA::Any,
                              CORBA::NO_MEMORY ());
            instance_excep = tmp;
            *tmp <<= ex_tmp;
          }
        catch (...)
          {
            DANCE_ERROR (3, (LM_ERROR, DLINFO
                             ACE_TEXT ("Connect_Instance::call - ")
                             ACE_TEXT ("C++ Exception propagated from connect_instance\n")));
            
            ::Deployment::InvalidConnection ex_tmp (name,
                                                    "Unknown C++ exception\n");
            CORBA::Any *tmp = 0;
            ACE_NEW_THROW_EX (tmp,
                              CORBA::Any,
                              CORBA::NO_MEMORY ());
            instance_excep = tmp;
            *tmp <<= ex_tmp;
          }
        
        DANCE_DEBUG (10, (LM_TRACE, DLINFO
                          ACE_TEXT ("Install_Instance::call - ")
                          ACE_TEXT ("Invoking post-install interceptors\n")));
        for (Plugin_Manager::CONN_INTERCEPTORS::const_iterator i = conn_int.begin ();
             i != conn_int.end ();
             ++i)
          {
            (*i)->instance_post_connect (this->plan_,
                                         this->connectionRef_,
                                         instance_excep.in ());
          }
        
        Event_Result result (name, false);

        this->holder_.set (result);
      }
    catch (CORBA::Exception &ex)
      {
        DANCE_ERROR (1, (LM_ERROR, DLINFO
                         ACE_TEXT ("Connect_Instance::call - ")
                         ACE_TEXT ("Exception propagated out of interceptors for connection <%C>: %C\n"),
                         name,
                         ex._info ().c_str ()));
        
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
