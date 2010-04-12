// -*- C++ -*-
// $Id$

#include "Local_Facet_exec.h"
#include "ciao/Logger/Log_Macros.h"

namespace CIAO_Bar_Impl
{
  //============================================================
  // Facet Executor Implementation Class: Foo_exec_i
  //============================================================

  Foo_exec_i::Foo_exec_i (void)
  {
  }

  Foo_exec_i::~Foo_exec_i (void)
  {
  }

  void Foo_exec_i::simple (void)
  {
    ACE_DEBUG ((LM_INFO, "Got simple invocation\n"));
  }

  // Operations from ::Foo

  //============================================================
  // Component Executor Implementation Class: Bar_exec_i
  //============================================================

  Bar_exec_i::Bar_exec_i (void)
  {
  }

  Bar_exec_i::~Bar_exec_i (void)
  {
  }

  // Supported operations and attributes.

  // Component attributes and port operations.

  ::CCM_Foo_ptr
  Bar_exec_i::get_foo_out (void)
  {
    return new Foo_exec_i ();
  }

  // Operations from Components::SessionComponent.

  void
  Bar_exec_i::set_session_context (
    ::Components::SessionContext_ptr ctx)
  {
    this->context_ =
      ::CCM_Bar_Context::_narrow (ctx);

    if ( ::CORBA::is_nil (this->context_.in ()))
      {
        throw ::CORBA::INTERNAL ();
      }
  }

  void
  Bar_exec_i::run_test (void)
  {
    ::Foo_var foo_intf = this->context_->get_connection_foo_in ();

    if (CORBA::is_nil (foo_intf))
      {
        ACE_ERROR ((LM_ERROR, "ERROR: Local_Facet_Test: got a nil object reference for my connection\n"));
        return;
      }

    ACE_DEBUG ((LM_DEBUG, "Invoking simple\n"));
    foo_intf->simple ();

    ACE_DEBUG ((LM_INFO, "Test successful!\n"));
  }

  void
  Bar_exec_i::configuration_complete (void)
  {
    /* Your code here. */
  }

  void
  Bar_exec_i::ccm_activate (void)
  {
    /* Your code here. */
  }

  void
  Bar_exec_i::ccm_passivate (void)
  {
    /* Your code here. */
  }

  void
  Bar_exec_i::ccm_remove (void)
  {
    /* Your code here. */
  }

  extern "C" LOCAL_FACET_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_Bar_Impl (void)
  {
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_NORETURN (
      retval,
      Bar_exec_i);

    return retval;
  }
}

