// -*- C++ -*-
// $Id$

#include "Unkeyed_Test_Connector_exec.h"

namespace CIAO_Unkeyed_Test_Unkeyed_Test_Connector_Impl
{
  Unkeyed_Test_Connector_exec_i::Unkeyed_Test_Connector_exec_i (const char * topic_name)
    : Connector_T<UnkeyedTest_DDS_Traits, UnkeyedTest_Connector_Traits> (topic_name)
  {
  }

  Unkeyed_Test_Connector_exec_i::~Unkeyed_Test_Connector_exec_i (void)
  {
  }

  extern "C" UNKEYED_TEST_CONNECTOR_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_Unkeyed_Test_Unkeyed_Test_Connector_Impl (void)
  {
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_NORETURN (
      retval,
      Unkeyed_Test_Connector_exec_i ("Default_Topic_Name")); //should be set by dep. plan.

    return retval;
  }
}

