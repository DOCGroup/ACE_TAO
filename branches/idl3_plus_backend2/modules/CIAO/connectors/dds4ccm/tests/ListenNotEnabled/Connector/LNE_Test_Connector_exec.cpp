// -*- C++ -*-
// $Id$

#include "LNE_Test_Connector_exec.h"

namespace CIAO_LNE_Test_LNE_Test_Connector_Impl
{
  LNE_Test_Connector_exec_i::LNE_Test_Connector_exec_i (void)
    : DDS_Event_Connector_T<ListenNotEnabledTest_DDS_Traits, ListenNotEnabledTest_Connector_Traits> ()
  {
  }

  LNE_Test_Connector_exec_i::~LNE_Test_Connector_exec_i (void)
  {
  }

  extern "C" LISTEN_NOT_ENABLED_TEST_CONNECTOR_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_LNE_Test_LNE_Test_Connector_Impl (void)
  {
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_NORETURN (
      retval,
      LNE_Test_Connector_exec_i ());

    return retval;
  }
}

