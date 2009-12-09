// -*- C++ -*-
// $Id$

#include "Event_Connection_Test_Connector_exec.h"

namespace CIAO_Event_Connection_Test_Event_Connection_Test_Connector_Impl
{
  Event_Connection_Test_Connector_exec_i::Event_Connection_Test_Connector_exec_i ()
    : DDS_Event_Connector_T<Event_ConnectionTest_DDS_Traits, Event_ConnectionTest_Connector_Traits> ()
  {
  }

  Event_Connection_Test_Connector_exec_i::~Event_Connection_Test_Connector_exec_i (void)
  {
  }

  extern "C" EVENT_CONNECTION_CONNECTOR_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_Event_Connection_Test_Event_Connection_Test_Connector_Impl (void)
  {
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_NORETURN (
      retval,
      Event_Connection_Test_Connector_exec_i ());

    return retval;
  }
}

