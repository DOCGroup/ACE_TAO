// -*- C++ -*-
// $Id$

#include "State_Connection_Test_Connector_exec.h"

namespace CIAO_State_Connection_Test_State_Connection_Test_Connector_Impl
{
  State_Connection_Test_Connector_exec_i::State_Connection_Test_Connector_exec_i ()
    : DDS_State_Connector_T<State_ConnectionTest_DDS_Traits, State_ConnectionTest_Connector_Traits> ()
  {
  }

  State_Connection_Test_Connector_exec_i::~State_Connection_Test_Connector_exec_i (void)
  {
  }

  extern "C" STATE_CONNECTION_CONNECTOR_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_State_Connection_Test_State_Connection_Test_Connector_Impl (void)
  {
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_NORETURN (
      retval,
      State_Connection_Test_Connector_exec_i ());

    return retval;
  }
}

