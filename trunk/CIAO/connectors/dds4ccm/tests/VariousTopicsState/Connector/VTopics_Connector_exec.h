// -*- C++ -*-
// $Id$

#ifndef VTOPICS_CONNECTOR_EXEC_H_
#define VTOPICS_CONNECTOR_EXEC_H_

#include /**/ "ace/pre.h"

#include "VTopics_ConnectorEC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include /**/ "VTopics_Connector_exec_export.h"

#include "dds4ccm/impl/ndds/DDS4CCM_Traits.h"
#include "dds4ccm/impl/ndds/DDS_Event_Connector_T.h"


#include "Base/VTopics_BaseSupport.h"

namespace CIAO_VTopics_VTopics_Connector_Impl
{

  // @from use of VTopics as a parameter
  typedef CIAO::DDS4CCM::RTI::Type_Traits <
    TestTopic,
    TestTopicSeq,
    TestTopicTypeSupport,
    TestTopicDataWriter,
    TestTopicDataReader > TestTopic_DDS_Traits;

  typedef CIAO::DDS4CCM::Connector_Traits <
    ::CIAO_VTopics_VTopics_Connector_Impl::VTopics_Connector_Exec,
    TestTopic_Seq,
    ::CCM_DDS::TestTopic::CCM_Writer,
    ::CCM_DDS::TestTopic::CCM_Updater,
    ::CCM_DDS::TestTopic::CCM_Getter,
    ::CCM_DDS::TestTopic::CCM_Reader,
    ::VTopics::CCM_VTopics_Connector_Context,
    ::CCM_DDS::TestTopic::Listener,
    ::CCM_DDS::TestTopic::StateListener,
    ::CCM_DDS::ConnectorStatusListener> TestTopic_Connector_Traits;

  class VTOPICS_CONNECTOR_EXEC_Export VTopics_Connector_exec_i :
    public DDS_Event_Connector_T <TestTopic_DDS_Traits, TestTopic_Connector_Traits>
  {
  public:
    VTopics_Connector_exec_i (void);
    virtual ~VTopics_Connector_exec_i (void);
  };

  extern "C" VTOPICS_CONNECTOR_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_VTopics_VTopics_Connector_Impl (void);
}

#include /**/ "ace/post.h"

#endif /* ifndef */

