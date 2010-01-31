// -*- C++ -*-
// $Id$

#include "LOBO_Test_Connector_exec.h"

namespace CIAO_LOBO_Test_LOBO_Test_Connector_Impl
{
  LOBO_Test_Connector_exec_i::LOBO_Test_Connector_exec_i (void)
    : DDS_Event_Connector_T<ListenOneByOneTest_DDS_Traits, ListenOneByOneTest_Connector_Traits> ()
  {
  }

  LOBO_Test_Connector_exec_i::~LOBO_Test_Connector_exec_i (void)
  {
  }

  extern "C" LISTEN_ONE_BY_ONE_TEST_CONNECTOR_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_LOBO_Test_LOBO_Test_Connector_Impl (void)
  {
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_NORETURN (
      retval,
      LOBO_Test_Connector_exec_i ());

    return retval;
  }
}

