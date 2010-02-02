// -*- C++ -*-
// $Id$

#ifndef QUERY_FILTER_TEST_CONNECTOR_EXEC_H_
#define QUERY_FILTER_TEST_CONNECTOR_EXEC_H_

#include /**/ "ace/pre.h"

#include "QueryFilter_Test_ConnectorEC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include /**/ "QueryFilter_Test_Connector_exec_export.h"

#include "dds4ccm/impl/ndds/DDS4CCM_Traits.h"
#include "dds4ccm/impl/ndds/DDS_Event_Connector_T.h"

#include "Base/QueryFilter_Test_BaseSupport.h"

namespace CIAO_QueryFilter_Test_QueryFilter_Test_Connector_Impl
{

  // @from use of QueryFilterTest as a parameter
  typedef CIAO::DDS4CCM::RTI::Type_Traits <
    QueryFilterTest,
    QueryFilterTestSeq,
    QueryFilterTestTypeSupport,
    QueryFilterTestDataWriter,
    QueryFilterTestDataReader > QueryFilterTest_DDS_Traits;

  typedef CIAO::DDS4CCM::Connector_Traits <
    ::CIAO_QueryFilter_Test_QueryFilter_Test_Connector_Impl::QueryFilter_Test_Connector_Exec,
    QueryFilterTest_Seq,
    ::CCM_DDS::QueryFilterTest::CCM_Writer,
    ::CCM_DDS::QueryFilterTest::CCM_Updater,
    ::CCM_DDS::QueryFilterTest::CCM_Getter,
    ::CCM_DDS::QueryFilterTest::CCM_Reader,
    ::QueryFilter_Test::CCM_QueryFilter_Test_Connector_Context,
    ::CCM_DDS::QueryFilterTest::Listener,
    ::CCM_DDS::QueryFilterTest::StateListener,
    ::CCM_DDS::ConnectorStatusListener> QueryFilterTest_Connector_Traits;

  class QUERY_FILTER_TEST_CONNECTOR_EXEC_Export QueryFilter_Test_Connector_exec_i :
    public DDS_Event_Connector_T <QueryFilterTest_DDS_Traits, QueryFilterTest_Connector_Traits>
  {
  public:
    QueryFilter_Test_Connector_exec_i (void);
    virtual ~QueryFilter_Test_Connector_exec_i (void);
  };

  extern "C" QUERY_FILTER_TEST_CONNECTOR_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_QueryFilter_Test_QueryFilter_Test_Connector_Impl (void);
}

#include /**/ "ace/post.h"

#endif /* ifndef */

