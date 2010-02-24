// -*- C++ -*-
// $Id$

#ifndef SETCONNECTORATTRIBUTE_CONNECTOR_EXEC_H_
#define SETCONNECTORATTRIBUTE_CONNECTOR_EXEC_H_

#include /**/ "ace/pre.h"

#include "SetConnectorAttribute_ConnectorEC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include /**/ "SetConnectorAttribute_Connector_exec_export.h"
#include "tao/LocalObject.h"

#include "dds4ccm/impl/ndds/DDS4CCM_Traits.h"
#include "dds4ccm/impl/ndds/DDS_Event_Connector_T.h"

#include "Base/SetConnectorAttribute_BaseSupport.h"

namespace CIAO_SetConnectorAttribute_SetConnectorAttribute_Connector_Impl
{
  // @from use of SetConnectorAttributeTest as a parameter
  typedef CIAO::DDS4CCM::RTI::Type_Traits < 
    SetConnectorAttributeTest,
    SetConnectorAttributeTestSeq,
    SetConnectorAttributeTestTypeSupport,
    SetConnectorAttributeTestDataWriter,
    SetConnectorAttributeTestDataReader > SetConnectorAttributeTest_DDS_Traits;

  typedef CIAO::DDS4CCM::Connector_Traits <
    ::CIAO_SetConnectorAttribute_SetConnectorAttribute_Connector_Impl::SetConnectorAttribute_Connector_Exec,
    SetConnectorAttributeTest_Seq,
    ::CCM_DDS::SetConnectorAttributeTest::CCM_Writer,
    ::CCM_DDS::SetConnectorAttributeTest::CCM_Updater,
    ::CCM_DDS::SetConnectorAttributeTest::CCM_Getter,
    ::CCM_DDS::SetConnectorAttributeTest::CCM_Reader,
    ::SetConnectorAttribute::CCM_SetConnectorAttribute_Connector_Context,
    ::CCM_DDS::SetConnectorAttributeTest::Listener,
    ::CCM_DDS::SetConnectorAttributeTest::StateListener,
    ::CCM_DDS::ConnectorStatusListener> SetConnectorAttributeTest_Connector_Traits;

  class SETCONNECTORATTRIBUTE_CONNECTOR_EXEC_Export SetConnectorAttribute_Connector_exec_i :
    public DDS_Event_Connector_T <SetConnectorAttributeTest_DDS_Traits, SetConnectorAttributeTest_Connector_Traits>
  {
  public:
    SetConnectorAttribute_Connector_exec_i (void);
    virtual ~SetConnectorAttribute_Connector_exec_i (void);

    void ccm_activate (void);
  private:
    void test_domain_id ();
    void test_qos_profile ();
    void test_topic_name ();
    void test_key_fields ();
  };

  extern "C" SETCONNECTORATTRIBUTE_CONNECTOR_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_SetConnectorAttribute_SetConnectorAttribute_Connector_Impl (void);
}

#include /**/ "ace/post.h"

#endif /* ifndef */

