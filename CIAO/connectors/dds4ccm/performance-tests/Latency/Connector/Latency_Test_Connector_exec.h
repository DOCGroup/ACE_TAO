// -*- C++ -*-
// $Id$

#ifndef LATENCY_TEST_CONNECTOR_EXEC_H_
#define LATENCY_TEST_CONNECTOR_EXEC_H_

#include /**/ "ace/pre.h"

#include "Latency_Test_ConnectorEC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include /**/ "Latency_Test_Connector_exec_export.h"

#include "dds4ccm/impl/ndds/DDS4CCM_Traits.h"
#include "dds4ccm/impl/ndds/DDS_Event_Connector_T.h"

#include "Base/Latency_Test_BaseSupport.h"

namespace CIAO_Latency_Test_Latency_Test_Connector_Impl
{

  // @from use of LatencyTest as a parameter
  typedef CIAO::DDS4CCM::RTI::Type_Traits <
    LatencyTest,
    LatencyTestSeq,
    LatencyTestTypeSupport,
    LatencyTestDataWriter,
    LatencyTestDataReader > LatencyTest_DDS_Traits;

  typedef CIAO::DDS4CCM::Connector_Traits <
    ::CIAO_Latency_Test_Latency_Test_Connector_Impl::Latency_Test_Connector_Exec,
    LatencyTest_Seq,
    ::CCM_DDS::LatencyTest::CCM_Writer,
    ::CCM_DDS::LatencyTest::CCM_Updater,
    ::CCM_DDS::LatencyTest::CCM_Getter,
    ::CCM_DDS::LatencyTest::CCM_Reader,
    ::Latency_Test::CCM_Latency_Test_Connector_Context,
    ::CCM_DDS::LatencyTest::Listener,
    ::CCM_DDS::LatencyTest::StateListener,
    ::CCM_DDS::ConnectorStatusListener> LatencyTest_Connector_Traits;

  class LATENCY_TEST_CONNECTOR_EXEC_Export Latency_Test_Connector_exec_i :
    public DDS_Event_Connector_T <LatencyTest_DDS_Traits, LatencyTest_Connector_Traits>
  {
  public:
    Latency_Test_Connector_exec_i (void);
    virtual ~Latency_Test_Connector_exec_i (void);
  };

  extern "C" LATENCY_TEST_CONNECTOR_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_Latency_Test_Latency_Test_Connector_Impl (void);
}

#include /**/ "ace/post.h"

#endif /* ifndef */

