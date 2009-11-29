// -*- C++ -*-
// $Id$

#include "CSL_TestSec_Connector_exec.h"

namespace CIAO_ConnectorStatusListener_TestSec_ConnectorStatusListener_TestSec_Connector_Impl
{
  ConnectorStatusListener_TestSec_Connector_exec_i::ConnectorStatusListener_TestSec_Connector_exec_i (void)
    : DDS_Event_Connector_T<TestSecondTopic_DDS_Traits, TestSecondTopic_Connector_Traits> ()
  {
  }

  ConnectorStatusListener_TestSec_Connector_exec_i::~ConnectorStatusListener_TestSec_Connector_exec_i (void)
  {
  }

  extern "C" CONNECTORSTATUSLISTENER_TESTSEC_CONNECTOR_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_ConnectorStatusListener_TestSec_ConnectorStatusListener_TestSec_Connector_Impl (void)
  {
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_NORETURN (
      retval,
      ConnectorStatusListener_TestSec_Connector_exec_i ()); //should be set by dep. plan.

    return retval;
  }
}

