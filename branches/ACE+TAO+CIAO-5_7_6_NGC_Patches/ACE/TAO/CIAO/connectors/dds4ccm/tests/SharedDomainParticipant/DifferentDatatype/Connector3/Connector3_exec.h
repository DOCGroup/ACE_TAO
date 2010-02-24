// -*- C++ -*-
// $Id$

#ifndef CONNECTOR3_EXEC_H_
#define CONNECTOR3_EXEC_H_

#include /**/ "ace/pre.h"

#include "Connector3EC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include /**/ "Connector3_exec_export.h"
#include "tao/LocalObject.h"

#include "dds4ccm/impl/ndds/DDS4CCM_Traits.h"
#include "dds4ccm/impl/ndds/DDS_Event_Connector_T.h"

#include "Base/BaseSupport.h"

namespace CIAO_SharedDP_Connector3_Connector3_Impl
{

  typedef CIAO::DDS4CCM::RTI::Type_Traits <
    SharedDPTest3,
    SharedDPTest3Seq,
    SharedDPTest3TypeSupport,
    SharedDPTest3DataWriter,
    SharedDPTest3DataReader > SharedDPTest3_DDS_Traits;

  typedef CIAO::DDS4CCM::Connector_Traits <
    ::CIAO_SharedDP_Connector3_Connector3_Impl::Connector3_Exec,
    SharedDPTest3_Seq,
    ::CCM_DDS::SharedDPTest3::CCM_Writer,
    ::CCM_DDS::SharedDPTest3::CCM_Updater,
    ::CCM_DDS::SharedDPTest3::CCM_Getter,
    ::CCM_DDS::SharedDPTest3::CCM_Reader,
    ::SharedDP_Connector3::CCM_Connector3_Context,
    ::CCM_DDS::SharedDPTest3::Listener,
    ::CCM_DDS::SharedDPTest3::StateListener,
    ::CCM_DDS::ConnectorStatusListener> SharedDPTest3_Connector_Traits;

  class SHAREDDP_CONNECTOR3_EXEC_Export Connector3_exec_i :
    public DDS_Event_Connector_T <SharedDPTest3_DDS_Traits, SharedDPTest3_Connector_Traits>
  {
  public:
    Connector3_exec_i (void);
    virtual ~Connector3_exec_i (void);
  };

  extern "C" SHAREDDP_CONNECTOR3_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_SharedDP_Connector3_Impl (void);
}

#include /**/ "ace/post.h"

#endif /* CONNECTOR3_EXEC_H_ */

