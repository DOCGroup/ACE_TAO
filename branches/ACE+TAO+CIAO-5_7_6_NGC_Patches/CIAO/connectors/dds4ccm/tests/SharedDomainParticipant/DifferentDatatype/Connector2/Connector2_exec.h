// -*- C++ -*-
// $Id$

#ifndef CONNECTOR2_EXEC_H_
#define CONNECTOR2_EXEC_H_

#include /**/ "ace/pre.h"

#include "Connector2EC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include /**/ "Connector2_exec_export.h"
#include "tao/LocalObject.h"

#include "dds4ccm/impl/ndds/DDS4CCM_Traits.h"
#include "dds4ccm/impl/ndds/DDS_Event_Connector_T.h"

#include "Base/BaseSupport.h"

namespace CIAO_SharedDP_Connector2_Connector2_Impl
{

  typedef CIAO::DDS4CCM::RTI::Type_Traits <
    SharedDPTest2,
    SharedDPTest2Seq,
    SharedDPTest2TypeSupport,
    SharedDPTest2DataWriter,
    SharedDPTest2DataReader > SharedDPTest2_DDS_Traits;

  typedef CIAO::DDS4CCM::Connector_Traits <
    ::CIAO_SharedDP_Connector2_Connector2_Impl::Connector2_Exec,
    SharedDPTest2_Seq,
    ::CCM_DDS::SharedDPTest2::CCM_Writer,
    ::CCM_DDS::SharedDPTest2::CCM_Updater,
    ::CCM_DDS::SharedDPTest2::CCM_Getter,
    ::CCM_DDS::SharedDPTest2::CCM_Reader,
    ::SharedDP_Connector2::CCM_Connector2_Context,
    ::CCM_DDS::SharedDPTest2::Listener,
    ::CCM_DDS::SharedDPTest2::StateListener,
    ::CCM_DDS::ConnectorStatusListener> SharedDPTest2_Connector_Traits;

  class SHAREDDP_CONNECTOR2_EXEC_Export Connector2_exec_i :
    public DDS_Event_Connector_T <SharedDPTest2_DDS_Traits, SharedDPTest2_Connector_Traits>
  {
  public:
    Connector2_exec_i (void);
    virtual ~Connector2_exec_i (void);
  };

  extern "C" SHAREDDP_CONNECTOR2_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_SharedDP_Connector2_Impl (void);
}

#include /**/ "ace/post.h"

#endif /* CONNECTOR2_EXEC_H_ */

