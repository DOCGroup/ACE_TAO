// $Id$

#include "SimpleProvider_exec.h"
#include "DAnCE/Logger/Log_Macros.h"

namespace CIAO_Simple_SimpleProvider_Impl
{
  //==================================================================
  // Facet Executor Implementation Class:   Trigger_exec_i
  //==================================================================

  Trigger_exec_i::Trigger_exec_i (bool &triggered)
    : triggered_ (triggered)
  {
    DANCE_TRACE ("Trigger_exec_i::Trigger_exec_i (void)");
  }

  Trigger_exec_i::~Trigger_exec_i (void)
  {
    DANCE_TRACE ("Trigger_exec_i::~Trigger_exec_i (void)");
  }

  // Operations from ::Simple::Trigger

  void
  Trigger_exec_i::hello (
    const char * hello )
  {
    DANCE_TRACE ("Trigger_exec_i::hello ()");
    this->triggered_ = true;
    // Your code here.
    ACE_DEBUG ((LM_EMERGENCY, "Trigger_exec_i::hello - "
                 "Got the following information from trig port: %C\n",
                 hello));
  }

  //==================================================================
  // Component Executor Implementation Class:   SimpleProvider_exec_i
  //==================================================================

  SimpleProvider_exec_i::SimpleProvider_exec_i (void)
    : triggered_ (false)
  {
    DANCE_TRACE ("SimpleProvider_exec_i::SimpleProvider_exec_i (void)");
  }

  SimpleProvider_exec_i::~SimpleProvider_exec_i (void)
  {
    DANCE_TRACE ("SimpleProvider_exec_i::~SimpleProvider_exec_i (void)");
  }

  // Supported or inherited operations.

  // Attribute operations.

  // Port operations.

  ::Simple::CCM_Trigger_ptr
  SimpleProvider_exec_i::get_trig ()
  {
    DANCE_TRACE ("SimpleProvider_exec_i::get_trig ()");
    return new Trigger_exec_i (this->triggered_);
  }

  // Operations from Components::SessionComponent

  void
  SimpleProvider_exec_i::set_session_context (
    ::Components::SessionContext_ptr ctx)
  {
    DANCE_TRACE ("SimpleProvider_exec_i::set_session_context ()");
    this->context_ =
      ::Simple::CCM_SimpleProvider_Context::_narrow (ctx);

    if (CORBA::is_nil (this->context_.in ()))
    {
      throw ::CORBA::INTERNAL ();
    }
  }

  void
  SimpleProvider_exec_i::configuration_complete ()
  {
    DANCE_TRACE ("SimpleProvider_exec_i::configuration_complete ()");
    // Your code here.
  }

  void
  SimpleProvider_exec_i::ccm_activate ()
  {
    DANCE_TRACE ("SimpleProvider_exec_i::ccm_activate ()");
    // Your code here.
  }

  void
  SimpleProvider_exec_i::ccm_passivate ()
  {
    DANCE_TRACE ("SimpleProvider_exec_i::ccm_passivate ()");
    // Your code here.
  }

  void
  SimpleProvider_exec_i::ccm_remove ()
  {
    DANCE_TRACE ("SimpleProvider_exec_i::ccm_remove ()");
    if (!triggered_)
      ACE_ERROR ((LM_EMERGENCY, "Error:  My facet wasn't triggered!!\n"));
    
    // Your code here.
  }

  //==================================================================
  // Home Executor Implementation Class:   SimpleProviderHome_exec_i
  //==================================================================

  SimpleProviderHome_exec_i::SimpleProviderHome_exec_i (void)
  {
    DANCE_TRACE ("SimpleProviderHome_exec_i::SimpleProviderHome_exec_i (void)");
  }

  SimpleProviderHome_exec_i::~SimpleProviderHome_exec_i (void)
  {
    DANCE_TRACE ("SimpleProviderHome_exec_i::~SimpleProviderHome_exec_i (void)");
  }

  // Supported or inherited operations.

  // Home operations.

  // Factory and finder operations.

  // Attribute operations.

  // Implicit operations.

  ::Components::EnterpriseComponent_ptr
  SimpleProviderHome_exec_i::create ()
  {
    DANCE_TRACE ("SimpleProviderHome_exec_i::create ()");
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_THROW_EX (
      retval,
      SimpleProvider_exec_i,
      ::CORBA::NO_MEMORY ());

    return retval;
  }

  extern "C" SIMPLEPROVIDER_EXEC_Export ::Components::HomeExecutorBase_ptr
  create_Simple_SimpleProviderHome_Impl (void)
  {
    DANCE_TRACE ("create_Simple_SimpleProviderHome_Impl (void)");
    ::Components::HomeExecutorBase_ptr retval =
      ::Components::HomeExecutorBase::_nil ();

    ACE_NEW_RETURN (
      retval,
      SimpleProviderHome_exec_i,
      ::Components::HomeExecutorBase::_nil ());

    return retval;
  }
}

