// -*- C++ -*-
// $Id$

#ifndef READER_TEST_CONNECTOR_EXEC_H_
#define READER_TEST_CONNECTOR_EXEC_H_

#include /**/ "ace/pre.h"

#include "Reader_Test_ConnectorEC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include /**/ "Reader_Test_Connector_exec_export.h"
#include "tao/LocalObject.h"

#include "dds4ccm/impl/ndds/DDS4CCM_Traits.h"
#include "dds4ccm/impl/ndds/DDS_Event_Connector_T.h"

#include "Base/Reader_Test_BaseSupport.h"

namespace CIAO_Reader_Test_Reader_Test_Connector_Impl
{

  // @from use of ReaderTest as a parameter
  typedef CIAO::DDS4CCM::RTI::Type_Traits < 
    ReaderTest,
    ReaderTestSeq,
    ReaderTestTypeSupport,
    ReaderTestDataWriter,
    ReaderTestDataReader > ReaderTest_DDS_Traits;

  typedef CIAO::DDS4CCM::Connector_Traits <
    ::CIAO_Reader_Test_Reader_Test_Connector_Impl::Reader_Test_Connector_Exec,
    ReaderTest_Seq,
    ::CCM_DDS::ReaderTest::CCM_Writer,
    ::CCM_DDS::ReaderTest::CCM_Updater,
    ::CCM_DDS::ReaderTest::CCM_Getter,
    ::CCM_DDS::ReaderTest::CCM_Reader,
    ::Reader_Test::CCM_Reader_Test_Connector_Context,
    ::CCM_DDS::ReaderTest::Listener,
    ::CCM_DDS::ReaderTest::StateListener,
    ::CCM_DDS::ConnectorStatusListener> ReaderTest_Connector_Traits;

  class READER_TEST_CONNECTOR_EXEC_Export Reader_Test_Connector_exec_i :
    public DDS_Event_Connector_T <ReaderTest_DDS_Traits, ReaderTest_Connector_Traits>
  {
  public:
    Reader_Test_Connector_exec_i (void);
    virtual ~Reader_Test_Connector_exec_i (void);
  };

  extern "C" READER_TEST_CONNECTOR_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_Reader_Test_Reader_Test_Connector_Impl (void);
}

#include /**/ "ace/post.h"

#endif /* ifndef */

