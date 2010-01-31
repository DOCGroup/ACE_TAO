// -*- C++ -*-
// $Id$

#ifndef EVENT_CONNECTION_TEST_CONNECTOR_EXEC_H_
#define EVENT_CONNECTION_TEST_CONNECTOR_EXEC_H_

#include /**/ "ace/pre.h"

#include "Event_Connection_Test_ConnectorEC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include /**/ "Event_Connection_Test_Connector_exec_export.h"
#include "tao/LocalObject.h"

#include "dds4ccm/impl/ndds/DDS4CCM_Traits.h"
#include "dds4ccm/impl/ndds/DDS_Event_Connector_T.h"

#include "Base/Event_Connection_Test_BaseSupport.h"

namespace CIAO_Event_Connection_Test_Event_Connection_Test_Connector_Impl
{

  // @from use of Event_ConnectionTest as a parameter
  typedef CIAO::DDS4CCM::RTI::Type_Traits < 
    Event_ConnectionTest,
    Event_ConnectionTestSeq,
    Event_ConnectionTestTypeSupport,
    Event_ConnectionTestDataWriter,
    Event_ConnectionTestDataReader > Event_ConnectionTest_DDS_Traits;

  typedef CIAO::DDS4CCM::Connector_Traits <
    ::CIAO_Event_Connection_Test_Event_Connection_Test_Connector_Impl::Event_Connection_Test_Connector_Exec,
    Event_ConnectionTest_Seq,
    ::CCM_DDS::Event_ConnectionTest::CCM_Writer,
    ::CCM_DDS::Event_ConnectionTest::CCM_Updater,
    ::CCM_DDS::Event_ConnectionTest::CCM_Getter,
    ::CCM_DDS::Event_ConnectionTest::CCM_Reader,
    ::Event_Connection_Test::CCM_Event_Connection_Test_Connector_Context,
    ::CCM_DDS::Event_ConnectionTest::Listener,
    ::CCM_DDS::Event_ConnectionTest::StateListener,
    ::CCM_DDS::ConnectorStatusListener> Event_ConnectionTest_Connector_Traits;

  class EVENT_CONNECTION_CONNECTOR_EXEC_Export Event_Connection_Test_Connector_exec_i :
    public DDS_Event_Connector_T <Event_ConnectionTest_DDS_Traits, Event_ConnectionTest_Connector_Traits>
  {
  public:
    Event_Connection_Test_Connector_exec_i ();
    virtual ~Event_Connection_Test_Connector_exec_i (void);
  };

  extern "C" EVENT_CONNECTION_CONNECTOR_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_Event_Connection_Test_Event_Connection_Test_Connector_Impl (void);
}

#include /**/ "ace/post.h"

#endif /* ifndef */

