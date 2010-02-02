// -*- C++ -*-
// $Id$

#ifndef CSL_SRTEST_CONNECTOR_EXEC_H_
#define CSL_SRTEST_CONNECTOR_EXEC_H_

#include /**/ "ace/pre.h"

#include "CSL_SRTest_ConnectorEC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include /**/ "CSL_SRTest_Connector_exec_export.h"

#include "dds4ccm/impl/ndds/DDS4CCM_Traits.h"
#include "dds4ccm/impl/ndds/DDS_Event_Connector_T.h"


#include "Base/CSL_SRTest_BaseSupport.h"

namespace CIAO_CSL_SRTest_CSL_SRTest_Connector_Impl
{

  // @from use of CSL_SRTest as a parameter
  typedef CIAO::DDS4CCM::RTI::Type_Traits <
    TestTopic,
    TestTopicSeq,
    TestTopicTypeSupport,
    TestTopicDataWriter,
    TestTopicDataReader > TestTopic_DDS_Traits;

  typedef CIAO::DDS4CCM::Connector_Traits <
    ::CIAO_CSL_SRTest_CSL_SRTest_Connector_Impl::CSL_SRTest_Connector_Exec,
    TestTopic_Seq,
    ::CCM_DDS::TestTopic::CCM_Writer,
    ::CCM_DDS::TestTopic::CCM_Updater,
    ::CCM_DDS::TestTopic::CCM_Getter,
    ::CCM_DDS::TestTopic::CCM_Reader,
    ::CSL_SRTest::CCM_CSL_SRTest_Connector_Context,
    ::CCM_DDS::TestTopic::Listener,
    ::CCM_DDS::TestTopic::StateListener,
    ::CCM_DDS::ConnectorStatusListener> TestTopic_Connector_Traits;

  class CSL_SRTEST_CONNECTOR_EXEC_Export CSL_SRTest_Connector_exec_i :
    public DDS_Event_Connector_T <TestTopic_DDS_Traits, TestTopic_Connector_Traits>
  {
  public:
    CSL_SRTest_Connector_exec_i (void);
    virtual ~CSL_SRTest_Connector_exec_i (void);
  };

  extern "C" CSL_SRTEST_CONNECTOR_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_CSL_SRTest_CSL_SRTest_Connector_Impl (void);
}

#include /**/ "ace/post.h"

#endif /* ifndef */

