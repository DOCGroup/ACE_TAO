// -*- C++ -*-
// $Id$

#include "Unkeyed_Test_Connector_exec.h"

namespace CIAO_Unkeyed_Test_Unkeyed_Test_Connector_Impl
{
  Unkeyed_Test_Connector_exec_i::Unkeyed_Test_Connector_exec_i (void)
    : DDS_Event_Connector_T<UnkeyedTest_DDS_Traits, UnkeyedTest_Connector_Traits> ()
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
      Unkeyed_Test_Connector_exec_i ());

    return retval;
  }
}

