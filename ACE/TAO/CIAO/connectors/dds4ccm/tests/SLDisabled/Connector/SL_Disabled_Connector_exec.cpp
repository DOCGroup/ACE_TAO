// -*- C++ -*-
// $Id$

#include "SL_Disabled_Connector_exec.h"

namespace CIAO_SL_Disabled_SL_Disabled_Connector_Impl
{
  SL_Disabled_Connector_exec_i::SL_Disabled_Connector_exec_i (void)
    : DDS_State_Connector_T<TestTopic_DDS_Traits, TestTopic_Connector_Traits> ()
  {
  }

  SL_Disabled_Connector_exec_i::~SL_Disabled_Connector_exec_i (void)
  {
  }

  extern "C" SL_DISABLED_CONNECTOR_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_SL_Disabled_SL_Disabled_Connector_Impl (void)
  {
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_NORETURN (
      retval,
      SL_Disabled_Connector_exec_i ());

    return retval;
  }
}

