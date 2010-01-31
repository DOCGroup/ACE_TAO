// -*- C++ -*-
// $Id$

#include "Connector3_exec.h"

namespace CIAO_SharedDP_Connector3_Connector3_Impl
{
  Connector3_exec_i::Connector3_exec_i (void)
    : DDS_Event_Connector_T<SharedDPTest3_DDS_Traits, SharedDPTest3_Connector_Traits> ()
  {
  }

  Connector3_exec_i::~Connector3_exec_i (void)
  {
  }

  extern "C" SHAREDDP_CONNECTOR3_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_SharedDP_Connector3_Impl (void)
  {
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_NORETURN (
      retval,
      Connector3_exec_i ());

    return retval;
  }
}

