// -*- C++ -*-
// $Id$

#ifndef LISTEN_TEST_CONNECTOR_EXEC_H_
#define LISTEN_TEST_CONNECTOR_EXEC_H_

#include /**/ "ace/pre.h"

#include "Listen_Test_ConnectorEC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include /**/ "Listen_Test_Connector_exec_export.h"
#include "tao/LocalObject.h"

#include "dds4ccm/impl/ndds/DDS4CCM_Traits.h"
#include "dds4ccm/impl/ndds/DDS_Event_Connector_T.h"

#include "Base/Listen_Test_BaseSupport.h"

namespace CIAO_Listen_Test_Listen_Test_Connector_Impl
{

  // @from use of ListenTest as a parameter
  typedef CIAO::DDS4CCM::RTI::Type_Traits < 
    ListenTest,
    ListenTestSeq,
    ListenTestTypeSupport,
    ListenTestDataWriter,
    ListenTestDataReader > ListenTest_DDS_Traits;

  typedef CIAO::DDS4CCM::Connector_Traits <
    ::CIAO_Listen_Test_Listen_Test_Connector_Impl::Listen_Test_Connector_Exec,
    ListenTest_Seq,
    ::CCM_DDS::ListenTest::CCM_Writer,
    ::CCM_DDS::ListenTest::CCM_Updater,
    ::CCM_DDS::ListenTest::CCM_Getter,
    ::CCM_DDS::ListenTest::CCM_Reader,
    ::Listen_Test::CCM_Listen_Test_Connector_Context,
    ::CCM_DDS::ListenTest::Listener,
    ::CCM_DDS::ListenTest::StateListener,
    ::CCM_DDS::ConnectorStatusListener> ListenTest_Connector_Traits;

  class LISTEN_TEST_CONNECTOR_EXEC_Export Listen_Test_Connector_exec_i :
    public DDS_Event_Connector_T <ListenTest_DDS_Traits, ListenTest_Connector_Traits>
  {
  public:
    Listen_Test_Connector_exec_i (void);
    virtual ~Listen_Test_Connector_exec_i (void);
  };

  extern "C" LISTEN_TEST_CONNECTOR_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_Listen_Test_Listen_Test_Connector_Impl (void);
}

#include /**/ "ace/post.h"

#endif /* ifndef */

