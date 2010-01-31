// -*- C++ -*-
// $Id$

#include "CSL_USTest_Connector_exec.h"

namespace CIAO_CSL_USTest_CSL_USTest_Connector_Impl
{
  CSL_USTest_Connector_exec_i::CSL_USTest_Connector_exec_i (void)
    : DDS_Event_Connector_T<TestTopic_DDS_Traits, TestTopic_Connector_Traits> ()
  {
  }

  CSL_USTest_Connector_exec_i::~CSL_USTest_Connector_exec_i (void)
  {
  }

  extern "C" CSL_USTEST_CONNECTOR_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_CSL_USTest_CSL_USTest_Connector_Impl (void)
  {
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_NORETURN (
      retval,
      CSL_USTest_Connector_exec_i ()); 
    return retval;
  }
}

