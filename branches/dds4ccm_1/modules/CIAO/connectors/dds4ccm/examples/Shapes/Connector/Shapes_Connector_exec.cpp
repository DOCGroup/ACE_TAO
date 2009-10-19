// -*- C++ -*-
// $Id$

#include "Shapes_Connector_exec.h"

namespace CIAO_Shapes_Shapes_Connector_Impl
{
  Shapes_Connector_exec_i::Shapes_Connector_exec_i (const char * topic_name)
    : Connector_T<Shape_Info_DDS_Traits, Shape_Info_Connector_Traits> (topic_name)
  {
  }

  Shapes_Connector_exec_i::~Shapes_Connector_exec_i (void)
  {
  }

  extern "C" SHAPES_CONNECTOR_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_Shapes_Shapes_Connector_Impl (void)
  {
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_NORETURN (
      retval,
      Shapes_Connector_exec_i ("Shapes_Topic"));

    return retval;
  }
}

