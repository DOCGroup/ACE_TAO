// $Id$

#include "SimpleLocalProvider_exec.h"
#include "ciao/Logger/Log_Macros.h"

namespace CIAO_Simple_SimpleLocalProvider_Impl
{
  //==================================================================
  // Facet Executor Implementation Class:   Trigger_exec_i
  //==================================================================

  Trigger_exec_i::Trigger_exec_i (bool &triggered)
    : triggered_ (triggered)
  {
    CIAO_TRACE ("Trigger_exec_i::Trigger_exec_i (void)");
  }

  Trigger_exec_i::~Trigger_exec_i (void)
  {
    CIAO_TRACE ("Trigger_exec_i::~Trigger_exec_i (void)");
  }

  // Operations from ::Simple::Trigger

  void
  Trigger_exec_i::hello (
    const char * hello )
  {
    CIAO_TRACE ("Trigger_exec_i::hello ()");
    this->triggered_ = true;
    // Your code here.
    ACE_DEBUG ((LM_EMERGENCY, "Trigger_exec_i::hello - "
                 "Got the following information from trig port: %C\n",
                 hello));
  }

  //==================================================================
  // Component Executor Implementation Class:   SimpleLocalProvider_exec_i
  //==================================================================

  SimpleLocalProvider_exec_i::SimpleLocalProvider_exec_i (void)
    : triggered_ (false)
  {
    CIAO_TRACE ("SimpleLocalProvider_exec_i::SimpleLocalProvider_exec_i (void)");
  }

  SimpleLocalProvider_exec_i::~SimpleLocalProvider_exec_i (void)
  {
    CIAO_TRACE ("SimpleLocalProvider_exec_i::~SimpleLocalProvider_exec_i (void)");
  }

  // Supported or inherited operations.

  // Attribute operations.

  // Port operations.

  ::Simple::CCM_LocalTrigger_ptr
  SimpleLocalProvider_exec_i::get_trig ()
  {
    CIAO_TRACE ("SimpleLocalProvider_exec_i::get_trig ()");
    return new Trigger_exec_i (this->triggered_);
  }

  // Operations from Components::SessionComponent

  void
  SimpleLocalProvider_exec_i::set_session_context (
    ::Components::SessionContext_ptr ctx)
  {
    CIAO_TRACE ("SimpleLocalProvider_exec_i::set_session_context ()");
    this->context_ =
      ::Simple::CCM_SimpleLocalProvider_Context::_narrow (ctx);

    if (CORBA::is_nil (this->context_.in ()))
    {
      throw ::CORBA::INTERNAL ();
    }
  }

  void
  SimpleLocalProvider_exec_i::configuration_complete ()
  {
    CIAO_TRACE ("SimpleLocalProvider_exec_i::configuration_complete ()");
    // Your code here.
  }

  void
  SimpleLocalProvider_exec_i::ccm_activate ()
  {
    CIAO_TRACE ("SimpleLocalProvider_exec_i::ccm_activate ()");
    // Your code here.
  }

  void
  SimpleLocalProvider_exec_i::ccm_passivate ()
  {
    CIAO_TRACE ("SimpleLocalProvider_exec_i::ccm_passivate ()");
    // Your code here.
  }

  void
  SimpleLocalProvider_exec_i::ccm_remove ()
  {
    CIAO_TRACE ("SimpleLocalProvider_exec_i::ccm_remove ()");
    if (!triggered_)
      ACE_ERROR ((LM_EMERGENCY, "Error:  My facet wasn't triggered!!\n"));

    // Your code here.
  }

  //==================================================================
  // Home Executor Implementation Class:   SimpleLocalProviderHome_exec_i
  //==================================================================

  SimpleLocalProviderHome_exec_i::SimpleLocalProviderHome_exec_i (void)
  {
    CIAO_TRACE ("SimpleLocalProviderHome_exec_i::SimpleLocalProviderHome_exec_i (void)");
  }

  SimpleLocalProviderHome_exec_i::~SimpleLocalProviderHome_exec_i (void)
  {
    CIAO_TRACE ("SimpleLocalProviderHome_exec_i::~SimpleLocalProviderHome_exec_i (void)");
  }

  // Supported or inherited operations.

  // Home operations.

  // Factory and finder operations.

  // Attribute operations.

  // Implicit operations.

  ::Components::EnterpriseComponent_ptr
  SimpleLocalProviderHome_exec_i::create ()
  {
    CIAO_TRACE ("SimpleLocalProviderHome_exec_i::create ()");
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_THROW_EX (
      retval,
      SimpleLocalProvider_exec_i,
      ::CORBA::NO_MEMORY ());

    return retval;
  }

  extern "C" SIMPLEPROVIDER_EXEC_Export ::Components::HomeExecutorBase_ptr
  create_Simple_SimpleLocalProviderHome_Impl (void)
  {
    CIAO_TRACE ("create_Simple_SimpleLocalProviderHome_Impl (void)");
    ::Components::HomeExecutorBase_ptr retval =
      ::Components::HomeExecutorBase::_nil ();

    ACE_NEW_RETURN (
      retval,
      SimpleLocalProviderHome_exec_i,
      ::Components::HomeExecutorBase::_nil ());

    return retval;
  }
}

