// $Id:$
#include "TM_Proxy_exec.h"
#include "ciao/CIAO_common.h"

namespace CIAO
{
  namespace RACE
  {
    namespace TM_Proxy
    {
      namespace CIDL_TM_Proxy_Component_Impl
      {
        TM_Proxy_Component_exec_i::TM_Proxy_Component_exec_i (void)
        {
        }

        TM_Proxy_Component_exec_i::~TM_Proxy_Component_exec_i (void)
        {
        }
        // Attribute operations.
        char *
        TM_Proxy_Component_exec_i::TM_Name ()
        {
          return 0;
        }

        void
        TM_Proxy_Component_exec_i::TM_Name (const char * /* name */ )
        {}

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
            ::CIAO::RACE::TM_Proxy::CCM_TM_Proxy_Component_Context::_narrow (ctx);

          if (CORBA::is_nil (this->context_.in ()))
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
          // Your code here.
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
