// -*- C++ -*-
// $Id$

#include "Receptacle_exec.h"
#include "ciao/Logger/Log_Macros.h"

namespace CIAO_Receptacle_Impl
{
  //============================================================
  // Component Executor Implementation Class: Receptacle_exec_i
  //============================================================
  Receptacle_exec_i::Receptacle_exec_i (void)
  {
  }

  Receptacle_exec_i::~Receptacle_exec_i (void)
  {
  }

  void
  Receptacle_exec_i::set_session_context (
    ::Components::SessionContext_ptr ctx)
  {
    this->context_ =
      ::CCM_Receptacle_Context::_narrow (ctx);

    if ( ::CORBA::is_nil (this->context_.in ()))
      {
        throw ::CORBA::INTERNAL ();
      }
  }

  void
  Receptacle_exec_i::configuration_complete (void)
  {
  }

  void
  Receptacle_exec_i::ccm_activate (void)
  {
  }

  void
  Receptacle_exec_i::ccm_passivate (void)
  {
  }

  void
  Receptacle_exec_i::ccm_remove (void)
  {
  }

  extern "C" RECEPTACLE_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_Receptacle_Impl (void)
  {
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_NORETURN (
      retval,
      Receptacle_exec_i);

    return retval;
  }
}

namespace CIAO_Provider_Impl
{
  //============================================================
  // Component Executor Implementation Class: Provider_exec_i
  //============================================================
  Provider_exec_i::Provider_exec_i (void)
  {
  }

  Provider_exec_i::~Provider_exec_i (void)
  {
  }

  // Component attributes and port operations.
  ::CCM_foo_ptr
  Provider_exec_i::get_prov_foo (void)
  {
    return ::CCM_foo::_nil ();
  }

  void
  Provider_exec_i::set_session_context (
    ::Components::SessionContext_ptr ctx)
  {
    this->context_ =
      ::CCM_Provider_Context::_narrow (ctx);

    if ( ::CORBA::is_nil (this->context_.in ()))
      {
        throw ::CORBA::INTERNAL ();
      }
  }

  void
  Provider_exec_i::configuration_complete (void)
  {
  }

  void
  Provider_exec_i::ccm_activate (void)
  {
  }

  void
  Provider_exec_i::ccm_passivate (void)
  {
  }

  void
  Provider_exec_i::ccm_remove (void)
  {
    /* Your code here. */
  }

  extern "C" RECEPTACLE_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_Provider_Impl (void)
  {
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_NORETURN (
      retval,
      Provider_exec_i);

    return retval;
  }
}

