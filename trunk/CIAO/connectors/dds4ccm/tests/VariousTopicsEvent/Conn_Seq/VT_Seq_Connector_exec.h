// -*- C++ -*-
// $Id$

#ifndef VT_SEQ_CONNECTOR_EXEC_H_
#define VT_SEQ_CONNECTOR_EXEC_H_

#include /**/ "ace/pre.h"

#include "VT_Seq_ConnectorEC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include /**/ "VT_Seq_Connector_exec_export.h"
#include "tao/LocalObject.h"

#include "dds4ccm/impl/ndds/DDS4CCM_Traits.h"
#include "dds4ccm/impl/ndds/DDS_Event_Connector_T.h"


#include "Base/VTopic_BaseSupport.h"

namespace CIAO_VT_Seq_VT_Seq_Connector_Impl
{

  // @from use of VTopic as a parameter
  typedef CIAO::DDS4CCM::RTI::Type_Traits < 
    TopicSeq,
    TopicSeqSeq,
    TopicSeqTypeSupport,
    TopicSeqDataWriter,
    TopicSeqDataReader > TopicSeq_DDS_Traits;

  typedef CIAO::DDS4CCM::Connector_Traits <
    ::CIAO_VTopic_VT_Seq_Connector_Impl::VT_Seq_Connector_Exec,
    TopicSeq_Seq,
    ::CCM_DDS::TopicSeq::CCM_Writer,
    ::CCM_DDS::TopicSeq::CCM_Updater,
    ::CCM_DDS::TopicSeq::CCM_Getter,
    ::CCM_DDS::TopicSeq::CCM_Reader,
    ::VTopic::CCM_VT_Seq_Connector_Context,
    ::CCM_DDS::TopicSeq::Listener,
    ::CCM_DDS::TopicSeq::StateListener,
    ::CCM_DDS::ConnectorStatusListener> TopicSeq_Connector_Traits;

  class VT_SEQ_CONNECTOR_EXEC_Export VT_Seq_Connector_exec_i :
    public DDS_Event_Connector_T <TopicSeq_DDS_Traits, TopicSeq_Connector_Traits>
  {
  public:
    VT_Seq_Connector_exec_i (void);
    virtual ~VT_Seq_Connector_exec_i (void);
  };

  extern "C" VT_SEQ_CONNECTOR_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_VT_Seq_VT_Seq_Connector_Impl (void);
}

#include /**/ "ace/post.h"

#endif /* ifndef */
