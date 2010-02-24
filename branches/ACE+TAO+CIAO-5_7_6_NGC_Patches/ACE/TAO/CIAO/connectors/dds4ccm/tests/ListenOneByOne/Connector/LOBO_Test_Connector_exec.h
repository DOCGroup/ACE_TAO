// -*- C++ -*-
// $Id$

#ifndef LISTEN_ONE_BY_ONE_TEST_CONNECTOR_EXEC_H_
#define LISTEN_ONE_BY_ONE_TEST_CONNECTOR_EXEC_H_

#include /**/ "ace/pre.h"

#include "LOBO_Test_ConnectorEC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include /**/ "LOBO_Test_Connector_exec_export.h"
#include "tao/LocalObject.h"

#include "dds4ccm/impl/ndds/DDS4CCM_Traits.h"
#include "dds4ccm/impl/ndds/DDS_Event_Connector_T.h"

#include "Base/LOBO_Test_BaseSupport.h"

namespace CIAO_LOBO_Test_LOBO_Test_Connector_Impl
{

  // @from use of ListenOneByOneTest as a parameter
  typedef CIAO::DDS4CCM::RTI::Type_Traits < 
    ListenOneByOneTest,
    ListenOneByOneTestSeq,
    ListenOneByOneTestTypeSupport,
    ListenOneByOneTestDataWriter,
    ListenOneByOneTestDataReader > ListenOneByOneTest_DDS_Traits;

  typedef CIAO::DDS4CCM::Connector_Traits <
    ::CIAO_LOBO_Test_LOBO_Test_Connector_Impl::LOBO_Test_Connector_Exec,
    ListenOneByOneTest_Seq,
    ::CCM_DDS::ListenOneByOneTest::CCM_Writer,
    ::CCM_DDS::ListenOneByOneTest::CCM_Updater,
    ::CCM_DDS::ListenOneByOneTest::CCM_Getter,
    ::CCM_DDS::ListenOneByOneTest::CCM_Reader,
    ::LOBO_Test::CCM_LOBO_Test_Connector_Context,
    ::CCM_DDS::ListenOneByOneTest::Listener,
    ::CCM_DDS::ListenOneByOneTest::StateListener,
    ::CCM_DDS::ConnectorStatusListener> ListenOneByOneTest_Connector_Traits;

  class LISTEN_ONE_BY_ONE_TEST_CONNECTOR_EXEC_Export LOBO_Test_Connector_exec_i :
    public DDS_Event_Connector_T <ListenOneByOneTest_DDS_Traits, ListenOneByOneTest_Connector_Traits>
  {
  public:
    LOBO_Test_Connector_exec_i (void);
    virtual ~LOBO_Test_Connector_exec_i (void);
  };

  extern "C" LISTEN_ONE_BY_ONE_TEST_CONNECTOR_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_LOBO_Test_LOBO_Test_Connector_Impl (void);
}

#include /**/ "ace/post.h"

#endif /* ifndef */

