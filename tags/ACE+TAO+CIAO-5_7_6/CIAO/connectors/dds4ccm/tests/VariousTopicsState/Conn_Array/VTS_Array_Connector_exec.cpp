// -*- C++ -*-
// $Id$

#include "VTS_Array_Connector_exec.h"

namespace CIAO_VTS_Array_VTS_Array_Connector_Impl
{
  VTS_Array_Connector_exec_i::VTS_Array_Connector_exec_i (void)
    : DDS_Event_Connector_T<TopicArray_DDS_Traits, TopicArray_Connector_Traits> ()
  {
  }

  VTS_Array_Connector_exec_i::~VTS_Array_Connector_exec_i (void)
  {
  }

  extern "C" VTS_ARRAY_CONNECTOR_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_VTS_Array_VTS_Array_Connector_Impl (void)
  {
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_NORETURN (
      retval,
      VTS_Array_Connector_exec_i ()); 
    return retval;
  }
}
