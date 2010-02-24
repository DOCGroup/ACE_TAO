// -*- C++ -*-
// $Id$

#include "VT_Seq_Connector_exec.h"

namespace CIAO_VT_Seq_VT_Seq_Connector_Impl
{
  VT_Seq_Connector_exec_i::VT_Seq_Connector_exec_i (void)
    : DDS_Event_Connector_T<TopicSeq_DDS_Traits, TopicSeq_Connector_Traits> ()
  {
  }

  VT_Seq_Connector_exec_i::~VT_Seq_Connector_exec_i (void)
  {
  }

  extern "C" VT_SEQ_CONNECTOR_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_VT_Seq_VT_Seq_Connector_Impl (void)
  {
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_NORETURN (
      retval,
      VT_Seq_Connector_exec_i ()); 
    return retval;
  }
}
