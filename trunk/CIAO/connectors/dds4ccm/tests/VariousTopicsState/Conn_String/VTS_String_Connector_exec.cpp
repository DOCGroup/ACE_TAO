// -*- C++ -*-
// $Id$

#include "VTS_String_Connector_exec.h"

namespace CIAO_VTS_String_VTS_String_Connector_Impl
{
  VTS_String_Connector_exec_i::VTS_String_Connector_exec_i (void)
    : DDS_Event_Connector_T<TopicString_DDS_Traits, TopicString_Connector_Traits> ()
  {
  }

  VTS_String_Connector_exec_i::~VTS_String_Connector_exec_i (void)
  {
  }

  extern "C" VTS_STRING_CONNECTOR_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_VTS_String_VTS_String_Connector_Impl (void)
  {
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_NORETURN (
      retval,
      VTS_String_Connector_exec_i ()); 
    return retval;
  }
}
