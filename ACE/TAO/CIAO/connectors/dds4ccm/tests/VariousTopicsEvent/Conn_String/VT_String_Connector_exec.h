// -*- C++ -*-
// $Id$

#ifndef VT_STRING_CONNECTOR_EXEC_H_
#define VT_STRING_CONNECTOR_EXEC_H_

#include /**/ "ace/pre.h"

#include "VT_String_ConnectorEC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include /**/ "VT_String_Connector_exec_export.h"
#include "tao/LocalObject.h"

#include "dds4ccm/impl/ndds/DDS4CCM_Traits.h"
#include "dds4ccm/impl/ndds/DDS_Event_Connector_T.h"


#include "Base/VTopic_BaseSupport.h"

namespace CIAO_VT_String_VT_String_Connector_Impl
{

  // @from use of VTopic as a parameter
  typedef CIAO::DDS4CCM::RTI::Type_Traits < 
    TopicString,
    TopicStringSeq,
    TopicStringTypeSupport,
    TopicStringDataWriter,
    TopicStringDataReader > TopicString_DDS_Traits;

  typedef CIAO::DDS4CCM::Connector_Traits <
    ::CIAO_VTopic_VT_String_Connector_Impl::VT_String_Connector_Exec,
    TopicString_Seq,
    ::CCM_DDS::TopicString::CCM_Writer,
    ::CCM_DDS::TopicString::CCM_Updater,
    ::CCM_DDS::TopicString::CCM_Getter,
    ::CCM_DDS::TopicString::CCM_Reader,
    ::VTopic::CCM_VT_String_Connector_Context,
    ::CCM_DDS::TopicString::Listener,
    ::CCM_DDS::TopicString::StateListener,
    ::CCM_DDS::ConnectorStatusListener> TopicString_Connector_Traits;

  class VT_STRING_CONNECTOR_EXEC_Export VT_String_Connector_exec_i :
    public DDS_Event_Connector_T <TopicString_DDS_Traits, TopicString_Connector_Traits>
  {
  public:
    VT_String_Connector_exec_i (void);
    virtual ~VT_String_Connector_exec_i (void);
  };

  extern "C" VT_STRING_CONNECTOR_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_VT_String_VT_String_Connector_Impl (void);
}

#include /**/ "ace/post.h"

#endif /* ifndef */
