// $Id$

#include "SimpleFailure_exec.h"
#include "ace/Log_Msg.h"

namespace CIAO_Simple_SimpleFailure_Impl
{
  //==================================================================
  // Component Executor Implementation Class:   SimpleFailure_exec_i
  //==================================================================

  SimpleFailure_exec_i::SimpleFailure_exec_i (void)
    : type_ (::Simple::NO_FAILURE)
  {
  }

  SimpleFailure_exec_i::~SimpleFailure_exec_i (void)
  {
  }

  // Supported or inherited operations.

  // Attribute operations.

  ::Simple::FailureType
  SimpleFailure_exec_i::type_attr (void)
  {
    return this->type_;
  }

  void
  SimpleFailure_exec_i::type_attr (
    ::Simple::FailureType type)
  {
    this->type_ = type;
  }

  // Port operations.

  // Operations from Components::SessionComponent

  void
  SimpleFailure_exec_i::set_session_context (
    ::Components::SessionContext_ptr ctx)
  {
    this->context_ =
      ::Simple::CCM_SimpleFailure_Context::_narrow (ctx);

    if (CORBA::is_nil (this->context_.in ()))
    {
      throw ::CORBA::INTERNAL ();
    }
  }

  void
  SimpleFailure_exec_i::configuration_complete ()
  {
    // Your code here.
    if (this->type_ == ::Simple::AT_STARTUP)
    {
        ACE_ERROR ((LM_ERROR, "SimpleFailure_exec_i::configuration_complete - "
                     "Configured failure!\n"));

        throw ::CORBA::INTERNAL ();
    }
    ACE_DEBUG ((LM_INFO, "SimpleFailure_exec_i::configuration_complete - "
                  "Successfully configured!\n"));
  }

  void
  SimpleFailure_exec_i::ccm_activate ()
  {
    // Your code here.
    if (this->type_ == ::Simple::AT_ACTIVATE)
    {
        ACE_ERROR ((LM_ERROR, "SimpleFailure_exec_i::ccm_activate - "
                     "Configured failure!\n"));

        throw ::CORBA::INTERNAL ();
    }
    ACE_DEBUG ((LM_INFO, "SimpleFailure_exec_i::ccm_activate - "
                  "Successfully activated!\n"));
  }

  void
  SimpleFailure_exec_i::ccm_passivate ()
  {
    // Your code here.
    if (this->type_ == ::Simple::AT_PASSIVATE)
    {
        ACE_ERROR ((LM_ERROR, "SimpleFailure_exec_i::ccm_passivate - "
                     "Configured failure!\n"));

        throw ::CORBA::INTERNAL ();
    }
    ACE_DEBUG ((LM_INFO, "SimpleFailure_exec_i::ccm_passivate - "
                  "Successfully deactivated!\n"));
  }

  void
  SimpleFailure_exec_i::ccm_remove ()
  {
    // Your code here.
    if (this->type_ == ::Simple::AT_REMOVE)
    {
        ACE_ERROR ((LM_ERROR, "SimpleFailure_exec_i::ccm_remove - "
                     "Configured failure!\n"));

        throw ::CORBA::INTERNAL ();
    }
    ACE_DEBUG ((LM_INFO, "SimpleFailure_exec_i::ccm_remove - "
                  "Successfully removed!\n"));
  }

  //==================================================================
  // Home Executor Implementation Class:   SimpleFailureHome_exec_i
  //==================================================================

  SimpleFailureHome_exec_i::SimpleFailureHome_exec_i (void)
  {
  }

  SimpleFailureHome_exec_i::~SimpleFailureHome_exec_i (void)
  {
  }

  // Supported or inherited operations.

  // Home operations.

  // Factory and finder operations.

  // Attribute operations.

  // Implicit operations.

  ::Components::EnterpriseComponent_ptr
  SimpleFailureHome_exec_i::create ()
  {
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_THROW_EX (
      retval,
      SimpleFailure_exec_i,
      ::CORBA::NO_MEMORY ());

    return retval;
  }

  extern "C" SIMPLEATTRIBUTE_EXEC_Export ::Components::HomeExecutorBase_ptr
  create_Simple_SimpleFailureHome_Impl (void)
  {
    ::Components::HomeExecutorBase_ptr retval =
      ::Components::HomeExecutorBase::_nil ();

    ACE_NEW_RETURN (
      retval,
      SimpleFailureHome_exec_i,
      ::Components::HomeExecutorBase::_nil ());

    return retval;
  }
}

