// $Id$

#include "Deployment_Events.h"
#include "tao/AnyTypeCode/ExceptionA.h"
#include "Logger/Log_Macros.h"
#include "Deployment/Deployment_StartErrorC.h"
#include "DAnCE/DAnCE_DeploymentInterceptorsC.h"
#include "DAnCE/DAnCE_LocalityManagerC.h"
#include "DAnCE/DAnCE_Utility.h"
#include "Plugin_Manager.h"

namespace DAnCE
{
  Deployment_Event::Deployment_Event (Event_Future holder,
                                      const char *inst_type)
    : holder_ (holder),
      instance_type_ (inst_type)
  {
  }

  Install_Instance::Install_Instance (const ::Deployment::DeploymentPlan & plan,
                                      ::CORBA::ULong instanceRef,
                                      const char *inst_type,
                                      Event_Future holder)
    : Deployment_Event (holder, inst_type),
      plan_ (plan),
      instanceRef_ (instanceRef)
  {
  }
    
  Install_Instance::~Install_Instance (void)
  {
  }

  int
  Install_Instance::call (void)
  {
    DANCE_DEBUG (10, (LM_TRACE, DLINFO
                      ACE_TEXT ("Install_Instance::call - ")
                      ACE_TEXT ("Entering Install_Instance\n")));

    const char *name = this->plan_.instance[this->instanceRef_].name.in ();

    try
      {
        const Plugin_Manager::INST_INTERCEPTORS &interceptors = 
          PLUGIN_MANAGER::instance ()->fetch_installation_interceptors ();
        
        DANCE_DEBUG (10, (LM_TRACE, DLINFO
                          ACE_TEXT ("Install_Instance::call - ")
                          ACE_TEXT ("Invoking pre-install interceptors\n")));
        for (Plugin_Manager::INST_INTERCEPTORS::const_iterator i = interceptors.begin ();
             i != interceptors.end ();
             ++i)
          {
            (*i)->instance_pre_install (this->plan_,
                                        this->instanceRef_);
          }
        
        ::Deployment::MonolithicDeploymentDescription &mdd = 
            this->plan_.implementation[this->plan_.instance[this->instanceRef_].implementationRef];

        const char *inst_type = 
          DAnCE::Utility::get_instance_type (mdd.execParameter);
        
        if (inst_type == 0)
          {
            throw ::Deployment::PlanError (name, 
                                           "Invalid instance type\n");
          }

        ::DAnCE::InstanceDeploymentHandler_var handler = 
            PLUGIN_MANAGER::instance ()->fetch_installation_handler (inst_type);
        
        if (CORBA::is_nil (handler))
          {
            throw ::Deployment::StartError (name,
                                            "Unable to load appropriate instance handler");
          }
              
        ::CORBA::Any_var instance_ref;
        ::CORBA::Any_var instance_excep;

        try
          {
            DANCE_DEBUG (10, (LM_TRACE, DLINFO
                              ACE_TEXT ("Install_Instance::call - ")
                              ACE_TEXT ("Invoking install_instance on handler for type <%C>\n"),
                              inst_type));
            handler->install_instance (this->plan_,
                                       this->instanceRef_,
                                       instance_ref.out ());
            DANCE_DEBUG (10, (LM_TRACE, DLINFO
                              ACE_TEXT ("Install_Instance::call - ")
                              ACE_TEXT ("install_instance completed\n")));
          }
        catch (CORBA::Exception &ex)
          {
            DANCE_ERROR (3, (LM_ERROR, DLINFO
                             ACE_TEXT ("Install_Instance::call - ")
                             ACE_TEXT ("Caught CORBA exception while installing instance ")
                             ACE_TEXT ("%u:<%C>\n"),
                             this->instanceRef_,
                             name));
            CORBA::Any *tmp = 0;
            ACE_NEW_THROW_EX (tmp,
                              CORBA::Any,
                              CORBA::NO_MEMORY ());
            instance_excep = tmp;
            *tmp <<= ex;
          }
        catch (...)
          {
            DANCE_ERROR (3, (LM_ERROR, DLINFO
                             ACE_TEXT ("Install_Instance::call - ")
                             ACE_TEXT ("Caught C++ exception while installing instance ")
                             ACE_TEXT ("%u:<%C>\n"),
                             this->instanceRef_,
                             name));

            ::Deployment::StartError ex_tmp (name,
                                             "Caught unknown C++ exception from install");
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
        for (Plugin_Manager::INST_INTERCEPTORS::const_iterator i = interceptors.begin ();
             i != interceptors.end ();
             ++i)
          {
            (*i)->instance_post_install (this->plan_,
                                         this->instanceRef_,
                                         instance_ref.in (),
                                         instance_excep.in ());
          }
        
        Event_Result result;
        result.id_ = name;
        result.exception_ = false;
        result.contents_ = instance_ref._retn ();
        
        DANCE_DEBUG (10, (LM_TRACE, DLINFO
                          ACE_TEXT ("Install_Instance::call - ")
                          ACE_TEXT ("Signaling result for instance <%C>\n"),
                          name));
        this->holder_.set (result);
      }
    catch (CORBA::Exception &ex)
      {
        DANCE_ERROR (3, (LM_ERROR, DLINFO
                         ACE_TEXT ("Install_Instance::call - ")
                         ACE_TEXT ("CORBA Exception propagated from interceptors for instance ")
                         ACE_TEXT ("%u:<%C>\n"),
                         this->instanceRef_,
                         name));
        
        Event_Result result;
        result.id_ = name;
        result.exception_ = true;
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

  Connect_Instance::Connect_Instance (const ::Deployment::DeploymentPlan & plan,
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
        
        Event_Result result;
        result.id_ = name;
        result.exception_ = false;
        
        this->holder_.set (result);
      }
    catch (CORBA::Exception &ex)
      {
        DANCE_ERROR (1, (LM_ERROR, DLINFO
                         ACE_TEXT ("Connect_Instance::call - ")
                         ACE_TEXT ("Exception propagated out of interceptors for connection <%C>: %C\n"),
                         name,
                         ex._info ().c_str ()));
        
        Event_Result result;
        result.id_ = name;
        result.exception_ = true;
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

  
  Remove_Instance::Remove_Instance (::Deployment::DeploymentPlan & plan,
                                    ::CORBA::ULong instanceRef,
                                    const CORBA::Any &reference,
                                    const char *inst_type,
                                    Event_Future holder)
    : Deployment_Event (holder,  inst_type),
      plan_ (plan),
      instanceRef_ (instanceRef),
      reference_ (reference)
  {
  }
    
  Remove_Instance::~Remove_Instance (void)
  {
  }
  
  int
  Remove_Instance::call (void)
  {
    DANCE_DEBUG (10, (LM_TRACE, DLINFO
                      ACE_TEXT ("Install_Instance::call - ")
                      ACE_TEXT ("Entering Install_Instance\n")));
    
    const char *name = this->plan_.instance[this->instanceRef_].name.in ();
    
    try
      {
        ::Deployment::MonolithicDeploymentDescription &mdd = 
          this->plan_.implementation[this->plan_.instance[this->instanceRef_].implementationRef];

        const char *inst_type = 
          DAnCE::Utility::get_instance_type (mdd.execParameter);
        
        if (inst_type == 0)
          {
            throw ::Deployment::StopError (name, 
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
                          ACE_TEXT ("Remove_Instance::call - ")
                          ACE_TEXT ("Invoking remove_instance on handler for type <%C>\n"),
                          inst_type));
        handler->remove_instance (this->plan_,
                                  this->instanceRef_,
                                  this->reference_);
        DANCE_DEBUG (10, (LM_TRACE, DLINFO
                          ACE_TEXT ("Remove_Instance::call - ")
                          ACE_TEXT ("remove_instance completed\n")));
        
      }
    catch (CORBA::Exception &ex)
      {
        DANCE_ERROR (3, (LM_ERROR, DLINFO
                         ACE_TEXT ("Remove_Instance::call - ")
                         ACE_TEXT ("Caught C++ exception while removeing instance ")
                         ACE_TEXT ("%u:<%C>\n"),
                         this->instanceRef_,
                         name));
        
        ::Deployment::StopError ex_tmp (name,
                                        ex._info ().c_str ());
        Event_Result result;
        result.id_ = name;
        result.exception_ = true;
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
    
    Event_Result result;
    result.id_ = name;
    result.exception_ = false;
    this->holder_.set (result);
    
    return 0;
  }
}
