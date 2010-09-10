// $Id$
#include "Action_Base.h"

#include "tao/AnyTypeCode/ExceptionA.h"
#include "DAnCE/Logger/Log_Macros.h"
#include "DAnCE/DAnCE_Utility.h"

namespace DAnCE
{
  Action_Base::Action_Base (Event_Future holder,
                                        const char *name,
                                        const char *instance_type)
    : Deployment_Event (holder, name, instance_type)
  {
    CORBA::Any *tmp;
    ACE_NEW_THROW_EX (tmp, 
                      ::CORBA::Any (),
                      CORBA::NO_MEMORY ());
    instance_excep_ = tmp;
  }
  
  Action_Base::~Action_Base (void)
  {
    
  }
  
  int
  Action_Base::call (void)
  {
    DANCE_DEBUG (10, (LM_TRACE, DLINFO
                      ACE_TEXT ("Action_Base::call - ")
                      ACE_TEXT ("Entering Action_Base\n")));

    try
      {
        const Plugin_Manager::INTERCEPTORS &interceptors =
          PLUGIN_MANAGER::instance ()->fetch_interceptors ();

        DANCE_DEBUG (10, (LM_TRACE, DLINFO
                          ACE_TEXT ("Action_Base::call - ")
                          ACE_TEXT ("Invoking pre-install interceptors\n")));
        for (Plugin_Manager::INTERCEPTORS::const_iterator i = interceptors.begin ();
             i != interceptors.end ();
             ++i)
          {
            this->invoke_pre_interceptor (i);
          }

        if (this->instance_type_.c_str () == 0)
          {
            this->create_unexpected_exception (this->name_,
                                               "Invalid Instance Type");
          }

        ::DAnCE::InstanceDeploymentHandler_var handler =
            PLUGIN_MANAGER::instance ()->fetch_installation_handler (this->instance_type_.c_str ());

        if (CORBA::is_nil (handler))
          {
            this->create_unexpected_exception (this->name_,
                                               "Unable to load appropriate instance handler");
          }

        try
          {
            this->invoke (handler.in ());
          }
        catch (CORBA::UserException &ex)
          {
            DANCE_ERROR (3, (LM_ERROR, DLINFO
                             ACE_TEXT ("Action_Base::call - ")
                             ACE_TEXT ("Caught CORBA UserException while processing instance ")
                             ACE_TEXT ("<%C>\n"),
                             this->name_.c_str ()));
            this->instance_excep_ = DAnCE::Utility::create_any_from_user_exception (ex);
          }
        catch (CORBA::SystemException &ex)
          {
            DANCE_ERROR (3, (LM_ERROR, DLINFO
                             ACE_TEXT ("Action_Base::call - ")
                             ACE_TEXT ("Caught CORBA SystemException while processing instance ")
                             ACE_TEXT ("<%C>\n"),
                             this->name_.c_str ()));
            this->instance_excep_ = DAnCE::Utility::create_any_from_exception (ex);
          }
        catch (...)
          {
            DANCE_ERROR (3, (LM_ERROR, DLINFO
                             ACE_TEXT ("Action_Base::call - ")
                             ACE_TEXT ("Caught C++ exception while processing instance ")
                             ACE_TEXT ("<%C>\n"),
                             this->name_.c_str ()));
            
            this->create_unexpected_exception (this->name_,
                                               "Caught unknown C++ exception from install");
          }

        Event_Result result (this->name_, this->instance_excep_.ptr () != 0);
        if (!interceptors.empty ())
          {
            DANCE_DEBUG (10, (LM_TRACE, DLINFO
                              ACE_TEXT ("Action_Base::call - ")
                              ACE_TEXT ("Invoking post-action interceptors\n")));
            for (Plugin_Manager::INTERCEPTORS::const_iterator i = interceptors.begin ();
                i != interceptors.end ();
                ++i)
              {
                this->invoke_post_interceptor (i);
              }
            
            this->create_valid_result (result);
            result.exception_ = false;
          }
        else
          {
            DANCE_DEBUG (10, (LM_TRACE, DLINFO
                              ACE_TEXT ("Action_Base::call - ")
                              ACE_TEXT ("No post-install interceptors; directly propagating result\n")));
            if (result.exception_)
              result.contents_ = this->instance_excep_._retn ();
            else
              this->create_valid_result (result);
          }

        DANCE_DEBUG (10, (LM_TRACE, DLINFO
                          ACE_TEXT ("Action_Base::call - ")
                          ACE_TEXT ("Signaling result for instance <%C>\n"),
                          this->name_.c_str ()));
        this->holder_.set (result);
      }
    catch (CORBA::UserException &ex)
      {
        DANCE_ERROR (3, (LM_ERROR, DLINFO
                         ACE_TEXT ("Action_Base::call - ")
                         ACE_TEXT ("CORBA UserException propagated from interceptors for instance ")
                         ACE_TEXT ("<%C>\n"),
                         this->name_.c_str ()));

        Event_Result result (this->name_, true);

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
                         ACE_TEXT ("Action_Base::call - ")
                         ACE_TEXT ("CORBA SystemException propagated from interceptors for instance ")
                         ACE_TEXT ("<%C>\n"),
                         this->name_.c_str ()));

        Event_Result result (this->name_, true);

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
