// -*- C++ -*-
// $Id$

#include "Writer_Connector_exec.h"

namespace CIAO_Writer_Writer_Connector_Impl
{
  Writer_Connector_exec_i::Writer_Connector_exec_i (void)
    : DDS_Event_Connector_T<WriterTest_DDS_Traits, WriterTest_Connector_Traits> ()
  {
  }

  Writer_Connector_exec_i::~Writer_Connector_exec_i (void)
  {
  }

  extern "C" WRITER_CONNECTOR_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_Writer_Writer_Connector_Impl (void)
  {
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_NORETURN (
      retval,
      Writer_Connector_exec_i ());

    return retval;
  }
}

