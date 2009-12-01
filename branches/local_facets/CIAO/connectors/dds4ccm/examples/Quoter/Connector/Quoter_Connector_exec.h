// -*- C++ -*-
// $Id$

#ifndef CIAO_QUOTER_CONNECTOR_EXEC_H_
#define CIAO_QUOTER_CONNECTOR_EXEC_H_

#include /**/ "ace/pre.h"

#include "Quoter_ConnectorEC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include /**/ "Quoter_Connector_exec_export.h"
#include "tao/LocalObject.h"

#include "dds4ccm/impl/ndds/DDS4CCM_Traits.h"
#include "dds4ccm/impl/ndds/DDS_Event_Connector_T.h"

#include "Base/Quoter_BaseSupport.h"

namespace CIAO_Quoter_Quoter_Connector_Impl
{

  // @from use of Stock_Info as a parameter
  typedef CIAO::DDS4CCM::RTI::Type_Traits < 
    ::Quoter::Stock_Info,
    ::Quoter::Stock_InfoSeq,
    ::Quoter::Stock_InfoTypeSupport,
    ::Quoter::Stock_InfoDataWriter,
    ::Quoter::Stock_InfoDataReader > Stock_Info_DDS_Traits;

  typedef CIAO::DDS4CCM::Connector_Traits <
    ::CIAO_Quoter_Quoter_Connector_Impl::Quoter_Connector_Exec,
    ::Quoter::Stock_Info_Seq,
    ::CCM_DDS::Quoter::CCM_Writer,
    ::CCM_DDS::Quoter::CCM_Updater,
    ::CCM_DDS::Quoter::CCM_Getter,
    ::CCM_DDS::Quoter::CCM_Reader,
    ::Quoter::CCM_Quoter_Connector_Context,
    ::CCM_DDS::Quoter::Listener,
    ::CCM_DDS::Quoter::StateListener,
    ::CCM_DDS::ConnectorStatusListener> Stock_Info_Connector_Traits;

  class QUOTER_CONNECTOR_EXEC_Export Quoter_Connector_exec_i :
    public DDS_Event_Connector_T <Stock_Info_DDS_Traits, Stock_Info_Connector_Traits>
  {
  public:
    Quoter_Connector_exec_i (void);
    virtual ~Quoter_Connector_exec_i (void);
  };

  extern "C" QUOTER_CONNECTOR_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_Quoter_Quoter_Connector_Impl (void);
}

#include /**/ "ace/post.h"

#endif /* ifndef */

