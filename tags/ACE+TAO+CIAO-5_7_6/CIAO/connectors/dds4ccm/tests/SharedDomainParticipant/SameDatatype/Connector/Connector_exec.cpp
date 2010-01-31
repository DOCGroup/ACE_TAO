// -*- C++ -*-
// $Id$

#include "Connector_exec.h"

namespace CIAO_SharedDP_Connector_SDP_Connector_Impl
{
  Connector_exec_i::Connector_exec_i (void)
    : DDS_Event_Connector_T<SharedDPTest_DDS_Traits, SharedDPTest_Connector_Traits> ()
  {
  }

  Connector_exec_i::~Connector_exec_i (void)
  {
  }

  extern "C" SHAREDDP_CONNECTOR_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_SharedDP_Connector_Impl (void)
  {
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_NORETURN (
      retval,
      Connector_exec_i ());

    return retval;
  }
}

