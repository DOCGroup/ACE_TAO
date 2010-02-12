// -*- C++ -*-
// $Id$

#ifndef HELLO_CONNECTOR_EXEC_H_
#define HELLO_CONNECTOR_EXEC_H_

#include /**/ "ace/pre.h"

#include "Hello_ConnectorEC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include /**/ "Hello_Connector_exec_export.h"

#include "dds4ccm/impl/ndds/DDS4CCM_Traits.h"
#include "dds4ccm/impl/ndds/DDS_Event_Connector_T.h"

#include "connectors/dds4ccm/idl/ccm_ddsC.h"
#include "Base/Hello_BaseC.h"

#include "Base/Hello_BaseSupport.h"

namespace CIAO_Hello_Hello_Connector_Impl
{
  typedef CIAO::DDS4CCM::RTI::Type_Traits <
    ::DDSHello,
    ::DDSHelloSeq,
    ::DDSHelloTypeSupport,
    ::DDSHelloDataWriter,
    ::DDSHelloDataReader > DDSHello_DDS_Traits;

  typedef CIAO::DDS4CCM::Connector_Traits <
    ::CIAO_Hello_Hello_Connector_Impl::Hello_Connector_Exec,
    ::DDSHello_Seq,
    ::Hello::HelloConn::CCM_Writer,
    ::Hello::HelloConn::CCM_Updater,
    ::Hello::HelloConn::CCM_Getter,
    ::Hello::HelloConn::CCM_Reader,
    Hello::CCM_Hello_Connector_Context,
    ::Hello::HelloConn::Listener,
    ::Hello::HelloConn::StateListener,
    ::CCM_DDS::ConnectorStatusListener> DDSHello_Connector_Traits;

  class HELLO_CONNECTOR_EXEC_Export Hello_Connector_exec_i :
    public DDS_Event_Connector_T <DDSHello_DDS_Traits, DDSHello_Connector_Traits>
  {
  public:
    Hello_Connector_exec_i (void);
    virtual ~Hello_Connector_exec_i (void);
  };

  extern "C" HELLO_CONNECTOR_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_Hello_Hello_Connector_Impl (void);
}

#include /**/ "ace/post.h"

#endif /* ifndef */

