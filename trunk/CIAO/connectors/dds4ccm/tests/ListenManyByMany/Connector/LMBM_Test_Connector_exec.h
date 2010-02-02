// -*- C++ -*-
// $Id$

#ifndef LISTEN_MANY_BY_MANY_TEST_CONNECTOR_EXEC_H_
#define LISTEN_MANY_BY_MANY_TEST_CONNECTOR_EXEC_H_

#include /**/ "ace/pre.h"

#include "LMBM_Test_ConnectorEC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include /**/ "LMBM_Test_Connector_exec_export.h"

#include "dds4ccm/impl/ndds/DDS4CCM_Traits.h"
#include "dds4ccm/impl/ndds/DDS_Event_Connector_T.h"

#include "Base/LMBM_Test_BaseSupport.h"

namespace CIAO_LMBM_Test_LMBM_Test_Connector_Impl
{

  // @from use of ListenManyByManyTest as a parameter
  typedef CIAO::DDS4CCM::RTI::Type_Traits <
    ListenManyByManyTest,
    ListenManyByManyTestSeq,
    ListenManyByManyTestTypeSupport,
    ListenManyByManyTestDataWriter,
    ListenManyByManyTestDataReader > ListenManyByManyTest_DDS_Traits;

  typedef CIAO::DDS4CCM::Connector_Traits <
    ::CIAO_LMBM_Test_LMBM_Test_Connector_Impl::LMBM_Test_Connector_Exec,
    ListenManyByManyTest_Seq,
    ::CCM_DDS::ListenManyByManyTest::CCM_Writer,
    ::CCM_DDS::ListenManyByManyTest::CCM_Updater,
    ::CCM_DDS::ListenManyByManyTest::CCM_Getter,
    ::CCM_DDS::ListenManyByManyTest::CCM_Reader,
    ::LMBM_Test::CCM_LMBM_Test_Connector_Context,
    ::CCM_DDS::ListenManyByManyTest::Listener,
    ::CCM_DDS::ListenManyByManyTest::StateListener,
    ::CCM_DDS::ConnectorStatusListener> ListenManyByManyTest_Connector_Traits;

  class LISTEN_MANY_BY_MANY_TEST_CONNECTOR_EXEC_Export LMBM_Test_Connector_exec_i :
    public DDS_Event_Connector_T <ListenManyByManyTest_DDS_Traits, ListenManyByManyTest_Connector_Traits>
  {
  public:
    LMBM_Test_Connector_exec_i (void);
    virtual ~LMBM_Test_Connector_exec_i (void);
  };

  extern "C" LISTEN_MANY_BY_MANY_TEST_CONNECTOR_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_LMBM_Test_LMBM_Test_Connector_Impl (void);
}

#include /**/ "ace/post.h"

#endif /* ifndef */

