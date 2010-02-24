// -*- C++ -*-
// $Id$

#include "VTS_Seq_Connector_exec.h"

namespace CIAO_VTS_Seq_VTS_Seq_Connector_Impl
{
  VTS_Seq_Connector_exec_i::VTS_Seq_Connector_exec_i (void)
    : DDS_Event_Connector_T<TopicSeq_DDS_Traits, TopicSeq_Connector_Traits> ()
  {
  }

  VTS_Seq_Connector_exec_i::~VTS_Seq_Connector_exec_i (void)
  {
  }

  extern "C" VTS_SEQ_CONNECTOR_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_VTS_Seq_VTS_Seq_Connector_Impl (void)
  {
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_NORETURN (
      retval,
      VTS_Seq_Connector_exec_i ()); 
    return retval;
  }
}
