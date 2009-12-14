// -*- C++ -*-
// $Id$

#include "LMBM_Test_Connector_exec.h"

namespace CIAO_LMBM_Test_LMBM_Test_Connector_Impl
{
  LMBM_Test_Connector_exec_i::LMBM_Test_Connector_exec_i (void)
    : DDS_Event_Connector_T<ListenManyByManyTest_DDS_Traits, ListenManyByManyTest_Connector_Traits> ()
  {
  }

  LMBM_Test_Connector_exec_i::~LMBM_Test_Connector_exec_i (void)
  {
  }

  extern "C" LISTEN_MANY_BY_MANY_TEST_CONNECTOR_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_LMBM_Test_LMBM_Test_Connector_Impl (void)
  {
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_NORETURN (
      retval,
      LMBM_Test_Connector_exec_i ());

    return retval;
  }
}

