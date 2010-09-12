// $Id$

#include "SimpleConsumer_exec.h"
#include "ciao/Logger/Log_Macros.h"

namespace CIAO_Simple_SimpleConsumer_Impl
{
  //==================================================================
  // Component Executor Implementation Class:   SimpleConsumer_exec_i
  //==================================================================

  SimpleConsumer_exec_i::SimpleConsumer_exec_i (void) :
    pushed_ (false)
  {
    CIAO_TRACE ("SimpleConsumer_exec_i::SimpleConsumer_exec_i (void)");
  }

  SimpleConsumer_exec_i::~SimpleConsumer_exec_i (void)
  {
    CIAO_TRACE ("SimpleConsumer_exec_i::~SimpleConsumer_exec_i (void)");
  }

  // Supported or inherited operations.

  // Attribute operations.

  // Port operations.

  void
  SimpleConsumer_exec_i::push_hello_ (
    ::Simple::Hello * ev )
  {
    CIAO_TRACE ("SimpleConsumer_exec_i::push_hello_ ()");
    // Your code here.
    
    this->pushed_ = true;
    ACE_DEBUG ((LM_EMERGENCY, "SimpleConsumer_exec_i::push_hello_ - "
                 "Received hello event, value is %s\n", ev->hello_ ()));
  }

  // Operations from Components::SessionComponent

  void
  SimpleConsumer_exec_i::set_session_context (
    ::Components::SessionContext_ptr ctx)
  {
    CIAO_TRACE ("SimpleConsumer_exec_i::set_session_context ()");

    this->context_ =
      ::Simple::CCM_SimpleConsumer_Context::_narrow (ctx);

    if (CORBA::is_nil (this->context_.in ()))
    {
      throw ::CORBA::INTERNAL ();
    }
  }

  void
  SimpleConsumer_exec_i::configuration_complete ()
  {
    CIAO_TRACE ("SimpleConsumer_exec_i::configuration_complete ()");
    // Your code here.
  }

  void
  SimpleConsumer_exec_i::ccm_activate ()
  {
    CIAO_TRACE ("SimpleConsumer_exec_i::ccm_activate ()");
    // Your code here.
  }

  void
  SimpleConsumer_exec_i::ccm_passivate ()
  {
    CIAO_TRACE ("SimpleConsumer_exec_i::ccm_passivate ()");
    // Your code here.
  }

  void
  SimpleConsumer_exec_i::ccm_remove ()
  {
    CIAO_TRACE ("SimpleConsumer_exec_i::ccm_remove ()");
    if (!pushed_)
      {
        ACE_ERROR ((LM_EMERGENCY, "Error: SimpleConsumer didn't get pushed!\n"));
      }
    // Your code here.
  }

  //==================================================================
  // Home Executor Implementation Class:   SimpleConsumerHome_exec_i
  //==================================================================

  SimpleConsumerHome_exec_i::SimpleConsumerHome_exec_i (void)
  {
    CIAO_TRACE ("SimpleConsumerHome_exec_i::SimpleConsumerHome_exec_i (void)");
  }

  SimpleConsumerHome_exec_i::~SimpleConsumerHome_exec_i (void)
  {
    CIAO_TRACE ("SimpleConsumerHome_exec_i::~SimpleConsumerHome_exec_i (void)");
  }

  // Supported or inherited operations.

  // Home operations.

  // Factory and finder operations.

  // Attribute operations.

  // Implicit operations.

  ::Components::EnterpriseComponent_ptr
  SimpleConsumerHome_exec_i::create ()
  {
    CIAO_TRACE ("SimpleConsumerHome_exec_i::create ()");
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_THROW_EX (
      retval,
      SimpleConsumer_exec_i,
      ::CORBA::NO_MEMORY ());

    return retval;
  }

  extern "C" SIMPLECONSUMER_EXEC_Export ::Components::HomeExecutorBase_ptr
  create_Simple_SimpleConsumerHome_Impl (void)
  {
    CIAO_TRACE ("create_Simple_SimpleConsumerHome_Impl (void)");
    ::Components::HomeExecutorBase_ptr retval =
      ::Components::HomeExecutorBase::_nil ();

    ACE_NEW_RETURN (
      retval,
      SimpleConsumerHome_exec_i,
      ::Components::HomeExecutorBase::_nil ());

    return retval;
  }
}

