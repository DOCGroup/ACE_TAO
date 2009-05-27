// $Id$
#include "EUCON_Controller_exec.h"
#include "ciao/CIAO_common.h"
#include "Admin_exec.h"
#include "Operations_exec.h"

namespace CIAO
{
  namespace RACE
  {
    namespace Controller
    {
      namespace CIDL_EUCON_Controller_Impl
      {
        EUCON_Controller_exec_i::EUCON_Controller_exec_i (void)
        {
        }

        EUCON_Controller_exec_i::~EUCON_Controller_exec_i (void)
        {
        }

        ::CORBA::Long
        EUCON_Controller_exec_i::sampling_period ()
        {
          return this->sampling_period_;          
        }

        void
        EUCON_Controller_exec_i::sampling_period (::CORBA::Long
                                                 sampling_period)
        {
          this->sampling_period_ = sampling_period;
        }

        // Port operations.

        ::CIAO::RACE::Controller::CCM_Control_Operations_ptr
        EUCON_Controller_exec_i::get_operations ()
        {
          return new Control_Operations_exec_i (this->controller_);
        }

        ::CIAO::RACE::Controller::CCM_Admin_ptr
        EUCON_Controller_exec_i::get_admin ()
        {
          return new Admin_exec_i (this->controller_);
        }

        // Operations from Components::SessionComponent

        void
        EUCON_Controller_exec_i::set_session_context (
          ::Components::SessionContext_ptr ctx)
        {
          this->context_ =
            ::CIAO::RACE::Controller::CCM_EUCON_Controller_Context::
            _narrow (ctx);

          if (CORBA::is_nil (this->context_.in ()))
          {
            throw ::CORBA::INTERNAL ();
          }
        }

        void
        EUCON_Controller_exec_i::ciao_preactivate ()
        {
          // Your code here.
        }

        void
        EUCON_Controller_exec_i::configuration_complete ()
        {
          this->controller_ = new Controller (this->context_,
                                              this->sampling_period_);

          // Your code here.
        }

        void
        EUCON_Controller_exec_i::ccm_activate ()
        {
          // Your code here.
        }

        void
        EUCON_Controller_exec_i::ccm_passivate ()
        {
          // Your code here.
        }

        void
        EUCON_Controller_exec_i::ccm_remove ()
        {
          // Your code here.
        }

        //==================================================================
        // Home Executor Implementation Class:   EUCON_Controller_Home_exec_i
        //==================================================================

        EUCON_Controller_Home_exec_i::EUCON_Controller_Home_exec_i (void)
        {
        }

        EUCON_Controller_Home_exec_i::~EUCON_Controller_Home_exec_i (void)
        {
        }

        // Supported or inherited operations.

        // Home operations.

        // Factory and finder operations.

        // Attribute operations.

        // Implicit operations.

        ::Components::EnterpriseComponent_ptr
        EUCON_Controller_Home_exec_i::create ()
        {
          ::Components::EnterpriseComponent_ptr retval =
            ::Components::EnterpriseComponent::_nil ();

          ACE_NEW_THROW_EX (
            retval,
            EUCON_Controller_exec_i,
            ::CORBA::NO_MEMORY ());

          return retval;
        }

        extern "C" EUCON_CONTROLLER_EXEC_Export
        ::Components::HomeExecutorBase_ptr
        create_CIAO_RACE_Controller_EUCON_Controller_Home_Impl (void)
        {
          ::Components::HomeExecutorBase_ptr retval =
            ::Components::HomeExecutorBase::_nil ();

          ACE_NEW_RETURN (
            retval,
            EUCON_Controller_Home_exec_i,
            ::Components::HomeExecutorBase::_nil ());

          return retval;
        }
      }
    }
  }
}
