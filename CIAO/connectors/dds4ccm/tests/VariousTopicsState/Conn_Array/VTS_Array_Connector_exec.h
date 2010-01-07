// -*- C++ -*-
// $Id$

#ifndef VTS_ARRAY_CONNECTOR_EXEC_H_
#define VTS_ARRAY_CONNECTOR_EXEC_H_

#include /**/ "ace/pre.h"

#include "VTS_Array_ConnectorEC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include /**/ "VTS_Array_Connector_exec_export.h"
#include "tao/LocalObject.h"

#include "dds4ccm/impl/ndds/DDS4CCM_Traits.h"
#include "dds4ccm/impl/ndds/DDS_Event_Connector_T.h"


#include "Base/VTopics_BaseSupport.h"

namespace CIAO_VTS_Array_VTS_Array_Connector_Impl
{

  // @from use of VTopics as a parameter
  typedef CIAO::DDS4CCM::RTI::Type_Traits < 
    TopicArray,
    TopicArraySeq,
    TopicArrayTypeSupport,
    TopicArrayDataWriter,
    TopicArrayDataReader > TopicArray_DDS_Traits;

  typedef CIAO::DDS4CCM::Connector_Traits <
	  ::CIAO_VTopics_VTS_Array_Connector_Impl::VTS_Array_Connector_Exec,
    TopicArray_Seq,
    ::CCM_DDS::TopicArray::CCM_Writer,
    ::CCM_DDS::TopicArray::CCM_Updater,
    ::CCM_DDS::TopicArray::CCM_Getter,
    ::CCM_DDS::TopicArray::CCM_Reader,
    ::VTopics::CCM_VTS_Array_Connector_Context,
    ::CCM_DDS::TopicArray::Listener,
    ::CCM_DDS::TopicArray::StateListener,
    ::CCM_DDS::ConnectorStatusListener> TopicArray_Connector_Traits;

  class VTS_ARRAY_CONNECTOR_EXEC_Export VTS_Array_Connector_exec_i :
    public DDS_Event_Connector_T <TopicArray_DDS_Traits, TopicArray_Connector_Traits>
  {
  public:
    VTS_Array_Connector_exec_i (void);
    virtual ~VTS_Array_Connector_exec_i (void);
  };

  extern "C" VTS_ARRAY_CONNECTOR_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_VTS_Array_VTS_Array_Connector_Impl (void);
}

#include /**/ "ace/post.h"

#endif /* ifndef */
