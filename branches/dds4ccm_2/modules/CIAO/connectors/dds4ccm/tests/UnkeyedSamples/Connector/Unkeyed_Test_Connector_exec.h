// -*- C++ -*-
// $Id$

#ifndef KEYED_TEST_CONNECTOR_EXEC_H_
#define KEYED_TEST_CONNECTOR_EXEC_H_

#include /**/ "ace/pre.h"

#include "Unkeyed_Test_ConnectorEC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include /**/ "Unkeyed_Test_Connector_exec_export.h"
#include "tao/LocalObject.h"

#include "dds4ccm/impl/ndds/NDDS_Traits.h"
#include "dds4ccm/impl/ndds/Connector_T.h"

#include "Base/Unkeyed_Test_BaseSupport.h"

namespace CIAO_Unkeyed_Test_Unkeyed_Test_Connector_Impl
{

  // @from use of UnkeyedTest as a parameter
  typedef CIAO::DDS4CCM::RTI::Type_Traits < 
    UnkeyedTest,
    UnkeyedTestSeq,
    UnkeyedTestTypeSupport,
    UnkeyedTestDataWriter,
    UnkeyedTestDataReader > UnkeyedTest_DDS_Traits;

  typedef Connector_Traits <
    ::CIAO_Unkeyed_Test_Unkeyed_Test_Connector_Impl::Unkeyed_Test_Connector_Exec,
    UnkeyedTest_Seq,
    ::CCM_DDS::UnkeyedTest::CCM_Writer,
    ::CCM_DDS::UnkeyedTest::CCM_Updater,
    ::CCM_DDS::UnkeyedTest::CCM_Getter,
    ::CCM_DDS::UnkeyedTest::CCM_Reader,
    ::Unkeyed_Test::CCM_Unkeyed_Test_Connector_Context,
    ::CCM_DDS::UnkeyedTest::RawListener,
    ::CCM_DDS::ConnectorStatusListener> UnkeyedTest_Connector_Traits;

  class KEYED_TEST_CONNECTOR_EXEC_Export Unkeyed_Test_Connector_exec_i :
    public Connector_T <UnkeyedTest_DDS_Traits, UnkeyedTest_Connector_Traits>
  {
  public:
    Unkeyed_Test_Connector_exec_i (const char * topic_name);
    virtual ~Unkeyed_Test_Connector_exec_i (void);
  };

  extern "C" KEYED_TEST_CONNECTOR_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_Unkeyed_Test_Unkeyed_Test_Connector_Impl (void);
}

#include /**/ "ace/post.h"

#endif /* ifndef */

