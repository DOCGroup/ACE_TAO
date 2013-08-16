// -*- C++ -*-
// $Id$

#include "Foo_exec.h"

namespace CIAO_Foo_Impl
{
  //============================================================
  // Component Executor Implementation Class: Foo_exec_i
  //============================================================

  Foo_exec_i::Foo_exec_i (void)
    : supported_short_ (false)
  {
  }

  Foo_exec_i::~Foo_exec_i (void)
  {
  }

  // Supported operations and attributes.

  ::CORBA::Short
  Foo_exec_i::supported_short (void)
  {
    /* Your code here. */
    return 0;
  }

  void
  Foo_exec_i::supported_short (
    const ::CORBA::Short supported_short)
  {
    if(supported_short != 11)
      ACE_ERROR ((LM_ERROR, "ERROR: supported_short != 11, it is %d\n", supported_short));
    else supported_short_ = true;

  }

  // Operations from Components::SessionComponent.

  void
  Foo_exec_i::set_session_context (
    ::Components::SessionContext_ptr ctx)
  {
    this->context_ =
      ::CCM_Foo_Context::_narrow (ctx);

    if ( ::CORBA::is_nil (this->context_.in ()))
      {
        throw ::CORBA::INTERNAL ();
      }
  }

  void
  Foo_exec_i::configuration_complete (void)
  {
  }

  void
  Foo_exec_i::ccm_activate (void)
  {
    if (! supported_short_ )
      {
        ACE_ERROR ((LM_ERROR, "ERROR: Not all expected attributes were initialized\n"));
      }
    else
      {
        ACE_DEBUG ((LM_DEBUG, "OK: All attributes were correctly initialized\n"));
      }
  }

  void
  Foo_exec_i::ccm_passivate (void)
  {
  }

  void
  Foo_exec_i::ccm_remove (void)
  {
  }

  extern "C" FOO_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_Foo_Impl (void)
  {
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_NORETURN (
      retval,
      Foo_exec_i);

    return retval;
  }
}
