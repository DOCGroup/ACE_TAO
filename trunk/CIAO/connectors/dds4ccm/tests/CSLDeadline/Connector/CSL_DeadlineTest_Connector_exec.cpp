// -*- C++ -*-
// $Id$

#include "CSL_DeadlineTest_Connector_exec.h"

namespace CIAO_CSL_DeadlineTest_CSL_DeadlineTest_Connector_Impl
{
  CSL_DeadlineTest_Connector_exec_i::CSL_DeadlineTest_Connector_exec_i (const char * /*topic_name*/)
    : DDS_Event_Connector_T<TestTopic_DDS_Traits, TestTopic_Connector_Traits> ()
  {
  }

  CSL_DeadlineTest_Connector_exec_i::~CSL_DeadlineTest_Connector_exec_i (void)
  {
  }

  extern "C" CSL_DEADLINETEST_CONNECTOR_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_CSL_DeadlineTest_CSL_DeadlineTest_Connector_Impl (void)
  {
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_NORETURN (
      retval,
      CSL_DeadlineTest_Connector_exec_i ("Square")); //should be set by dep. plan.

    return retval;
  }
}

