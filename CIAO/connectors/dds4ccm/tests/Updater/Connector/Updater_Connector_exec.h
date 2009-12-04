// -*- C++ -*-
// $Id$

#ifndef UPDATER_CONNECTOR_EXEC_H_
#define UPDATER_CONNECTOR_EXEC_H_

#include /**/ "ace/pre.h"

#include "Updater_ConnectorEC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include /**/ "Updater_Connector_exec_export.h"
#include "tao/LocalObject.h"

#include "dds4ccm/impl/ndds/DDS4CCM_Traits.h"
#include "dds4ccm/impl/ndds/DDS_State_Connector_T.h"

#include "Base/Updater_BaseSupport.h"

namespace CIAO_Updater_Updater_Connector_Impl
{
  // @from use of UpdaterTest as a parameter
  typedef CIAO::DDS4CCM::RTI::Type_Traits <
    UpdaterTest,
    UpdaterTestSeq,
    UpdaterTestTypeSupport,
    UpdaterTestDataWriter,
    UpdaterTestDataReader > UpdaterTest_DDS_Traits;

  typedef CIAO::DDS4CCM::Connector_Traits <
    ::CIAO_Updater_Updater_Connector_Impl::Updater_Connector_Exec,
    UpdaterTest_Seq,
    ::CCM_DDS::UpdaterTest::CCM_Writer,
    ::CCM_DDS::UpdaterTest::CCM_Updater,
    ::CCM_DDS::UpdaterTest::CCM_Getter,
    ::CCM_DDS::UpdaterTest::CCM_Reader,
    ::Updater::CCM_Updater_Connector_Context,
    ::CCM_DDS::UpdaterTest::Listener,
    ::CCM_DDS::UpdaterTest::StateListener,
    ::CCM_DDS::ConnectorStatusListener> UpdaterTest_Connector_Traits;

  class UPDATER_CONNECTOR_EXEC_Export Updater_Connector_exec_i :
    public DDS_State_Connector_T <UpdaterTest_DDS_Traits, UpdaterTest_Connector_Traits>
  {
  public:
    Updater_Connector_exec_i (void);
    virtual ~Updater_Connector_exec_i (void);
  };

  extern "C" UPDATER_CONNECTOR_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_Updater_Updater_Connector_Impl (void);
}

#include /**/ "ace/post.h"

#endif /* ifndef */

