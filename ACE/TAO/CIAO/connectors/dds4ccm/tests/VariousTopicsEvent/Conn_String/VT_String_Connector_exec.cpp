// -*- C++ -*-
// $Id$

#include "VT_String_Connector_exec.h"

namespace CIAO_VT_String_VT_String_Connector_Impl
{
  VT_String_Connector_exec_i::VT_String_Connector_exec_i (void)
    : DDS_Event_Connector_T<TopicString_DDS_Traits, TopicString_Connector_Traits> ()
  {
  }

  VT_String_Connector_exec_i::~VT_String_Connector_exec_i (void)
  {
  }

  extern "C" VT_STRING_CONNECTOR_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_VT_String_VT_String_Connector_Impl (void)
  {
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_NORETURN (
      retval,
      VT_String_Connector_exec_i ()); 
    return retval;
  }
}
