// $Id$
#include "EUCON_Effector_exec.h"
#include "ciao/CIAO_common.h"
#include "ApplicationActuator_exec.h"
#include "SystemActuator_exec.h"
#include <sstream>

namespace CIAO
{
  namespace RACE
  {
    namespace Effector
    {
      namespace CIDL_EUCON_Effector_Impl
      {
        EUCON_Effector_exec_i::EUCON_Effector_exec_i (void)
          : logger_ ("EUCON_Effector.log")
        {
        }

        EUCON_Effector_exec_i::~EUCON_Effector_exec_i (void)
        {
        }

        // Supported or inherited operations.

        // Attribute operations.

        // Port operations.

        ::CIAO::RACE::Effector::CCM_ApplicationActuator_ptr
        EUCON_Effector_exec_i::get_appActuator ()
        {
          std::stringstream msg ;
          msg << "Entering EUCON_Effector_exec_i::get_appActuator ().\n";
          msg << "Exiting EUCON_Effector_exec_i::get_appActuator ().\n"; 
          this->logger_.log (msg.str());
          return new ApplicationActuator_exec_i (this->context_, this->logger_);
        }

        ::CIAO::RACE::Effector::CCM_SystemActuator_ptr
        EUCON_Effector_exec_i::get_sysActuator ()
        {
          // Your code here.
          return new SystemActuator_exec_i ();
        }

        // Operations from Components::SessionComponent

        void
        EUCON_Effector_exec_i::set_session_context (
          ::Components::SessionContext_ptr ctx)
        {
          this->context_ =
            CIAO_CIAO_RACE_Effector_EUCON_Effector_Impl::EUCON_Effector_Context::_narrow (ctx);

          if (CORBA::is_nil (this->context_))
          {
            throw ::CORBA::INTERNAL ();
          }
        }

        void
        EUCON_Effector_exec_i::ciao_preactivate ()
        {
          // Your code here.
        }

        void
        EUCON_Effector_exec_i::configuration_complete ()
        {
          // Your code here.
        }

        void
        EUCON_Effector_exec_i::ccm_activate ()
        {
          // Your code here.
        }

        void
        EUCON_Effector_exec_i::ccm_passivate ()
        {
          // Your code here.
        }

        void
        EUCON_Effector_exec_i::ccm_remove ()
        {
          // Your code here.
        }

        //==================================================================
        // Home Executor Implementation Class:   EUCON_Effector_Home_exec_i
        //==================================================================

        EUCON_Effector_Home_exec_i::EUCON_Effector_Home_exec_i (void)
        {
        }

        EUCON_Effector_Home_exec_i::~EUCON_Effector_Home_exec_i (void)
        {
        }

        // Supported or inherited operations.

        // Home operations.

        // Factory and finder operations.

        // Attribute operations.

        // Implicit operations.

        ::Components::EnterpriseComponent_ptr
        EUCON_Effector_Home_exec_i::create ()
        {
          ::Components::EnterpriseComponent_ptr retval =
            ::Components::EnterpriseComponent::_nil ();

          ACE_NEW_THROW_EX (
            retval,
            EUCON_Effector_exec_i,
            ::CORBA::NO_MEMORY ());

          return retval;
        }

        extern "C" EUCON_EFFECTOR_EXEC_Export ::Components::HomeExecutorBase_ptr
        create_CIAO_RACE_Effector_EUCON_Effector_Home_Impl (void)
        {
          ::Components::HomeExecutorBase_ptr retval =
            ::Components::HomeExecutorBase::_nil ();

          ACE_NEW_RETURN (
            retval,
            EUCON_Effector_Home_exec_i,
            ::Components::HomeExecutorBase::_nil ());

          return retval;
        }
      }
    }
  }
}
