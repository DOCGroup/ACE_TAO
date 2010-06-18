// -*- C++ -*-
//
// $Id$

#include "CFT_Test_Component_exec.h"

namespace CIAO_CFT_Test_CFTTestComponent_Impl
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

  // Operations from Components::SessionComponent.
  void
  Component_exec_i::set_session_context (
    ::Components::SessionContext_ptr ctx)
  {
    this->context_ =
      ::CFT_Test::CCM_CFTTestComponent_Context::_narrow (ctx);
    if ( ::CORBA::is_nil (this->context_.in ()))
      {
        throw ::CORBA::INTERNAL ();
      }
  }

  ::CCM_DDS::CCM_PortStatusListener_ptr
  Component_exec_i::get_two_dim_get_status (void)
  {
    return ::CCM_DDS::CCM_PortStatusListener::_nil ();
  }

  ::CCM_DDS::CCM_PortStatusListener_ptr
  Component_exec_i::get_three_dim_get_status (void)
  {
    return ::CCM_DDS::CCM_PortStatusListener::_nil ();
  }

  ::CCM_DDS::CCM_PortStatusListener_ptr
  Component_exec_i::get_three_dim_read_status (void)
  {
    return ::CCM_DDS::CCM_PortStatusListener::_nil ();
  }

  void
  Component_exec_i::configuration_complete (void)
  {
  }

  void
  Component_exec_i::ccm_activate (void)
  {
    // if deployment succeeds, this test succeeds
    ACE_DEBUG ((LM_DEBUG, "Component_exec_i::ccm_activate - "
                          "OK : Test succeeded. Two "
                          "ContentfilteredTopics can "
                          "be created.\n"));
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
  create_CFTTest_Component_Impl (void)
  {
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_NORETURN (
      retval,
      Component_exec_i);

    return retval;
  }
}

