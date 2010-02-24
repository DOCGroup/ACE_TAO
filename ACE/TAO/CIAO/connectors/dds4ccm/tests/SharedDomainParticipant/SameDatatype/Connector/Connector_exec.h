// -*- C++ -*-
// $Id$

#ifndef CONNECTOR_EXEC_H_
#define CONNECTOR_EXEC_H_

#include /**/ "ace/pre.h"

#include "ConnectorEC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include /**/ "Connector_exec_export.h"
#include "tao/LocalObject.h"

#include "dds4ccm/impl/ndds/DDS4CCM_Traits.h"
#include "dds4ccm/impl/ndds/DDS_Event_Connector_T.h"

#include "Base/BaseSupport.h"

namespace CIAO_SharedDP_Connector_SDP_Connector_Impl
{

  typedef CIAO::DDS4CCM::RTI::Type_Traits <
    SharedDPTest,
    SharedDPTestSeq,
    SharedDPTestTypeSupport,
    SharedDPTestDataWriter,
    SharedDPTestDataReader > SharedDPTest_DDS_Traits;

  typedef CIAO::DDS4CCM::Connector_Traits <
    ::CIAO_SharedDP_Connector_SDP_Connector_Impl::SDP_Connector_Exec,
    SharedDPTest_Seq,
    ::CCM_DDS::SharedDPTest::CCM_Writer,
    ::CCM_DDS::SharedDPTest::CCM_Updater,
    ::CCM_DDS::SharedDPTest::CCM_Getter,
    ::CCM_DDS::SharedDPTest::CCM_Reader,
    ::SharedDP_Connector::CCM_SDP_Connector_Context,
    ::CCM_DDS::SharedDPTest::Listener,
    ::CCM_DDS::SharedDPTest::StateListener,
    ::CCM_DDS::ConnectorStatusListener> SharedDPTest_Connector_Traits;

  class SHAREDDP_CONNECTOR_EXEC_Export Connector_exec_i :
    public DDS_Event_Connector_T <SharedDPTest_DDS_Traits, SharedDPTest_Connector_Traits>
  {
  public:
    Connector_exec_i (void);
    virtual ~Connector_exec_i (void);
  };

  extern "C" SHAREDDP_CONNECTOR_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_SharedDP_Connector_Impl (void);
}

#include /**/ "ace/post.h"

#endif /* CONNECTOR_EXEC_H_ */

