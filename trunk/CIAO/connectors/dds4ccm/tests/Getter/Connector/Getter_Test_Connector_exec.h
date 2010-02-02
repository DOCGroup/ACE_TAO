// -*- C++ -*-
// $Id$

#ifndef GETTER_TEST_CONNECTOR_EXEC_H_
#define GETTER_TEST_CONNECTOR_EXEC_H_

#include /**/ "ace/pre.h"

#include "Getter_Test_ConnectorEC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include /**/ "Getter_Test_Connector_exec_export.h"

#include "dds4ccm/impl/ndds/DDS4CCM_Traits.h"
#include "dds4ccm/impl/ndds/DDS_Event_Connector_T.h"

#include "Base/Getter_Test_BaseSupport.h"

namespace CIAO_Getter_Test_Getter_Test_Connector_Impl
{

  // @from use of GetterTest as a parameter
  typedef CIAO::DDS4CCM::RTI::Type_Traits <
    GetterTest,
    GetterTestSeq,
    GetterTestTypeSupport,
    GetterTestDataWriter,
    GetterTestDataReader > GetterTest_DDS_Traits;

  typedef CIAO::DDS4CCM::Connector_Traits <
    ::CIAO_Getter_Test_Getter_Test_Connector_Impl::Getter_Test_Connector_Exec,
    GetterTest_Seq,
    ::CCM_DDS::GetterTest::CCM_Writer,
    ::CCM_DDS::GetterTest::CCM_Updater,
    ::CCM_DDS::GetterTest::CCM_Getter,
    ::CCM_DDS::GetterTest::CCM_Reader,
    ::Getter_Test::CCM_Getter_Test_Connector_Context,
    ::CCM_DDS::GetterTest::Listener,
    ::CCM_DDS::GetterTest::StateListener,
    ::CCM_DDS::ConnectorStatusListener> GetterTest_Connector_Traits;

  class GETTER_TEST_CONNECTOR_EXEC_Export Getter_Test_Connector_exec_i :
    public DDS_Event_Connector_T <GetterTest_DDS_Traits, GetterTest_Connector_Traits>
  {
  public:
    Getter_Test_Connector_exec_i (void);
    virtual ~Getter_Test_Connector_exec_i (void);
  };

  extern "C" GETTER_TEST_CONNECTOR_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_Getter_Test_Getter_Test_Connector_Impl (void);
}

#include /**/ "ace/post.h"

#endif /* ifndef */

