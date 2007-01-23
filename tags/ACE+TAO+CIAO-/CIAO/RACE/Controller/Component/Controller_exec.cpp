// $Id$
#include "Controller_exec.h"
#include "ciao/CIAO_common.h"

namespace CIAO
{
  namespace RACE
  {
    namespace CIDL_Controller_Impl
    {
      Controller_exec_i::Controller_exec_i (void)
      {
      }

      Controller_exec_i::~Controller_exec_i (void)
      {
      }

      // Supported operations.
      void
      Controller_exec_i::start (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
        ACE_THROW_SPEC ((::CORBA::SystemException))
      {}

      void
      Controller_exec_i::stop (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
        ACE_THROW_SPEC ((::CORBA::SystemException))
      {}

      // Attribute operations.

      ::CORBA::Double
      Controller_exec_i::sampling_period (
        ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((CORBA::SystemException))
      {
        // Your code here.
        return 0.0;
      }

      // Port operations.

      // Operations from Components::SessionComponent

      void
      Controller_exec_i::set_session_context (
        ::Components::SessionContext_ptr ctx
        ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((
                        ::CORBA::SystemException,
                        ::Components::CCMException))
      {
        this->context_ =
        Controller_Context::_narrow (
          ctx
          ACE_ENV_ARG_PARAMETER);
        ACE_CHECK;

        if (this->context_ == 0)
        {
          ACE_THROW (CORBA::INTERNAL ());
        }
      }

      void
      Controller_exec_i::ciao_preactivate (
        ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((
                        ::CORBA::SystemException,
                        ::Components::CCMException))
      {
        // Your code here.
      }

      void
      Controller_exec_i::ciao_postactivate (
        ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((
                        ::CORBA::SystemException,
                        ::Components::CCMException))
      {
        // Your code here.
      }

      void
      Controller_exec_i::ccm_activate (
        ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((
                        ::CORBA::SystemException,
                        ::Components::CCMException))
      {
        // Your code here.
      }

      void
      Controller_exec_i::ccm_passivate (
        ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((
                        ::CORBA::SystemException,
                        ::Components::CCMException))
      {
        // Your code here.
      }

      void
      Controller_exec_i::ccm_remove (
        ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((
                        ::CORBA::SystemException,
                        ::Components::CCMException))
      {
        // Your code here.
      }

      //==================================================================
      // Home Executor Implementation Class:   Controller_Home_exec_i
      //==================================================================

      Controller_Home_exec_i::Controller_Home_exec_i (void)
      {
      }

      Controller_Home_exec_i::~Controller_Home_exec_i (void)
      {
      }

      // Supported or inherited operations.

      // Home operations.

      // Factory and finder operations.

      // Attribute operations.

      // Implicit operations.

      ::Components::EnterpriseComponent_ptr
      Controller_Home_exec_i::create (
        ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((
                        ::CORBA::SystemException,
                        ::Components::CCMException))
      {
        ::Components::EnterpriseComponent_ptr retval =
        ::Components::EnterpriseComponent::_nil ();

        ACE_NEW_THROW_EX (
          retval,
          Controller_exec_i,
          CORBA::NO_MEMORY ());
        ACE_CHECK_RETURN (::Components::EnterpriseComponent::_nil ());

        return retval;
      }

      extern "C" CONTROLLER_EXEC_Export ::Components::HomeExecutorBase_ptr
      create_CIAO_RACE_Controller_Home_Impl (void)
      {
        ::Components::HomeExecutorBase_ptr retval =
        ::Components::HomeExecutorBase::_nil ();

        ACE_NEW_RETURN (
          retval,
          Controller_Home_exec_i,
          ::Components::HomeExecutorBase::_nil ());

        return retval;
      }
    }
  }
}
