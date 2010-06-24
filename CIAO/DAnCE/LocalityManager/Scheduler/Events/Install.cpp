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
        catch (CORBA::UserException &ex)
          {
            DANCE_ERROR (3, (LM_ERROR, DLINFO
                             ACE_TEXT ("Install_Instance::call - ")
                             ACE_TEXT ("Caught CORBA UserException while installing instance ")
                             ACE_TEXT ("%u:<%C>\n"),
                             this->instanceRef_,
                             name));
            instance_excep = DAnCE::Utility::create_any_from_user_exception (ex);
          }
        catch (CORBA::SystemException &ex)
          {
            DANCE_ERROR (3, (LM_ERROR, DLINFO
                             ACE_TEXT ("Install_Instance::call - ")
                             ACE_TEXT ("Caught CORBA SystemException while installing instance ")
                             ACE_TEXT ("%u:<%C>\n"),
                             this->instanceRef_,
                             name));
            instance_excep = DAnCE::Utility::create_any_from_exception (ex);
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
            instance_excep = DAnCE::Utility::create_any_from_exception (ex_tmp);
          }

        Event_Result result (name, instance_excep.ptr () != 0);
        if (!interceptors.empty ())
          {
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

            result.contents_ = instance_ref._retn ();
            result.exception_ = false;
          }
        else
          {
            DANCE_DEBUG (10, (LM_TRACE, DLINFO
                              ACE_TEXT ("Install_Instance::call - ")
                              ACE_TEXT ("No post-install interceptors; directly propagating result\n")));
            if (result.exception_)
              result.contents_ = instance_excep._retn ();
            else
              result.contents_ = instance_ref._retn ();
          }

        DANCE_DEBUG (10, (LM_TRACE, DLINFO
                          ACE_TEXT ("Install_Instance::call - ")
                          ACE_TEXT ("Signaling result for instance <%C>\n"),
                          name));
        this->holder_.set (result);
      }
    catch (CORBA::UserException &ex)
      {
        DANCE_ERROR (3, (LM_ERROR, DLINFO
                         ACE_TEXT ("Install_Instance::call - ")
                         ACE_TEXT ("CORBA UserException propagated from interceptors for instance ")
                         ACE_TEXT ("%u:<%C>\n"),
                         this->instanceRef_,
                         name));

        Event_Result result (name, true);

        try {
          result.contents_ = DAnCE::Utility::create_any_from_user_exception (ex);
        }
        catch (...) { }

        this->holder_.set (result);
        return -1;
      }
    catch (CORBA::SystemException &ex)
      {
        DANCE_ERROR (3, (LM_ERROR, DLINFO
                         ACE_TEXT ("Install_Instance::call - ")
                         ACE_TEXT ("CORBA SystemException propagated from interceptors for instance ")
                         ACE_TEXT ("%u:<%C>\n"),
                         this->instanceRef_,
                         name));

        Event_Result result (name, true);

        try {
          result.contents_ = DAnCE::Utility::create_any_from_exception (ex);
        }
        catch (...) { }

        this->holder_.set (result);
        return -1;
      }
    return 0;
  }
}
