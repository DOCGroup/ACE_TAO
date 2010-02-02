// -*- C++ -*-
// $Id$

#ifndef CONNECTORSTATUSLISTENER_TEST_CONNECTOR_EXEC_H_
#define CONNECTORSTATUSLISTENER_TEST_CONNECTOR_EXEC_H_

#include /**/ "ace/pre.h"

#include "ConnectorStatusListener_Test_ConnectorEC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include /**/ "ConnectorStatusListener_Test_Connector_exec_export.h"

#include "dds4ccm/impl/ndds/DDS4CCM_Traits.h"
#include "dds4ccm/impl/ndds/DDS_Event_Connector_T.h"

#include "Base/ConnectorStatusListener_Test_BaseSupport.h"

namespace CIAO_ConnectorStatusListener_Test_ConnectorStatusListener_Test_Connector_Impl
{

  // @from use of ConnectorStatusListener_Test as a parameter
  typedef CIAO::DDS4CCM::RTI::Type_Traits <
    TestTopic,
    TestTopicSeq,
    TestTopicTypeSupport,
    TestTopicDataWriter,
    TestTopicDataReader > TestTopic_DDS_Traits;

  typedef CIAO::DDS4CCM::Connector_Traits <
    ::CIAO_ConnectorStatusListener_Test_ConnectorStatusListener_Test_Connector_Impl::ConnectorStatusListener_Test_Connector_Exec,
    TestTopic_Seq,
    ::CCM_DDS::TestTopic::CCM_Writer,
    ::CCM_DDS::TestTopic::CCM_Updater,
    ::CCM_DDS::TestTopic::CCM_Getter,
    ::CCM_DDS::TestTopic::CCM_Reader,
    ::ConnectorStatusListener_Test::CCM_ConnectorStatusListener_Test_Connector_Context,
    ::CCM_DDS::TestTopic::Listener,
    ::CCM_DDS::TestTopic::StateListener,
    ::CCM_DDS::ConnectorStatusListener> TestTopic_Connector_Traits;

  class CONNECTORSTATUSLISTENER_TEST_CONNECTOR_EXEC_Export ConnectorStatusListener_Test_Connector_exec_i :
    public DDS_Event_Connector_T <TestTopic_DDS_Traits, TestTopic_Connector_Traits>
  {
  public:
    ConnectorStatusListener_Test_Connector_exec_i (void);
    virtual ~ConnectorStatusListener_Test_Connector_exec_i (void);
  };

  extern "C" CONNECTORSTATUSLISTENER_TEST_CONNECTOR_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_ConnectorStatusListener_Test_ConnectorStatusListener_Test_Connector_Impl (void);
}

#include /**/ "ace/post.h"

#endif /* ifndef */

