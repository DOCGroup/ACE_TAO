#include "Comm_Component_exec.h"
#include "ciao/CIAO_common.h"

namespace CIDL_Comm_Component
{
  //==================================================================
  // Component Executor Implementation Class:   Comm_Component_exec_i
  //==================================================================

  Comm_Component_exec_i::Comm_Component_exec_i (void)
  {
  }

  Comm_Component_exec_i::~Comm_Component_exec_i (void)
  {
  }

  // Supported or inherited operations.

  // Attribute operations.

  // Port operations.

  void
  Comm_Component_exec_i::push_recv_command (
    ::SPACE::Gizmo_Data * ev
    ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC (( ::CORBA::SystemException))
  {
    ACE_DEBUG ((LM_DEBUG, "(%P|%t) Comm-Component: "
                "Got health data from %s.\n", ev->sender ()));
    //    this->context_->push_transmit_data (ev);
  }

  void
  Comm_Component_exec_i::push_science_data (
    ::SPACE::Gizmo_Data * ev
    ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC (( ::CORBA::SystemException))
  {
    ACE_DEBUG ((LM_DEBUG, "(%P|%t) Comm-Component: "
                "Got science data from %s.\n", ev->sender ()));
    //    this->context_->push_transmit_data (ev);

  }

  // Operations from Components::SessionComponent

  void
  Comm_Component_exec_i::set_session_context (
    ::Components::SessionContext_ptr ctx
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC (( ::CORBA::SystemException,
                   ::Components::CCMException))
  {
    this->context_ =
      Comm_Component_Context::_narrow (
        ctx
        ACE_ENV_ARG_PARAMETER);
    ACE_CHECK;

    if (this->context_ == 0)
    {
      ACE_THROW ( ::CORBA::INTERNAL ());
    }
  }

  void
  Comm_Component_exec_i::ciao_preactivate (
    ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC (( ::CORBA::SystemException,
                   ::Components::CCMException))
  {
    // Your code here.
  }

  void
  Comm_Component_exec_i::configuration_complete (
    ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC (( ::CORBA::SystemException,
                   ::Components::CCMException))
  {
    // Your code here.
  }

  void
  Comm_Component_exec_i::ccm_activate (
    ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC (( ::CORBA::SystemException,
                   ::Components::CCMException))
  {
    // Your code here.
  }

  void
  Comm_Component_exec_i::ccm_passivate (
    ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC (( ::CORBA::SystemException,
                   ::Components::CCMException))
  {
    // Your code here.
  }

  void
  Comm_Component_exec_i::ccm_remove (
    ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC (( ::CORBA::SystemException,
                   ::Components::CCMException))
  {
    // Your code here.
  }

  //==================================================================
  // Home Executor Implementation Class:   Comm_Component_Factory_exec_i
  //==================================================================

  Comm_Component_Factory_exec_i::Comm_Component_Factory_exec_i (void)
  {
  }

  Comm_Component_Factory_exec_i::~Comm_Component_Factory_exec_i (void)
  {
  }

  // Supported or inherited operations.

  // Home operations.

  // Factory and finder operations.

  // Attribute operations.

  // Implicit operations.

  ::Components::EnterpriseComponent_ptr
  Comm_Component_Factory_exec_i::create (
    ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC (( ::CORBA::SystemException,
                   ::Components::CCMException))
  {
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_THROW_EX (
      retval,
      Comm_Component_exec_i,
      ::CORBA::NO_MEMORY ());
    ACE_CHECK_RETURN (::Components::EnterpriseComponent::_nil ());

    return retval;
  }

  extern "C" COMM_COMPONENT_EXEC_Export ::Components::HomeExecutorBase_ptr
  create_SPACE_Comm_Component_Factory_Impl (void)
  {
    ::Components::HomeExecutorBase_ptr retval =
      ::Components::HomeExecutorBase::_nil ();

    ACE_NEW_RETURN (
      retval,
      Comm_Component_Factory_exec_i,
      ::Components::HomeExecutorBase::_nil ());

    return retval;
  }
}
