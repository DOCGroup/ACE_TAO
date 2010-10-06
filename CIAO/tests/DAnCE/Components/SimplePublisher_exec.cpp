// $Id$

#include "SimplePublisher_exec.h"
#include "ciao/Logger/Log_Macros.h"

namespace CIAO_Simple_SimplePublisher_Impl
{
  //==================================================================
  // Component Executor Implementation Class:   SimplePublisher_exec_i
  //==================================================================

  SimplePublisher_exec_i::SimplePublisher_exec_i (void)
  {
    CIAO_TRACE ("SimplePublisher_exec_i::SimplePublisher_exec_i (void)");
  }

  SimplePublisher_exec_i::~SimplePublisher_exec_i (void)
  {
    CIAO_TRACE ("SimplePublisher_exec_i::~SimplePublisher_exec_i (void)");
  }

  // Supported or inherited operations.

  // Attribute operations.

  // Port operations.

  // Operations from Components::SessionComponent

  void
  SimplePublisher_exec_i::set_session_context (
    ::Components::SessionContext_ptr ctx)
  {
    CIAO_TRACE ("SimplePublisher_exec_i::set_session_context ()");
    this->context_ =
      ::Simple::CCM_SimplePublisher_Context::_narrow (ctx);

    if (CORBA::is_nil (this->context_.in ()))
    {
      throw ::CORBA::INTERNAL ();
    }
  }

  void
  SimplePublisher_exec_i::configuration_complete ()
  {
    CIAO_TRACE ("SimplePublisher_exec_i::configuration_complete ()");
    // Your code here.
  }

  void
  SimplePublisher_exec_i::ccm_activate ()
  {
    CIAO_TRACE ("SimplePublisher_exec_i::ccm_activate ()");
#if !defined (CCM_NOEVENT)
    ::Simple::Hello_var hello = new OBV_Simple::Hello ("Test successful, hello from SimplePublisher_exec_i");
    this->context_->push_hello_ (hello._retn ());
#endif
    // Your code here.
  }

  void
  SimplePublisher_exec_i::ccm_passivate ()
  {
    CIAO_TRACE ("SimplePublisher_exec_i::ccm_passivate ()");
    // Your code here.
  }

  void
  SimplePublisher_exec_i::ccm_remove ()
  {
    CIAO_TRACE ("SimplePublisher_exec_i::ccm_remove ()");
    // Your code here.
  }

  //==================================================================
  // Home Executor Implementation Class:   SimplePublisherHome_exec_i
  //==================================================================

  SimplePublisherHome_exec_i::SimplePublisherHome_exec_i (void)
  {
    CIAO_TRACE ("SimplePublisherHome_exec_i::SimplePublisherHome_exec_i (void)");
  }

  SimplePublisherHome_exec_i::~SimplePublisherHome_exec_i (void)
  {
    CIAO_TRACE ("SimplePublisherHome_exec_i::~SimplePublisherHome_exec_i (void)");
  }

  // Supported or inherited operations.

  // Home operations.

  // Factory and finder operations.

  // Attribute operations.

  // Implicit operations.

  ::Components::EnterpriseComponent_ptr
  SimplePublisherHome_exec_i::create ()
  {
    CIAO_TRACE ("SimplePublisherHome_exec_i::create ()");
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_THROW_EX (
      retval,
      SimplePublisher_exec_i,
      ::CORBA::NO_MEMORY ());

    return retval;
  }

  extern "C" SIMPLEPUBLISHER_EXEC_Export ::Components::HomeExecutorBase_ptr
  create_Simple_SimplePublisherHome_Impl (void)
  {
    CIAO_TRACE ("create_Simple_SimplePublisherHome_Impl (void)");

    ::Components::HomeExecutorBase_ptr retval =
      ::Components::HomeExecutorBase::_nil ();

    ACE_NEW_RETURN (
      retval,
      SimplePublisherHome_exec_i,
      ::Components::HomeExecutorBase::_nil ());

    return retval;
  }
}

