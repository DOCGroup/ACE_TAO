#include "Data_Base_exec.h"
#include "ciao/CIAO_common.h"
#include "Admin_exec.h"
#include "Query_exec.h"

namespace CIAO
{
  namespace RACE
  {
    namespace Data_Base
    {
      namespace CIDL_Data_Base_Component_Impl
      {
        Data_Base_Component_exec_i::Data_Base_Component_exec_i (void)
        {
        }

        Data_Base_Component_exec_i::~Data_Base_Component_exec_i (void)
        {
        }

        ::CIAO::RACE::Data_Base::CCM_Admin_ptr
        Data_Base_Component_exec_i::get_admin ()
        {
          return (new Admin_exec_i (this));
        }

        ::CIAO::RACE::Data_Base::CCM_Query_ptr
        Data_Base_Component_exec_i::get_query ()
        {
          return (new Query_exec_i (this));
        }

        void
        Data_Base_Component_exec_i::set_session_context (
          ::Components::SessionContext_ptr ctx)
        {
          this->context_ =
            Data_Base_Component_Context::_narrow (ctx);

          if (this->context_ == 0)
          {
            throw ::CORBA::INTERNAL ();
          }
        }

        void
        Data_Base_Component_exec_i::ciao_preactivate ()
        {
        }

        void
        Data_Base_Component_exec_i::configuration_complete ()
        {
        }

        void
        Data_Base_Component_exec_i::ccm_activate ()
        {
        }

        void
        Data_Base_Component_exec_i::ccm_passivate ()
        {
        }

        void
        Data_Base_Component_exec_i::ccm_remove ()
        {
        }

        //==================================================================
        // Home Executor Implementation Class:   Data_Base_Component_Home_exec_i
        //==================================================================

        Data_Base_Component_Home_exec_i::Data_Base_Component_Home_exec_i (void)
        {
        }

        Data_Base_Component_Home_exec_i::~Data_Base_Component_Home_exec_i (void)
        {
        }

        // Supported or inherited operations.

        // Home operations.

        // Factory and finder operations.

        // Attribute operations.

        // Implicit operations.

        ::Components::EnterpriseComponent_ptr
        Data_Base_Component_Home_exec_i::create ()
        {
          ::Components::EnterpriseComponent_ptr retval =
            ::Components::EnterpriseComponent::_nil ();

          ACE_NEW_THROW_EX (
            retval,
            Data_Base_Component_exec_i,
            ::CORBA::NO_MEMORY ());

          return retval;
        }

        extern "C" DATA_BASE_EXEC_Export ::Components::HomeExecutorBase_ptr
        create_CIAO_RACE_Data_Base_Data_Base_Component_Home_Impl (void)
        {
          ::Components::HomeExecutorBase_ptr retval =
            ::Components::HomeExecutorBase::_nil ();

          ACE_NEW_RETURN (
            retval,
            Data_Base_Component_Home_exec_i,
            ::Components::HomeExecutorBase::_nil ());

          return retval;
        }
      }
    }
  }
}
