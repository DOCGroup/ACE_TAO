// -*- C++ -*-
// $Id$

#ifndef CSL_QOSTEST_CONNECTOR_EXEC_H_
#define CSL_QOSTEST_CONNECTOR_EXEC_H_

#include /**/ "ace/pre.h"

#include "CSL_QoSTest_ConnectorEC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include /**/ "CSL_QoSTest_Connector_exec_export.h"
#include "tao/LocalObject.h"

#include "dds4ccm/impl/ndds/DDS4CCM_Traits.h"
#include "dds4ccm/impl/ndds/DDS_Event_Connector_T.h"


#include "Base/CSL_QoSTest_BaseSupport.h"

namespace CIAO_CSL_QoSTest_CSL_QoSTest_Connector_Impl
{

  // @from use of CSL_QoSTest as a parameter
  typedef CIAO::DDS4CCM::RTI::Type_Traits < 
    TestTopic,
    TestTopicSeq,
    TestTopicTypeSupport,
    TestTopicDataWriter,
    TestTopicDataReader > TestTopic_DDS_Traits;

  typedef CIAO::DDS4CCM::Connector_Traits <
    ::CIAO_CSL_QoSTest_CSL_QoSTest_Connector_Impl::CSL_QoSTest_Connector_Exec,
    TestTopic_Seq,
    ::CCM_DDS::TestTopic::CCM_Writer,
    ::CCM_DDS::TestTopic::CCM_Updater,
    ::CCM_DDS::TestTopic::CCM_Getter,
    ::CCM_DDS::TestTopic::CCM_Reader,
    ::CSL_QoSTest::CCM_CSL_QoSTest_Connector_Context,
    ::CCM_DDS::TestTopic::Listener,
    ::CCM_DDS::TestTopic::StateListener,
    ::CCM_DDS::ConnectorStatusListener> TestTopic_Connector_Traits;

  class CSL_QOSTEST_CONNECTOR_EXEC_Export CSL_QoSTest_Connector_exec_i :
    public DDS_Event_Connector_T <TestTopic_DDS_Traits, TestTopic_Connector_Traits>
  {
  public:
    CSL_QoSTest_Connector_exec_i (const char * topic_name);
    virtual ~CSL_QoSTest_Connector_exec_i (void);
  };

  extern "C" CSL_QOSTEST_CONNECTOR_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_CSL_QoSTest_CSL_QoSTest_Connector_Impl (void);
}

#include /**/ "ace/post.h"

#endif /* ifndef */

