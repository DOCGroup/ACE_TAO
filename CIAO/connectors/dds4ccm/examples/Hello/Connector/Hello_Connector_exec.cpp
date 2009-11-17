// -*- C++ -*-
// $Id$

#include "Hello_Connector_exec.h"

namespace CIAO_Hello_Hello_Connector_Impl
{
  Hello_Connector_exec_i::Hello_Connector_exec_i (const char * topic_name)
    : DDS_Event_Connector_T<DDSHello_DDS_Traits, DDSHello_Connector_Traits> (topic_name)
  {
  }

  Hello_Connector_exec_i::~Hello_Connector_exec_i (void)
  {
  }

  extern "C" HELLO_CONNECTOR_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_Hello_Hello_Connector_Impl (void)
  {
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_NORETURN (
      retval,
      Hello_Connector_exec_i ("Default_Topic_Name")); //should be set by dep. plan.

    return retval;
  }
}

