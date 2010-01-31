// -*- C++ -*-
// $Id$

#include "Updater_Connector_exec.h"

namespace CIAO_Updater_Updater_Connector_Impl
{
  Updater_Connector_exec_i::Updater_Connector_exec_i (void)
    : DDS_State_Connector_T<TestTopic_DDS_Traits, TestTopic_Connector_Traits> ()
  {
  }

  Updater_Connector_exec_i::~Updater_Connector_exec_i (void)
  {
  }

  extern "C" UPDATER_CONNECTOR_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_Updater_Updater_Connector_Impl (void)
  {
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_NORETURN (
      retval,
      Updater_Connector_exec_i ());

    return retval;
  }
}

