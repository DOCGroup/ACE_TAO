// $Id$

#include "SimpleLocalUser_exec.h"
#include "DAnCE/Logger/Log_Macros.h"

namespace CIAO_Simple_SimpleLocalUser_Impl
{
  //==================================================================
  // Component Executor Implementation Class:   SimpleLocalUser_exec_i
  //==================================================================

  SimpleLocalUser_exec_i::SimpleLocalUser_exec_i (void)
  {
    DANCE_TRACE ("SimpleLocalUser_exec_i::SimpleLocalUser_exec_i (void)");
  }

  SimpleLocalUser_exec_i::~SimpleLocalUser_exec_i (void)
  {
    DANCE_TRACE ("SimpleLocalUser_exec_i::~SimpleLocalUser_exec_i (void)");
  }

  // Supported or inherited operations.

  // Attribute operations.

  // Port operations.

  // Operations from Components::SessionComponent

  void
  SimpleLocalUser_exec_i::set_session_context (
    ::Components::SessionContext_ptr ctx)
  {
    DANCE_TRACE ("SimpleLocalUser_exec_i::set_session_context ()");
    this->context_ =
      ::Simple::CCM_SimpleLocalUser_Context::_narrow (ctx);

    if (CORBA::is_nil (this->context_.in ()))
    {
      throw ::CORBA::INTERNAL ();
    }
  }

  void
  SimpleLocalUser_exec_i::configuration_complete ()
  {
    DANCE_TRACE ("SimpleLocalUser_exec_i::configuration_complete ()");
    // Your code here.
  }

  void
  SimpleLocalUser_exec_i::ccm_activate ()
  {
    DANCE_TRACE ("SimpleLocalUser_exec_i::ccm_activate ()");

    ::Simple::LocalTrigger_var trig (this->context_->get_connection_trig ());

    if (CORBA::is_nil (trig.in ()))
      {
        ACE_ERROR ((LM_ERROR, "SimpleLocalUser_exec_i::ccm_activate () - "
                     "Error: Reference nil for port trig\n"));
        return;
      }

    // Your code here.
    try
      {
        trig->hello ("Test successful; hello from SimpleLocalUser.");
      }
    catch (CORBA::Exception &ex)
      {
        ACE_ERROR ((LM_ERROR, "SimpleLocalUser_exec_i::ccm_activate () - "
                     "Caught CORBA exception, details follow:\n"));
        ex._tao_print_exception ("SimpleLocalUser_exec_i::ccm_activate () - ");
      }
    catch (...)
      {
        ACE_ERROR ((LM_ERROR, "SimpleLocalUser_exec_i::ccm_activate () - "
                     "Error: Caught unknown exception whilst invoking reference for port trig.\n"));
      }
  }

  void
  SimpleLocalUser_exec_i::ccm_passivate ()
  {
    DANCE_TRACE ("SimpleLocalUser_exec_i::ccm_passivate ()");
    // Your code here.
  }

  void
  SimpleLocalUser_exec_i::ccm_remove ()
  {
    DANCE_TRACE ("SimpleLocalUser_exec_i::ccm_remove ()");
    // Your code here.
  }

  //==================================================================
  // Home Executor Implementation Class:   SimpleLocalUserHome_exec_i
  //==================================================================

  SimpleLocalUserHome_exec_i::SimpleLocalUserHome_exec_i (void)
  {
    DANCE_TRACE ("SimpleLocalUserHome_exec_i::SimpleLocalUserHome_exec_i (void)");
  }

  SimpleLocalUserHome_exec_i::~SimpleLocalUserHome_exec_i (void)
  {
    DANCE_TRACE ("SimpleLocalUserHome_exec_i::~SimpleLocalUserHome_exec_i (void)");
  }

  // Supported or inherited operations.

  // Home operations.

  // Factory and finder operations.

  // Attribute operations.

  // Implicit operations.

  ::Components::EnterpriseComponent_ptr
  SimpleLocalUserHome_exec_i::create ()
  {
    DANCE_TRACE ("SimpleLocalUserHome_exec_i::create ()");
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_THROW_EX (
      retval,
      SimpleLocalUser_exec_i,
      ::CORBA::NO_MEMORY ());

    return retval;
  }

  extern "C" SIMPLEUSER_EXEC_Export ::Components::HomeExecutorBase_ptr
  create_Simple_SimpleLocalUserHome_Impl (void)
  {
    DANCE_TRACE ("create_Simple_SimpleLocalUserHome_Impl (void)");
    ::Components::HomeExecutorBase_ptr retval =
      ::Components::HomeExecutorBase::_nil ();

    ACE_NEW_RETURN (
      retval,
      SimpleLocalUserHome_exec_i,
      ::Components::HomeExecutorBase::_nil ());

    return retval;
  }
}

