// -*- C++ -*-
// $Id$

#include "Reader_Test_Connector_exec.h"

namespace CIAO_Reader_Test_Reader_Test_Connector_Impl
{
  Reader_Test_Connector_exec_i::Reader_Test_Connector_exec_i (void)
    : DDS_Event_Connector_T<ReaderTest_DDS_Traits, ReaderTest_Connector_Traits> ()
  {
  }

  Reader_Test_Connector_exec_i::~Reader_Test_Connector_exec_i (void)
  {
  }

  extern "C" READER_TEST_CONNECTOR_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_Reader_Test_Reader_Test_Connector_Impl (void)
  {
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_NORETURN (
      retval,
      Reader_Test_Connector_exec_i ());

    return retval;
  }
}

