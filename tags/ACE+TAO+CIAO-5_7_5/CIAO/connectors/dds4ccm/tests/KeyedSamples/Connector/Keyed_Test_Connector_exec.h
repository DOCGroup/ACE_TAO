// -*- C++ -*-
// $Id$

#ifndef KEYED_TEST_CONNECTOR_EXEC_H_
#define KEYED_TEST_CONNECTOR_EXEC_H_

#include /**/ "ace/pre.h"

#include "Keyed_Test_ConnectorEC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include /**/ "Keyed_Test_Connector_exec_export.h"
#include "tao/LocalObject.h"

#include "dds4ccm/impl/ndds/DDS4CCM_Traits.h"
#include "dds4ccm/impl/ndds/DDS_Event_Connector_T.h"

#include "Base/Keyed_Test_BaseSupport.h"

namespace CIAO_Keyed_Test_Keyed_Test_Connector_Impl
{

  // @from use of KeyedTest as a parameter
  typedef CIAO::DDS4CCM::RTI::Type_Traits < 
    KeyedTest,
    KeyedTestSeq,
    KeyedTestTypeSupport,
    KeyedTestDataWriter,
    KeyedTestDataReader > KeyedTest_DDS_Traits;

  typedef CIAO::DDS4CCM::Connector_Traits <
    ::CIAO_Keyed_Test_Keyed_Test_Connector_Impl::Keyed_Test_Connector_Exec,
    KeyedTest_Seq,
    ::CCM_DDS::KeyedTest::CCM_Writer,
    ::CCM_DDS::KeyedTest::CCM_Updater,
    ::CCM_DDS::KeyedTest::CCM_Getter,
    ::CCM_DDS::KeyedTest::CCM_Reader,
    ::Keyed_Test::CCM_Keyed_Test_Connector_Context,
    ::CCM_DDS::KeyedTest::Listener,
    ::CCM_DDS::KeyedTest::StateListener,
    ::CCM_DDS::ConnectorStatusListener> KeyedTest_Connector_Traits;

  class KEYED_TEST_CONNECTOR_EXEC_Export Keyed_Test_Connector_exec_i :
    public DDS_Event_Connector_T <KeyedTest_DDS_Traits, KeyedTest_Connector_Traits>
  {
  public:
    Keyed_Test_Connector_exec_i (void);
    virtual ~Keyed_Test_Connector_exec_i (void);
  };

  extern "C" KEYED_TEST_CONNECTOR_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_Keyed_Test_Keyed_Test_Connector_Impl (void);
}

#include /**/ "ace/post.h"

#endif /* ifndef */

