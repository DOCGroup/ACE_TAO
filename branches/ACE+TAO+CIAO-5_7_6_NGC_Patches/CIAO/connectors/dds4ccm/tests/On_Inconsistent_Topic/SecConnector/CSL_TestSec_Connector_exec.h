// -*- C++ -*-
// $Id$

#ifndef CONNECTORSTATUSLISTENER_TESTSEC_CONNECTOR_EXEC_H_
#define CONNECTORSTATUSLISTENER_TESTSEC_CONNECTOR_EXEC_H_

#include /**/ "ace/pre.h"

#include "ConnectorStatusListener_TestSec_ConnectorEC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include /**/ "ConnectorStatusListener_TestSec_Connector_exec_export.h"
#include "tao/LocalObject.h"

#include "dds4ccm/impl/ndds/DDS4CCM_Traits.h"
#include "dds4ccm/impl/ndds/DDS_Event_Connector_T.h"

#include "Base/ConnectorStatusListener_Test_BaseSupport.h"

namespace CIAO_ConnectorStatusListener_TestSec_ConnectorStatusListener_TestSec_Connector_Impl
{

  // @from use of ConnectorStatusListener_Test as a parameter
  typedef CIAO::DDS4CCM::RTI::Type_Traits < 
    TestSecondTopic,
    TestSecondTopicSeq,
    TestSecondTopicTypeSupport,
    TestSecondTopicDataWriter,
    TestSecondTopicDataReader > TestSecondTopic_DDS_Traits;

  typedef CIAO::DDS4CCM::Connector_Traits <
    ::CIAO_ConnectorStatusListener_Test_ConnectorStatusListener_TestSec_Connector_Impl::ConnectorStatusListener_TestSec_Connector_Exec,
    TestSecondTopic_Seq,
    ::CCM_DDS::TestSecondTopic::CCM_Writer,
    ::CCM_DDS::TestSecondTopic::CCM_Updater,
    ::CCM_DDS::TestSecondTopic::CCM_Getter,
    ::CCM_DDS::TestSecondTopic::CCM_Reader,
    ::ConnectorStatusListener_Test::CCM_ConnectorStatusListener_TestSec_Connector_Context,
    ::CCM_DDS::TestSecondTopic::Listener,
    ::CCM_DDS::TestSecondTopic::StateListener,
    ::CCM_DDS::ConnectorStatusListener> TestSecondTopic_Connector_Traits;

  class CONNECTORSTATUSLISTENER_TESTSEC_CONNECTOR_EXEC_Export ConnectorStatusListener_TestSec_Connector_exec_i :
    public DDS_Event_Connector_T <TestSecondTopic_DDS_Traits, TestSecondTopic_Connector_Traits>
  {
  public:
    ConnectorStatusListener_TestSec_Connector_exec_i (void);
    virtual ~ConnectorStatusListener_TestSec_Connector_exec_i (void);
  };

  extern "C" CONNECTORSTATUSLISTENER_TESTSEC_CONNECTOR_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_ConnectorStatusListener_TestSec_ConnectorStatusListener_TestSec_Connector_Impl (void);
}

#include /**/ "ace/post.h"

#endif /* ifndef */

