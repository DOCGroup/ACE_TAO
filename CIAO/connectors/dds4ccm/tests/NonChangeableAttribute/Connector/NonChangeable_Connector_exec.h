// -*- C++ -*-
// $Id$

#ifndef NONCHANGEABLE_CONNECTOR_EXEC_H_
#define NONCHANGEABLE_CONNECTOR_EXEC_H_

#include /**/ "ace/pre.h"

#include "NonChangeable_ConnectorEC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include /**/ "NonChangeable_Connector_exec_export.h"

#include "dds4ccm/impl/ndds/DDS4CCM_Traits.h"
#include "dds4ccm/impl/ndds/DDS_Event_Connector_T.h"

#include "Base/NonChangeable_BaseSupport.h"

namespace CIAO_NonChangeable_NonChangeable_Connector_Impl
{
  // @from use of NonChangeableTest as a parameter
  typedef CIAO::DDS4CCM::RTI::Type_Traits <
    NonChangeableTest,
    NonChangeableTestSeq,
    NonChangeableTestTypeSupport,
    NonChangeableTestDataWriter,
    NonChangeableTestDataReader > NonChangeableTest_DDS_Traits;

  typedef CIAO::DDS4CCM::Connector_Traits <
    ::CIAO_NonChangeable_NonChangeable_Connector_Impl::NonChangeable_Connector_Exec,
    NonChangeableTest_Seq,
    ::CCM_DDS::NonChangeableTest::CCM_Writer,
    ::CCM_DDS::NonChangeableTest::CCM_Updater,
    ::CCM_DDS::NonChangeableTest::CCM_Getter,
    ::CCM_DDS::NonChangeableTest::CCM_Reader,
    ::NonChangeable::CCM_NonChangeable_Connector_Context,
    ::CCM_DDS::NonChangeableTest::Listener,
    ::CCM_DDS::NonChangeableTest::StateListener,
    ::CCM_DDS::ConnectorStatusListener> NonChangeableTest_Connector_Traits;

  class NONCHANGEABLE_CONNECTOR_EXEC_Export NonChangeable_Connector_exec_i :
    public DDS_Event_Connector_T <NonChangeableTest_DDS_Traits, NonChangeableTest_Connector_Traits>
  {
  public:
    NonChangeable_Connector_exec_i (void);
    virtual ~NonChangeable_Connector_exec_i (void);

    void ccm_activate (void);
  private:
    void test_domain_id ();
    void test_qos_profile ();
    void test_topic_name ();
    void test_key_fields ();
  };

  extern "C" NONCHANGEABLE_CONNECTOR_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_NonChangeable_NonChangeable_Connector_Impl (void);
}

#include /**/ "ace/post.h"

#endif /* ifndef */

