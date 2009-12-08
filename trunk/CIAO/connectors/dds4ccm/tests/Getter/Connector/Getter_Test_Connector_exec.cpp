// -*- C++ -*-
// $Id$

#include "Getter_Test_Connector_exec.h"

namespace CIAO_Getter_Test_Getter_Test_Connector_Impl
{
  Getter_Test_Connector_exec_i::Getter_Test_Connector_exec_i (void)
    : DDS_Event_Connector_T<GetterTest_DDS_Traits, GetterTest_Connector_Traits> ()
  {
  }

  Getter_Test_Connector_exec_i::~Getter_Test_Connector_exec_i (void)
  {
  }

  extern "C" GETTER_TEST_CONNECTOR_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_Getter_Test_Getter_Test_Connector_Impl (void)
  {
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_NORETURN (
      retval,
      Getter_Test_Connector_exec_i ());

    return retval;
  }
}

