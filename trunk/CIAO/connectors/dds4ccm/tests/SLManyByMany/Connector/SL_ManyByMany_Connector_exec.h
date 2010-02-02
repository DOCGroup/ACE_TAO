// -*- C++ -*-
// $Id$

#ifndef SL_MANYBYMANY_CONNECTOR_EXEC_H_
#define SL_MANYBYMANY_CONNECTOR_EXEC_H_

#include /**/ "ace/pre.h"

#include "SL_ManyByMany_ConnectorEC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include /**/ "SL_ManyByMany_Connector_exec_export.h"

#include "dds4ccm/impl/ndds/DDS4CCM_Traits.h"
#include "dds4ccm/impl/ndds/DDS_State_Connector_T.h"


#include "Base/SL_ManyByMany_BaseSupport.h"

namespace CIAO_SL_ManyByMany_SL_ManyByMany_Connector_Impl
{

  // @from use of SL_ManyByMany as a parameter
  typedef CIAO::DDS4CCM::RTI::Type_Traits <
    TestTopic,
    TestTopicSeq,
    TestTopicTypeSupport,
    TestTopicDataWriter,
    TestTopicDataReader > TestTopic_DDS_Traits;

  typedef CIAO::DDS4CCM::Connector_Traits <
    ::CIAO_SL_ManyByMany_SL_ManyByMany_Connector_Impl::SL_ManyByMany_Connector_Exec,
    TestTopic_Seq,
    ::CCM_DDS::TestTopic::CCM_Writer,
    ::CCM_DDS::TestTopic::CCM_Updater,
    ::CCM_DDS::TestTopic::CCM_Getter,
    ::CCM_DDS::TestTopic::CCM_Reader,
    ::SL_ManyByMany::CCM_SL_ManyByMany_Connector_Context,
    ::CCM_DDS::TestTopic::Listener,
    ::CCM_DDS::TestTopic::StateListener,
    ::CCM_DDS::ConnectorStatusListener> TestTopic_Connector_Traits;

  class SL_MANYBYMANY_CONNECTOR_EXEC_Export SL_ManyByMany_Connector_exec_i :
    public DDS_State_Connector_T <TestTopic_DDS_Traits, TestTopic_Connector_Traits>
  {
  public:
    SL_ManyByMany_Connector_exec_i (void);
    virtual ~SL_ManyByMany_Connector_exec_i (void);
  };

  extern "C" SL_MANYBYMANY_CONNECTOR_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_SL_ManyByMany_SL_ManyByMany_Connector_Impl (void);
}

#include /**/ "ace/post.h"

#endif /* ifndef */

