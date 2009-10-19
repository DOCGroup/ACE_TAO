// -*- C++ -*-
// $Id$

#ifndef CIAO_SHAPES_CONNECTOR_EXEC_H_
#define CIAO_SHAPES_CONNECTOR_EXEC_H_

#include /**/ "ace/pre.h"

#include "Shapes_ConnectorEC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include /**/ "Shapes_Connector_exec_export.h"
#include "tao/LocalObject.h"

#include "dds4ccm/impl/ndds/NDDS_Traits.h"
#include "dds4ccm/impl/ndds/Connector_T.h"

#include "Base/Shapes_BaseSupport.h"

namespace CIAO_Shapes_Shapes_Connector_Impl
{

  // @from use of Shape_Info as a parameter
  typedef CIAO::DDS4CCM::RTI::Type_Traits < 
    ::Shapes::Shape_Info,
    ::Shapes::Shape_Info_Seq,
    ::Shapes::Shape_InfoSeq,
    ::Shapes::Shape_InfoTypeSupport,
    ::Shapes::Shape_InfoDataWriter,
    ::Shapes::Shape_InfoDataReader > Shape_Info_DDS_Traits;

  typedef Connector_Traits <
    ::CIAO_Shapes_Shapes_Connector_Impl::Shapes_Connector_Exec,
    ::CCM_DDS::CCM_Shape_Info_Writer,
    ::CCM_DDS::CCM_Shape_Info_Updater,
    ::CCM_DDS::CCM_Shape_Info_Getter,
    ::CCM_DDS::CCM_Shape_Info_Reader,
    ::Shapes::CCM_Shapes_Connector_Context,
    ::CCM_DDS::Shape_Info_RawListener> Shape_Info_Connector_Traits;

  class Shapes_CONNECTOR_EXEC_Export Shapes_Connector_exec_i :
    public Connector_T <Shape_Info_DDS_Traits, Shape_Info_Connector_Traits>
  {
  public:
    Shapes_Connector_exec_i (const char * topic_name);
    virtual ~Shapes_Connector_exec_i (void);
  };

  extern "C" Shapes_CONNECTOR_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_Shapes_Shapes_Connector_Impl (void);
}

#include /**/ "ace/post.h"

#endif /* ifndef */

