// -*- C++ -*-
// $Id$

#include "PSL_SampleLost_Connector_exec.h"

namespace CIAO_PSL_SampleLost_PSL_SampleLost_Connector_Impl
{
  PSL_SampleLost_Connector_exec_i::PSL_SampleLost_Connector_exec_i (void)
    : DDS_Event_Connector_T<TestTopic_DDS_Traits, TestTopic_Connector_Traits> ()
  {
  }

  PSL_SampleLost_Connector_exec_i::~PSL_SampleLost_Connector_exec_i (void)
  {
  }

  extern "C" PSL_SAMPLELOST_CONNECTOR_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_PSL_SampleLost_PSL_SampleLost_Connector_Impl (void)
  {
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_NORETURN (
      retval,
      PSL_SampleLost_Connector_exec_i ()); 

    return retval;
  }
}
