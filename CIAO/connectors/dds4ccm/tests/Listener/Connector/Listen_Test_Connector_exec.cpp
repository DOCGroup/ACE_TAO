// -*- C++ -*-
// $Id$

#include "Listen_Test_Connector_exec.h"

namespace CIAO_Listen_Test_Listen_Test_Connector_Impl
{
  Listen_Test_Connector_exec_i::Listen_Test_Connector_exec_i (void)
    : DDS_Event_Connector_T<ListenTest_DDS_Traits, ListenTest_Connector_Traits> ()
  {
  }

  Listen_Test_Connector_exec_i::~Listen_Test_Connector_exec_i (void)
  {
  }

  extern "C" LISTEN_TEST_CONNECTOR_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_Listen_Test_Listen_Test_Connector_Impl (void)
  {
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_NORETURN (
      retval,
      Listen_Test_Connector_exec_i ());

    return retval;
  }
}

