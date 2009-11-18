// -*- C++ -*-
// $Id$

#include "Get_Connection_Test_Connector_exec.h"

namespace CIAO_Get_Connection_Test_Get_Connection_Test_Connector_Impl
{
  Get_Connection_Test_Connector_exec_i::Get_Connection_Test_Connector_exec_i (const char * topic_name)
    : DDS_Event_Connector_T<Get_ConnectionTest_DDS_Traits, Get_ConnectionTest_Connector_Traits> (topic_name)
  {
  }

  Get_Connection_Test_Connector_exec_i::~Get_Connection_Test_Connector_exec_i (void)
  {
  }

  extern "C" KEYED_TEST_CONNECTOR_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_Get_Connection_Test_Get_Connection_Test_Connector_Impl (void)
  {
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_NORETURN (
      retval,
      Get_Connection_Test_Connector_exec_i ("Default_Topic_Name")); //should be set by dep. plan.

    return retval;
  }
}

