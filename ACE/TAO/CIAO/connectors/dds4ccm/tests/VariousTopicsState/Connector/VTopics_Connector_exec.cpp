// -*- C++ -*-
// $Id$

#include "VTopics_Connector_exec.h"

namespace CIAO_VTopics_VTopics_Connector_Impl
{
  VTopics_Connector_exec_i::VTopics_Connector_exec_i (void)
    : DDS_Event_Connector_T<TestTopic_DDS_Traits, TestTopic_Connector_Traits> ()
  {
  }

  VTopics_Connector_exec_i::~VTopics_Connector_exec_i (void)
  {
  }

  extern "C" VTOPICS_CONNECTOR_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_VTopics_VTopics_Connector_Impl (void)
  {
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_NORETURN (
      retval,
      VTopics_Connector_exec_i ()); 
    return retval;
  }
}

