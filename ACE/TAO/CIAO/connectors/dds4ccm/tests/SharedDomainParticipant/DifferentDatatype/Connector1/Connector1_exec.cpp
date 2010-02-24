// -*- C++ -*-
// $Id$

#include "Connector1_exec.h"

namespace CIAO_SharedDP_Connector1_Connector1_Impl
{
  Connector1_exec_i::Connector1_exec_i (void)
    : DDS_Event_Connector_T<SharedDPTest1_DDS_Traits, SharedDPTest1_Connector_Traits> ()
  {
  }

  Connector1_exec_i::~Connector1_exec_i (void)
  {
  }

  extern "C" SHAREDDP_CONNECTOR1_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_SharedDP_Connector1_Impl (void)
  {
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_NORETURN (
      retval,
      Connector1_exec_i ());

    return retval;
  }
}

