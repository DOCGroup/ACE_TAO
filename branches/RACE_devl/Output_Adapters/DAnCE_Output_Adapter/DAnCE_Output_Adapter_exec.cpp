#include "DAnCE_Output_Adapter_exec.h"
#include "Admin_exec.h"
#include "ciao/CIAO_common.h"
#include <string>

namespace CIAO
{
  namespace RACE
  {
    namespace DAnCE_OA
    {
      namespace CIDL_DAnCE_OA_Impl
      {
        DAnCE_OA_Component_exec_i::DAnCE_OA_Component_exec_i (void)
          : logger_ ("DAnCE_OA.log")
        {

        }

        DAnCE_OA_Component_exec_i::~DAnCE_OA_Component_exec_i (void)
        {
        }


        ::CIAO::RACE::DAnCE_OA::CCM_Admin_ptr
        DAnCE_OA_Component_exec_i::get_admin ()
        {
          std::string msg = "DAnCE_OA::Creating the admin object.";
          this->logger_.log (msg);
          return (new Admin_exec_i (this->context_,
                                    this->repoman_id_.in (),
                                    this->logger_));
        }

        void
        DAnCE_OA_Component_exec_i::repoman_id (const char * id)
        {
          this->repoman_id_ = CORBA::string_dup (id);
        }

        char *
        DAnCE_OA_Component_exec_i::repoman_id ()
        {
          return CORBA::string_dup (this->repoman_id_.in ());
        }


        void
        DAnCE_OA_Component_exec_i::set_session_context (
          ::Components::SessionContext_ptr ctx)
        {
          this->context_ = DAnCE_OA_Component_Context::_narrow (ctx);

          if (this->context_ == 0)
            {
              throw ::CORBA::INTERNAL ();
            }
        }

        void
        DAnCE_OA_Component_exec_i::ciao_preactivate ()
        {
        }

        void
        DAnCE_OA_Component_exec_i::configuration_complete ()
        {
        }

        void
        DAnCE_OA_Component_exec_i::ccm_activate ()
        {
        }

        void
        DAnCE_OA_Component_exec_i::ccm_passivate ()
        {
        }

        void
        DAnCE_OA_Component_exec_i::ccm_remove ()
        {
        }

        DAnCE_OA_Component_Home_exec_i::DAnCE_OA_Component_Home_exec_i (void)
        {
        }

        DAnCE_OA_Component_Home_exec_i::~DAnCE_OA_Component_Home_exec_i (void)
        {
        }

        ::Components::EnterpriseComponent_ptr
        DAnCE_OA_Component_Home_exec_i::create ()
        {
          ::Components::EnterpriseComponent_ptr retval =
            ::Components::EnterpriseComponent::_nil ();

          ACE_NEW_THROW_EX (
            retval,
            DAnCE_OA_Component_exec_i,
            ::CORBA::NO_MEMORY ());

          return retval;
        }

        extern "C" DANCE_OUTPUT_ADAPTER_EXEC_Export
        ::Components::HomeExecutorBase_ptr
        create_CIAO_RACE_DAnCE_OA_DAnCE_OA_Component_Home_Impl (void)
        {
          ::Components::HomeExecutorBase_ptr retval =
            ::Components::HomeExecutorBase::_nil ();

          ACE_NEW_RETURN (
            retval,
            DAnCE_OA_Component_Home_exec_i,
            ::Components::HomeExecutorBase::_nil ());

          return retval;
        }
      }
    }
  }
}
