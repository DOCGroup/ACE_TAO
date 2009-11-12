// -*- C++ -*-
// $Id$

#include "Keyed_Test_Connector_exec.h"

namespace CIAO_Keyed_Test_Keyed_Test_Connector_Impl
{
  Keyed_Test_Connector_exec_i::Keyed_Test_Connector_exec_i (const char * topic_name)
    : Connector_T<KeyedTest_DDS_Traits, KeyedTest_Connector_Traits> (topic_name)
  {
  }

  Keyed_Test_Connector_exec_i::~Keyed_Test_Connector_exec_i (void)
  {
  }

  extern "C" KEYED_TEST_CONNECTOR_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_Keyed_Test_Keyed_Test_Connector_Impl (void)
  {
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_NORETURN (
      retval,
      Keyed_Test_Connector_exec_i ("Default_Topic_Name")); //should be set by dep. plan.

    return retval;
  }
}

