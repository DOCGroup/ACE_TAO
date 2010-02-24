// -*- C++ -*-
// $Id$

#include "VT_Array_Connector_exec.h"

namespace CIAO_VT_Array_VT_Array_Connector_Impl
{
  VT_Array_Connector_exec_i::VT_Array_Connector_exec_i (void)
    : DDS_Event_Connector_T<TopicArray_DDS_Traits, TopicArray_Connector_Traits> ()
  {
  }

  VT_Array_Connector_exec_i::~VT_Array_Connector_exec_i (void)
  {
  }

  extern "C" VT_ARRAY_CONNECTOR_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_VT_Array_VT_Array_Connector_Impl (void)
  {
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_NORETURN (
      retval,
      VT_Array_Connector_exec_i ()); 
    return retval;
  }
}
