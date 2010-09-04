// -*- C++ -*-
//
// $Id$

#include "ParseError_Test_Component_exec.h"
#include "tao/ORB_Core.h"
#include "ace/Reactor.h"

#define NR_OF_KEYS 10

namespace CIAO_ParseError_Test_ParseErrorTestComponent_Impl
{
  //============================================================
  // Component_exec_i
  //============================================================
  Component_exec_i::Component_exec_i (void)
  {
  }

  Component_exec_i::~Component_exec_i (void)
  {
  }

  CCM_ParseErrorTest_ptr
  Component_exec_i::get_to_do()
  {
    return CCM_ParseErrorTest::_nil ();
  }

  CORBA::ULong
  Component_exec_i::test()
  {
    return 0;
  }

  void
  Component_exec_i::test(CORBA::ULong)
  {
  }

  // Operations from Components::SessionComponent.
  void
  Component_exec_i::set_session_context (
    ::Components::SessionContext_ptr ctx)
  {
    this->context_ =
      ::ParseError_Test::CCM_ParseErrorTestComponent_Context::_narrow (ctx);
    if ( ::CORBA::is_nil (this->context_.in ()))
      {
        throw ::CORBA::INTERNAL ();
      }
  }

  void
  Component_exec_i::configuration_complete (void)
  {
  }

  void
  Component_exec_i::ccm_activate (void)
  {
    ACE_ERROR ((LM_ERROR, "ERROR: Component shouldn't be started\n"));
  }

  void
  Component_exec_i::ccm_passivate (void)
  {
  }

  void
  Component_exec_i::ccm_remove (void)
  {
  }

  extern "C" COMPONENT_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_ParseErrorTest_Component_Impl (void)
  {
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_NORETURN (
      retval,
      Component_exec_i);

    return retval;
  }
}

