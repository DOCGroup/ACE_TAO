// -*- C++ -*-
// $Id$

#include "Connector2_exec.h"

namespace CIAO_SharedDP_Connector2_Connector2_Impl
{
  Connector2_exec_i::Connector2_exec_i (void)
    : DDS_Event_Connector_T<SharedDPTest2_DDS_Traits, SharedDPTest2_Connector_Traits> ()
  {
  }

  Connector2_exec_i::~Connector2_exec_i (void)
  {
  }

  extern "C" SHAREDDP_CONNECTOR2_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_SharedDP_Connector2_Impl (void)
  {
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_NORETURN (
      retval,
      Connector2_exec_i ());

    return retval;
  }
}

