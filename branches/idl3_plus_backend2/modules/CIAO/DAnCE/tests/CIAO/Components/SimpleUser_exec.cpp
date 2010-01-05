// $Id$

#include "SimpleUser_exec.h"
#include "DAnCE/Logger/Log_Macros.h"

namespace CIAO_Simple_SimpleUser_Impl
{
  //==================================================================
  // Component Executor Implementation Class:   SimpleUser_exec_i
  //==================================================================

  SimpleUser_exec_i::SimpleUser_exec_i (void)
  {
    DANCE_TRACE ("SimpleUser_exec_i::SimpleUser_exec_i (void)");
  }

  SimpleUser_exec_i::~SimpleUser_exec_i (void)
  {
    DANCE_TRACE ("SimpleUser_exec_i::~SimpleUser_exec_i (void)");
  }

  // Supported or inherited operations.

  // Attribute operations.

  // Port operations.

  // Operations from Components::SessionComponent

  void
  SimpleUser_exec_i::set_session_context (
    ::Components::SessionContext_ptr ctx)
  {
    DANCE_TRACE ("SimpleUser_exec_i::set_session_context ()");
    this->context_ =
      ::Simple::CCM_SimpleUser_Context::_narrow (ctx);

    if (CORBA::is_nil (this->context_.in ()))
    {
      throw ::CORBA::INTERNAL ();
    }
  }

  void
  SimpleUser_exec_i::configuration_complete ()
  {
    DANCE_TRACE ("SimpleUser_exec_i::configuration_complete ()");
    // Your code here.
  }

  void
  SimpleUser_exec_i::ccm_activate ()
  {
    DANCE_TRACE ("SimpleUser_exec_i::ccm_activate ()");

    ::Simple::Trigger_var trig (this->context_->get_connection_trig ());

    if (CORBA::is_nil (trig.in ()))
      {
        ACE_ERROR ((LM_ERROR, "SimpleUser_exec_i::ccm_activate () - "
                     "Error: Reference nil for port trig\n"));
        return;
      }

    // Your code here.
    try
      {
        trig->hello ("Test successful; hello from SimpleUser.");
      }
    catch (CORBA::Exception &ex)
      {
        ACE_ERROR ((LM_ERROR, "SimpleUser_exec_i::ccm_activate () - "
                     "Caught CORBA exception, details follow:\n"));
        ex._tao_print_exception ("SimpleUser_exec_i::ccm_activate () - ");
      }
    catch (...)
      {
        ACE_ERROR ((LM_ERROR, "SimpleUser_exec_i::ccm_activate () - "
                     "Error: Caught unknown exception whilst invoking reference for port trig.\n"));
      }
  }

  void
  SimpleUser_exec_i::ccm_passivate ()
  {
    DANCE_TRACE ("SimpleUser_exec_i::ccm_passivate ()");
    // Your code here.
  }

  void
  SimpleUser_exec_i::ccm_remove ()
  {
    DANCE_TRACE ("SimpleUser_exec_i::ccm_remove ()");
    // Your code here.
  }

  //==================================================================
  // Home Executor Implementation Class:   SimpleUserHome_exec_i
  //==================================================================

  SimpleUserHome_exec_i::SimpleUserHome_exec_i (void)
  {
    DANCE_TRACE ("SimpleUserHome_exec_i::SimpleUserHome_exec_i (void)");
  }

  SimpleUserHome_exec_i::~SimpleUserHome_exec_i (void)
  {
    DANCE_TRACE ("SimpleUserHome_exec_i::~SimpleUserHome_exec_i (void)");
  }

  // Supported or inherited operations.

  // Home operations.

  // Factory and finder operations.

  // Attribute operations.

  // Implicit operations.

  ::Components::EnterpriseComponent_ptr
  SimpleUserHome_exec_i::create ()
  {
    DANCE_TRACE ("SimpleUserHome_exec_i::create ()");
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_THROW_EX (
      retval,
      SimpleUser_exec_i,
      ::CORBA::NO_MEMORY ());

    return retval;
  }

  extern "C" SIMPLEUSER_EXEC_Export ::Components::HomeExecutorBase_ptr
  create_Simple_SimpleUserHome_Impl (void)
  {
    DANCE_TRACE ("create_Simple_SimpleUserHome_Impl (void)");
    ::Components::HomeExecutorBase_ptr retval =
      ::Components::HomeExecutorBase::_nil ();

    ACE_NEW_RETURN (
      retval,
      SimpleUserHome_exec_i,
      ::Components::HomeExecutorBase::_nil ());

    return retval;
  }
}

