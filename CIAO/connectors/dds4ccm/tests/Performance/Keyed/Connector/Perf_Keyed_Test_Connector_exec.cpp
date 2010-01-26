// -*- C++ -*-
// $Id$

#include "Perf_Keyed_Test_Connector_exec.h"

namespace CIAO_Perf_Keyed_Test_Perf_Keyed_Test_Connector_Impl
{
  Perf_Keyed_Test_Connector_exec_i::Perf_Keyed_Test_Connector_exec_i (void)
    : DDS_Event_Connector_T<PerfKeyedTest_DDS_Traits, PerfKeyedTest_Connector_Traits> ()
  {
  }

  Perf_Keyed_Test_Connector_exec_i::~Perf_Keyed_Test_Connector_exec_i (void)
  {
  }

  extern "C" PERF_KEYED_TEST_CONNECTOR_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_Perf_Keyed_Test_Perf_Keyed_Test_Connector_Impl (void)
  {
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_NORETURN (
      retval,
      Perf_Keyed_Test_Connector_exec_i ());

    return retval;
  }
}

