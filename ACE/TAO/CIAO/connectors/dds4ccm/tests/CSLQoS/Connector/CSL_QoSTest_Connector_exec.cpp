// -*- C++ -*-
// $Id$

#include "CSL_QoSTest_Connector_exec.h"

namespace CIAO_CSL_QoSTest_CSL_QoSTest_Connector_Impl
{
  CSL_QoSTest_Connector_exec_i::CSL_QoSTest_Connector_exec_i (void)
    : DDS_Event_Connector_T<TestTopic_DDS_Traits, TestTopic_Connector_Traits> ()
  {
  }

  CSL_QoSTest_Connector_exec_i::~CSL_QoSTest_Connector_exec_i (void)
  {
  }

  extern "C" CSL_QOSTEST_CONNECTOR_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_CSL_QoSTest_CSL_QoSTest_Connector_Impl (void)
  {
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();
    ACE_NEW_NORETURN (
      retval,
      CSL_QoSTest_Connector_exec_i ());
    return retval;
  }
}
