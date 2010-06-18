// -*- C++ -*-
// $Id$

#include "Navigation_exec.h"
#include "ciao/Logger/Log_Macros.h"

namespace CIAO_Navigation_Impl
{
  //============================================================
  // Component Executor Implementation Class: Navigation_exec_i
  //============================================================
  Navigation_exec_i::Navigation_exec_i (void)
  {
  }

  Navigation_exec_i::~Navigation_exec_i (void)
  {
  }

  // Supported operations and attributes.

  // Component attributes and port operations.
  ::CCM_foo_ptr
  Navigation_exec_i::get_navigation_foo (void)
  {
    return ::CCM_foo::_nil ();
  }

  ::CCM_foo_inherited_ptr
  Navigation_exec_i::get_inherited_foo (void)
  {
    return ::CCM_foo_inherited::_nil ();
  }

  // Operations from Components::SessionComponent.

  void
  Navigation_exec_i::set_session_context (
    ::Components::SessionContext_ptr ctx)
  {
    this->context_ =
      ::CCM_Navigation_Context::_narrow (ctx);

    if ( ::CORBA::is_nil (this->context_.in ()))
      {
        throw ::CORBA::INTERNAL ();
      }
  }

  void
  Navigation_exec_i::configuration_complete (void)
  {
    /* Your code here. */
  }

  void
  Navigation_exec_i::ccm_activate (void)
  {
    /* Your code here. */
  }

  void
  Navigation_exec_i::ccm_passivate (void)
  {
    /* Your code here. */
  }

  void
  Navigation_exec_i::ccm_remove (void)
  {
    /* Your code here. */
  }

  extern "C" NAVIGATION_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_Navigation_Impl (void)
  {
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_NORETURN (
      retval,
      Navigation_exec_i);

    return retval;
  }
}

