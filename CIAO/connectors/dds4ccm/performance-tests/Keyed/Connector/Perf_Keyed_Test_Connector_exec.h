// -*- C++ -*-
// $Id$

#ifndef PERF_KEYED_TEST_CONNECTOR_EXEC_H_
#define PERF_KEYED_TEST_CONNECTOR_EXEC_H_

#include /**/ "ace/pre.h"

#include "Perf_Keyed_Test_ConnectorEC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include /**/ "Perf_Keyed_Test_Connector_exec_export.h"

#include "dds4ccm/impl/ndds/DDS4CCM_Traits.h"
#include "dds4ccm/impl/ndds/DDS_Event_Connector_T.h"

#include "Base/Perf_Keyed_Test_BaseSupport.h"

namespace CIAO_Perf_Keyed_Test_Perf_Keyed_Test_Connector_Impl
{

  // @from use of PerfKeyedTest as a parameter
  typedef CIAO::DDS4CCM::RTI::Type_Traits <
    PerfKeyedTest,
    PerfKeyedTestSeq,
    PerfKeyedTestTypeSupport,
    PerfKeyedTestDataWriter,
    PerfKeyedTestDataReader > PerfKeyedTest_DDS_Traits;

  typedef CIAO::DDS4CCM::Connector_Traits <
    ::CIAO_Perf_Keyed_Test_Perf_Keyed_Test_Connector_Impl::Perf_Keyed_Test_Connector_Exec,
    PerfKeyedTest_Seq,
    ::CCM_DDS::PerfKeyedTest::CCM_Writer,
    ::CCM_DDS::PerfKeyedTest::CCM_Updater,
    ::CCM_DDS::PerfKeyedTest::CCM_Getter,
    ::CCM_DDS::PerfKeyedTest::CCM_Reader,
    ::Perf_Keyed_Test::CCM_Perf_Keyed_Test_Connector_Context,
    ::CCM_DDS::PerfKeyedTest::Listener,
    ::CCM_DDS::PerfKeyedTest::StateListener,
    ::CCM_DDS::ConnectorStatusListener> PerfKeyedTest_Connector_Traits;

  class PERF_KEYED_TEST_CONNECTOR_EXEC_Export Perf_Keyed_Test_Connector_exec_i :
    public DDS_Event_Connector_T <PerfKeyedTest_DDS_Traits, PerfKeyedTest_Connector_Traits>
  {
  public:
    Perf_Keyed_Test_Connector_exec_i (void);
    virtual ~Perf_Keyed_Test_Connector_exec_i (void);
  };

  extern "C" PERF_KEYED_TEST_CONNECTOR_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_Perf_Keyed_Test_Perf_Keyed_Test_Connector_Impl (void);
}

#include /**/ "ace/post.h"

#endif /* ifndef */

