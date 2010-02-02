// -*- C++ -*-
// $Id$

#ifndef PSL_DEADLINETEST_CONNECTOR_EXEC_H_
#define PSL_DEADLINETEST_CONNECTOR_EXEC_H_

#include /**/ "ace/pre.h"

#include "PSL_DeadlineTest_ConnectorEC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include /**/ "PSL_DeadlineTest_Connector_exec_export.h"

#include "dds4ccm/impl/ndds/DDS4CCM_Traits.h"
#include "dds4ccm/impl/ndds/DDS_Event_Connector_T.h"


#include "Base/PSL_DeadlineTest_BaseSupport.h"

namespace CIAO_PSL_DeadlineTest_PSL_DeadlineTest_Connector_Impl
{

  // @from use of PSL_DeadlineTest as a parameter
  typedef CIAO::DDS4CCM::RTI::Type_Traits <
    TestTopic,
    TestTopicSeq,
    TestTopicTypeSupport,
    TestTopicDataWriter,
    TestTopicDataReader > TestTopic_DDS_Traits;

  typedef CIAO::DDS4CCM::Connector_Traits <
    ::CIAO_PSL_DeadlineTest_PSL_DeadlineTest_Connector_Impl::PSL_DeadlineTest_Connector_Exec,
    TestTopic_Seq,
    ::CCM_DDS::TestTopic::CCM_Writer,
    ::CCM_DDS::TestTopic::CCM_Updater,
    ::CCM_DDS::TestTopic::CCM_Getter,
    ::CCM_DDS::TestTopic::CCM_Reader,
    ::PSL_DeadlineTest::CCM_PSL_DeadlineTest_Connector_Context,
    ::CCM_DDS::TestTopic::Listener,
    ::CCM_DDS::TestTopic::StateListener,
    ::CCM_DDS::ConnectorStatusListener> TestTopic_Connector_Traits;

  class PSL_DEADLINETEST_CONNECTOR_EXEC_Export PSL_DeadlineTest_Connector_exec_i :
    public DDS_Event_Connector_T <TestTopic_DDS_Traits, TestTopic_Connector_Traits>
  {
  public:
    PSL_DeadlineTest_Connector_exec_i (void);
    virtual ~PSL_DeadlineTest_Connector_exec_i (void);
  };

  extern "C" PSL_DEADLINETEST_CONNECTOR_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_PSL_DeadlineTest_PSL_DeadlineTest_Connector_Impl (void);
}

#include /**/ "ace/post.h"

#endif /* ifndef */

