// $Id$

#include "SimpleComponent_exec.h"
#include "ciao/Logger/Log_Macros.h"

namespace CIAO_Foo_SimpleComponent_Impl
{
  //==================================================================
  // Component Executor Implementation Class:   SimpleComponent_exec_i
  //==================================================================

  SimpleComponent_exec_i::SimpleComponent_exec_i (const char *type)
    : type_ (CORBA::string_dup (type))
  {
  }

  SimpleComponent_exec_i::~SimpleComponent_exec_i (void)
  {
  }

  // Supported or inherited operations.

  void
  SimpleComponent_exec_i::trigger ()
  {
    ACE_DEBUG ((LM_EMERGENCY, "Hello, world!!!!!!!!\n"));
  }

  // Attribute operations.

  char *
  SimpleComponent_exec_i::creationtype_ ()
  {
    return CORBA::string_dup (this->type_.in ());
  }

  // Port operations.

  // Operations from Components::SessionComponent

  void
  SimpleComponent_exec_i::set_session_context (
    ::Components::SessionContext_ptr ctx)
  {
    this->context_ =
      ::Foo::CCM_SimpleComponent_Context::_narrow (ctx);

    if (CORBA::is_nil (this->context_.in ()))
    {
      throw ::CORBA::INTERNAL ();
    }
  }

  void
  SimpleComponent_exec_i::configuration_complete ()
  {
    // Your code here.
  }

  void
  SimpleComponent_exec_i::ccm_activate ()
  {
    // Your code here.
  }

  void
  SimpleComponent_exec_i::ccm_passivate ()
  {
    // Your code here.
  }

  void
  SimpleComponent_exec_i::ccm_remove ()
  {
    // Your code here.
  }

  //==================================================================
  // Home Executor Implementation Class:   SimpleHome_exec_i
  //==================================================================

  SimpleHome_exec_i::SimpleHome_exec_i (void)
  {
    CIAO_TRACE ("SimpleHome_exec_i::SimpleHome_exec_i");
    ACE_DEBUG ((LM_NOTICE, CLINFO "SimpleHome_exec_i::SimpleHome_exec_i - "
                 "Home constructed\n"));
  }

  SimpleHome_exec_i::~SimpleHome_exec_i (void)
  {
  }

  // Supported or inherited operations.

  // Home operations.

  // Factory and finder operations.

  // Attribute operations.

  // Implicit operations.

  ::Components::EnterpriseComponent_ptr
  SimpleHome_exec_i::create ()
  {
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_THROW_EX (
      retval,
      SimpleComponent_exec_i ("HomeCreated"),
      ::CORBA::NO_MEMORY ());

    return retval;
  }

  extern "C" SIMPLECOMPONENT_EXEC_Export ::Components::HomeExecutorBase_ptr
  create_Foo_SimpleHome_Impl (void)
  {
    ::Components::HomeExecutorBase_ptr retval =
      ::Components::HomeExecutorBase::_nil ();

    ACE_NEW_RETURN (
      retval,
      SimpleHome_exec_i,
      ::Components::HomeExecutorBase::_nil ());

    return retval;
  }
}

