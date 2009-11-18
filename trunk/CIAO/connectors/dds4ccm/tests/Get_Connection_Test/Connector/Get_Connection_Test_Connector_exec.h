// -*- C++ -*-
// $Id$

#ifndef KEYED_TEST_CONNECTOR_EXEC_H_
#define KEYED_TEST_CONNECTOR_EXEC_H_

#include /**/ "ace/pre.h"

#include "Get_Connection_Test_ConnectorEC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include /**/ "Get_Connection_Test_Connector_exec_export.h"
#include "tao/LocalObject.h"

#include "dds4ccm/impl/ndds/NDDS_Traits.h"
#include "dds4ccm/impl/ndds/Connector_T.h"

#include "Base/Get_Connection_Test_BaseSupport.h"

namespace CIAO_Get_Connection_Test_Get_Connection_Test_Connector_Impl
{

  // @from use of Get_ConnectionTest as a parameter
  typedef CIAO::DDS4CCM::RTI::Type_Traits < 
    Get_ConnectionTest,
    Get_ConnectionTestSeq,
    Get_ConnectionTestTypeSupport,
    Get_ConnectionTestDataWriter,
    Get_ConnectionTestDataReader > Get_ConnectionTest_DDS_Traits;

  typedef Connector_Traits <
    ::CIAO_Get_Connection_Test_Get_Connection_Test_Connector_Impl::Get_Connection_Test_Connector_Exec,
    Get_ConnectionTest_Seq,
    ::CCM_DDS::Get_ConnectionTest::CCM_Writer,
    ::CCM_DDS::Get_ConnectionTest::CCM_Updater,
    ::CCM_DDS::Get_ConnectionTest::CCM_Getter,
    ::CCM_DDS::Get_ConnectionTest::CCM_Reader,
    ::Get_Connection_Test::CCM_Get_Connection_Test_Connector_Context,
    ::CCM_DDS::Get_ConnectionTest::Listener,
    ::CCM_DDS::Get_ConnectionTest::StateListener,
    ::CCM_DDS::ConnectorStatusListener> Get_ConnectionTest_Connector_Traits;

  class KEYED_TEST_CONNECTOR_EXEC_Export Get_Connection_Test_Connector_exec_i :
    public DDS_Event_Connector_T <Get_ConnectionTest_DDS_Traits, Get_ConnectionTest_Connector_Traits>
  {
  public:
    Get_Connection_Test_Connector_exec_i (const char * topic_name);
    virtual ~Get_Connection_Test_Connector_exec_i (void);
  };

  extern "C" KEYED_TEST_CONNECTOR_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_Get_Connection_Test_Get_Connection_Test_Connector_Impl (void);
}

#include /**/ "ace/post.h"

#endif /* ifndef */

