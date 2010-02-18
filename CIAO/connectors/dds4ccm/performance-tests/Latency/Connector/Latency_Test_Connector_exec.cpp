// -*- C++ -*-
// $Id$

#include "Latency_Test_Connector_exec.h"

namespace CIAO_Latency_Test_Latency_Test_Connector_Impl
{
  Latency_Test_Connector_exec_i::Latency_Test_Connector_exec_i (void)
    : DDS_Event_Connector_T<LatencyTest_DDS_Traits, LatencyTest_Connector_Traits> ()
  {
  }

  Latency_Test_Connector_exec_i::~Latency_Test_Connector_exec_i (void)
  {
  }

  extern "C" LATENCY_TEST_CONNECTOR_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_Latency_Test_Latency_Test_Connector_Impl (void)
  {
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_NORETURN (
      retval,
      Latency_Test_Connector_exec_i ());

    return retval;
  }
}

