// -*- C++ -*-
// $Id$

#ifndef VT_ARRAY_CONNECTOR_EXEC_H_
#define VT_ARRAY_CONNECTOR_EXEC_H_

#include /**/ "ace/pre.h"

#include "VT_Array_ConnectorEC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include /**/ "VT_Array_Connector_exec_export.h"

#include "dds4ccm/impl/ndds/DDS4CCM_Traits.h"
#include "dds4ccm/impl/ndds/DDS_Event_Connector_T.h"


#include "Base/VTopic_BaseSupport.h"

namespace CIAO_VT_Array_VT_Array_Connector_Impl
{

  // @from use of VTopic as a parameter
  typedef CIAO::DDS4CCM::RTI::Type_Traits <
    TopicArray,
    TopicArraySeq,
    TopicArrayTypeSupport,
    TopicArrayDataWriter,
    TopicArrayDataReader > TopicArray_DDS_Traits;

  typedef CIAO::DDS4CCM::Connector_Traits <
    ::CIAO_VTopic_VT_Array_Connector_Impl::VT_Array_Connector_Exec,
    TopicArray_Seq,
    ::CCM_DDS::TopicArray::CCM_Writer,
    ::CCM_DDS::TopicArray::CCM_Updater,
    ::CCM_DDS::TopicArray::CCM_Getter,
    ::CCM_DDS::TopicArray::CCM_Reader,
    ::VTopic::CCM_VT_Array_Connector_Context,
    ::CCM_DDS::TopicArray::Listener,
    ::CCM_DDS::TopicArray::StateListener,
    ::CCM_DDS::ConnectorStatusListener> TopicArray_Connector_Traits;

  class VT_ARRAY_CONNECTOR_EXEC_Export VT_Array_Connector_exec_i :
    public DDS_Event_Connector_T <TopicArray_DDS_Traits, TopicArray_Connector_Traits>
  {
  public:
    VT_Array_Connector_exec_i (void);
    virtual ~VT_Array_Connector_exec_i (void);
  };

  extern "C" VT_ARRAY_CONNECTOR_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_VT_Array_VT_Array_Connector_Impl (void);
}

#include /**/ "ace/post.h"

#endif /* ifndef */
