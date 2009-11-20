// -*- C++ -*-
// $Id$

#ifndef WRITER_CONNECTOR_EXEC_H_
#define WRITER_CONNECTOR_EXEC_H_

#include /**/ "ace/pre.h"

#include "Writer_ConnectorEC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include /**/ "Writer_Connector_exec_export.h"
#include "tao/LocalObject.h"

#include "dds4ccm/impl/ndds/DDS4CCM_Traits.h"
#include "dds4ccm/impl/ndds/DDS_Event_Connector_T.h"

#include "Base/Writer_BaseSupport.h"

namespace CIAO_Writer_Writer_Connector_Impl
{
  // @from use of WriterTest as a parameter
  typedef CIAO::DDS4CCM::RTI::Type_Traits < 
    WriterTest,
    WriterTestSeq,
    WriterTestTypeSupport,
    WriterTestDataWriter,
    WriterTestDataReader > WriterTest_DDS_Traits;

  typedef CIAO::DDS4CCM::Connector_Traits <
    ::CIAO_Writer_Writer_Connector_Impl::Writer_Connector_Exec,
    WriterTest_Seq,
    ::CCM_DDS::WriterTest::CCM_Writer,
    ::CCM_DDS::WriterTest::CCM_Updater,
    ::CCM_DDS::WriterTest::CCM_Getter,
    ::CCM_DDS::WriterTest::CCM_Reader,
    ::Writer::CCM_Writer_Connector_Context,
    ::CCM_DDS::WriterTest::Listener,
    ::CCM_DDS::WriterTest::StateListener,
    ::CCM_DDS::ConnectorStatusListener> WriterTest_Connector_Traits;

  class WRITER_CONNECTOR_EXEC_Export Writer_Connector_exec_i :
    public DDS_Event_Connector_T <WriterTest_DDS_Traits, WriterTest_Connector_Traits>
  {
  public:
    Writer_Connector_exec_i (void);
    virtual ~Writer_Connector_exec_i (void);
  };

  extern "C" WRITER_CONNECTOR_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_Writer_Writer_Connector_Impl (void);
}

#include /**/ "ace/post.h"

#endif /* ifndef */

