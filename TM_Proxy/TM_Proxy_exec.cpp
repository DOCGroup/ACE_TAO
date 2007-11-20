// $Id$
#include "TM_Proxy_exec.h"
#include "ciao/CIAO_common.h"
#include "tao/ORB_Core.h"
#include "orbsvcs/CosNamingC.h"
#include <string>

namespace CIAO
{
  namespace RACE
  {
    namespace TM_Proxy
    {
      namespace CIDL_TM_Proxy_Component_Impl
      {
        TM_Proxy_Component_exec_i::TM_Proxy_Component_exec_i (void)
          : logger_ ("TM_Proxy.log")
        {
        }

        TM_Proxy_Component_exec_i::~TM_Proxy_Component_exec_i (void)
        {
        }
        // Attribute operations.
        char *
          TM_Proxy_Component_exec_i::TM_Name ()
        {
          return CORBA::string_dup (this->TM_Name_.in ());
        }

        void
          TM_Proxy_Component_exec_i::TM_Name (const char * name )
        {
          this->TM_Name_ = CORBA::string_dup (name);
        }

        // Port operations.

        ::CIAO::RACE::TM_Proxy::CCM_SystemSnapshot_ptr
          TM_Proxy_Component_exec_i::get_snapshot ()
        {
          // Your code here.
          return ::CIAO::RACE::TM_Proxy::CCM_SystemSnapshot::_nil ();
        }

        ::CIAO::RACE::TM_Proxy::CCM_Utility_ptr
          TM_Proxy_Component_exec_i::get_utils ()
        {
          // Your code here.
          return ::CIAO::RACE::TM_Proxy::CCM_Utility::_nil ();
        }

        ::CIAO::RACE::TM_Proxy::CCM_ResourceManager_ptr
          TM_Proxy_Component_exec_i::get_manager ()
        {
          // Your code here.
          return ::CIAO::RACE::TM_Proxy::CCM_ResourceManager::_nil ();
        }

        // Operations from Components::SessionComponent

        void
          TM_Proxy_Component_exec_i::set_session_context (
          ::Components::SessionContext_ptr ctx)
        {
          this->context_ =
            TM_Proxy_Component_Context::_narrow (ctx);
          if (CORBA::is_nil (this->context_))
          {
            throw ::CORBA::INTERNAL ();
          }
        }

        void
          TM_Proxy_Component_exec_i::ciao_preactivate ()
        {
          // Your code here.
        }

        void
          TM_Proxy_Component_exec_i::ciao_postactivate ()
        {
          std::string msg = "TM_Proxy_Component_exec_i::"
                            "ciao_postactivate (): ";
          try
          {
            ::CORBA::ORB_ptr orb =
              this->context_->_ciao_the_Container()->the_ORB();

            /// Trying to get the object reference from the Naming Service.
            CORBA::Object_var naming_obj =
              orb->resolve_initial_references ("NameService");
            if (CORBA::is_nil (naming_obj.in ()))
            {
              msg += "Unable to resolve the Name Service.";
              this->logger_.log (msg);
              return;
            }

            CosNaming::NamingContext_var namingContext =
              CosNaming::NamingContext::_narrow (naming_obj.in ());
            CosNaming::Name name (1);
            name.length (1);
            name[0].id = CORBA::string_dup (this->TM_Name_.in ());
            CORBA::Object_var TM = namingContext->resolve (name);

            this->TM_Daemon_ =
              ::Deployment::TargetManager::_narrow (TM.in ());

            if (CORBA::is_nil (this->TM_Daemon_.in ()))
            {
              msg += "Object reference of TM_Daemon is nil!";
              this->logger_.log (msg);
            }
          }
          catch (CORBA::Exception &ex)
          {
            msg += "Exception caught\n";
            msg += ex._info ().c_str();
            msg += "\nUnable to resolve reference to ";
            msg += this->TM_Name_.in ();
            this->logger_.log (msg);
          }
          return;
        }

        void
          TM_Proxy_Component_exec_i::ccm_activate ()
        {
          // Your code here.
        }

        void
          TM_Proxy_Component_exec_i::ccm_passivate ()
        {
          // Your code here.
        }

        void
          TM_Proxy_Component_exec_i::ccm_remove ()
        {
          // Your code here.
        }

        //==================================================================
        // Home Executor Implementation Class:   TM_Proxy_Component_Home_exec_i
        //==================================================================

        TM_Proxy_Component_Home_exec_i::TM_Proxy_Component_Home_exec_i (void)
        {
        }

        TM_Proxy_Component_Home_exec_i::~TM_Proxy_Component_Home_exec_i (void)
        {
        }

        ::Components::EnterpriseComponent_ptr
          TM_Proxy_Component_Home_exec_i::create ()
        {
          ::Components::EnterpriseComponent_ptr retval =
            ::Components::EnterpriseComponent::_nil ();

          ACE_NEW_THROW_EX (
            retval,
            TM_Proxy_Component_exec_i,
            ::CORBA::NO_MEMORY ());

          return retval;
        }

        extern "C" TM_PROXY_EXEC_Export ::Components::HomeExecutorBase_ptr
          create_CIAO_RACE_TM_Proxy_TM_Proxy_Component_Home_Impl (void)
        {
          ::Components::HomeExecutorBase_ptr retval =
            ::Components::HomeExecutorBase::_nil ();

          ACE_NEW_RETURN (
            retval,
            TM_Proxy_Component_Home_exec_i,
            ::Components::HomeExecutorBase::_nil ());

          return retval;
        }
      }
    }
  }
}
