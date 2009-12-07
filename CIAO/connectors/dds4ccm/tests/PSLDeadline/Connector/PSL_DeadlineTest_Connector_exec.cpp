// -*- C++ -*-
// $Id$

#include "PSL_DeadlineTest_Connector_exec.h"

namespace CIAO_PSL_DeadlineTest_PSL_DeadlineTest_Connector_Impl
{
  PSL_DeadlineTest_Connector_exec_i::PSL_DeadlineTest_Connector_exec_i (const char * /*topic_name*/)
    : DDS_Event_Connector_T<TestTopic_DDS_Traits, TestTopic_Connector_Traits> ()
  {
  }

  PSL_DeadlineTest_Connector_exec_i::~PSL_DeadlineTest_Connector_exec_i (void)
  {
  }

  extern "C" PSL_DEADLINETEST_CONNECTOR_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_PSL_DeadlineTest_PSL_DeadlineTest_Connector_Impl (void)
  {
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_NORETURN (
      retval,
      PSL_DeadlineTest_Connector_exec_i ("Square")); //should be set by dep. plan.

    return retval;
  }
}

