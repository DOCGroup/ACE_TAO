// -*- C++ -*-
// $Id$

#include "VTopic_Connector_exec.h"

namespace CIAO_VTopic_VTopic_Connector_Impl
{
  VTopic_Connector_exec_i::VTopic_Connector_exec_i (void)
    : DDS_Event_Connector_T<TestTopic_DDS_Traits, TestTopic_Connector_Traits> ()
  {
  }

  VTopic_Connector_exec_i::~VTopic_Connector_exec_i (void)
  {
  }

  extern "C" VTOPIC_CONNECTOR_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_VTopic_VTopic_Connector_Impl (void)
  {
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_NORETURN (
      retval,
      VTopic_Connector_exec_i ()); 
    return retval;
  }
}

