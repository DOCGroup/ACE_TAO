// -*- C++ -*-
// $Id$

#ifndef STATE_CONNECTION_TEST_CONNECTOR_EXEC_H_
#define STATE_CONNECTION_TEST_CONNECTOR_EXEC_H_

#include /**/ "ace/pre.h"

#include "State_Connection_Test_ConnectorEC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include /**/ "State_Connection_Test_Connector_exec_export.h"
#include "tao/LocalObject.h"

#include "dds4ccm/impl/ndds/DDS4CCM_Traits.h"
#include "dds4ccm/impl/ndds/DDS_State_Connector_T.h"

#include "Base/State_Connection_Test_BaseSupport.h"

namespace CIAO_State_Connection_Test_State_Connection_Test_Connector_Impl
{

  // @from use of State_ConnectionTest as a parameter
  typedef CIAO::DDS4CCM::RTI::Type_Traits < 
    State_ConnectionTest,
    State_ConnectionTestSeq,
    State_ConnectionTestTypeSupport,
    State_ConnectionTestDataWriter,
    State_ConnectionTestDataReader > State_ConnectionTest_DDS_Traits;

  typedef CIAO::DDS4CCM::Connector_Traits <
    ::CIAO_State_Connection_Test_State_Connection_Test_Connector_Impl::State_Connection_Test_Connector_Exec,
    State_ConnectionTest_Seq,
    ::CCM_DDS::State_ConnectionTest::CCM_Writer,
    ::CCM_DDS::State_ConnectionTest::CCM_Updater,
    ::CCM_DDS::State_ConnectionTest::CCM_Getter,
    ::CCM_DDS::State_ConnectionTest::CCM_Reader,
    ::State_Connection_Test::CCM_State_Connection_Test_Connector_Context,
    ::CCM_DDS::State_ConnectionTest::Listener,
    ::CCM_DDS::State_ConnectionTest::StateListener,
    ::CCM_DDS::ConnectorStatusListener> State_ConnectionTest_Connector_Traits;

  class STATE_CONNECTION_CONNECTOR_EXEC_Export State_Connection_Test_Connector_exec_i :
    public DDS_State_Connector_T <State_ConnectionTest_DDS_Traits, State_ConnectionTest_Connector_Traits>
  {
  public:
    State_Connection_Test_Connector_exec_i ();
    virtual ~State_Connection_Test_Connector_exec_i (void);
  };

  extern "C" STATE_CONNECTION_CONNECTOR_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_State_Connection_Test_State_Connection_Test_Connector_Impl (void);
}

#include /**/ "ace/post.h"

#endif /* ifndef */

