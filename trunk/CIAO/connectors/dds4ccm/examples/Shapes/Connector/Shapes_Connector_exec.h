// -*- C++ -*-
// $Id$

#ifndef SHAPES_CONNECTOR_EXEC_H_
#define SHAPES_CONNECTOR_EXEC_H_

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

  // @from use of ShapeType as a parameter
  typedef CIAO::DDS4CCM::RTI::Type_Traits < 
    ShapeType,
    ShapeTypeSeq,
    ShapeTypeTypeSupport,
    ShapeTypeDataWriter,
    ShapeTypeDataReader > ShapeType_DDS_Traits;

  typedef Connector_Traits <
    ::CIAO_Shapes_Shapes_Connector_Impl::Shapes_Connector_Exec,
    ShapeType_Seq,
    ::CCM_DDS::ShapeType::CCM_Writer,
    ::CCM_DDS::ShapeType::CCM_Updater,
    ::CCM_DDS::ShapeType::CCM_Getter,
    ::CCM_DDS::ShapeType::CCM_Reader,
    ::Shapes::CCM_Shapes_Connector_Context,
    ::CCM_DDS::ShapeType::Listener,
    ::CCM_DDS::ShapeType::StateListener,
    ::CCM_DDS::ConnectorStatusListener> ShapeType_Connector_Traits;

  class SHAPES_CONNECTOR_EXEC_Export Shapes_Connector_exec_i :
    public DDS_Event_Connector_T <ShapeType_DDS_Traits, ShapeType_Connector_Traits>
  {
  public:
    Shapes_Connector_exec_i (const char * topic_name);
    virtual ~Shapes_Connector_exec_i (void);
  };

  extern "C" SHAPES_CONNECTOR_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_Shapes_Shapes_Connector_Impl (void);
}

#include /**/ "ace/post.h"

#endif /* ifndef */

