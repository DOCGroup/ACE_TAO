// -*- C++ -*-
// $Id$

#ifndef LISTEN_NOT_ENABLED_TEST_CONNECTOR_EXEC_H_
#define LISTEN_NOT_ENABLED_TEST_CONNECTOR_EXEC_H_

#include /**/ "ace/pre.h"

#include "LNE_Test_ConnectorEC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include /**/ "LNE_Test_Connector_exec_export.h"
#include "tao/LocalObject.h"

#include "dds4ccm/impl/ndds/DDS4CCM_Traits.h"
#include "dds4ccm/impl/ndds/DDS_Event_Connector_T.h"

#include "Base/LNE_Test_BaseSupport.h"

namespace CIAO_LNE_Test_LNE_Test_Connector_Impl
{

  // @from use of ListenNotEnabledTest as a parameter
  typedef CIAO::DDS4CCM::RTI::Type_Traits < 
    ListenNotEnabledTest,
    ListenNotEnabledTestSeq,
    ListenNotEnabledTestTypeSupport,
    ListenNotEnabledTestDataWriter,
    ListenNotEnabledTestDataReader > ListenNotEnabledTest_DDS_Traits;

  typedef CIAO::DDS4CCM::Connector_Traits <
    ::CIAO_LNE_Test_LNE_Test_Connector_Impl::LNE_Test_Connector_Exec,
    ListenNotEnabledTest_Seq,
    ::CCM_DDS::ListenNotEnabledTest::CCM_Writer,
    ::CCM_DDS::ListenNotEnabledTest::CCM_Updater,
    ::CCM_DDS::ListenNotEnabledTest::CCM_Getter,
    ::CCM_DDS::ListenNotEnabledTest::CCM_Reader,
    ::LNE_Test::CCM_LNE_Test_Connector_Context,
    ::CCM_DDS::ListenNotEnabledTest::Listener,
    ::CCM_DDS::ListenNotEnabledTest::StateListener,
    ::CCM_DDS::ConnectorStatusListener> ListenNotEnabledTest_Connector_Traits;

  class LISTEN_NOT_ENABLED_TEST_CONNECTOR_EXEC_Export LNE_Test_Connector_exec_i :
    public DDS_Event_Connector_T <ListenNotEnabledTest_DDS_Traits, ListenNotEnabledTest_Connector_Traits>
  {
  public:
    LNE_Test_Connector_exec_i (void);
    virtual ~LNE_Test_Connector_exec_i (void);
  };

  extern "C" LISTEN_NOT_ENABLED_TEST_CONNECTOR_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_LNE_Test_LNE_Test_Connector_Impl (void);
}

#include /**/ "ace/post.h"

#endif /* ifndef */

