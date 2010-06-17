// -*- C++ -*-
// $Id$

#ifndef SHAPES_RECEIVER_EXEC_H_
#define SHAPES_RECEIVER_EXEC_H_

#include "Shapes_Receiver_compEC.h"

#include /**/ "Shapes_Receiver_comp_exec_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/LocalObject.h"
#include "ace/Reactor.h"

namespace CIAO_Shapes_Receiver_comp_Impl
{
  //============================================================
  // ShapeType_Listener_exec_i
  //============================================================
  class SHAPES_RECEIVER_COMP_EXEC_Export ShapeType_Listener_exec_i
    : public virtual ::Shapes::ShapesConnector::CCM_Listener,
      public virtual ::CORBA::LocalObject
  {
  public:
    ShapeType_Listener_exec_i (void);
    virtual ~ShapeType_Listener_exec_i (void);

    virtual void
    on_one_data (
      const ShapeType & an_instance,
      const ::CCM_DDS::ReadInfo & info);
    virtual void
    on_many_data (
      const ::Shapes::ShapeTypeSeq & an_instance,
      const ::CCM_DDS::ReadInfoSeq & info);
  };

  class SHAPES_RECEIVER_COMP_EXEC_Export Receiver_exec_i
    : public virtual Receiver_comp_Exec,
      public virtual ::CORBA::LocalObject
  {
  public:
    Receiver_exec_i (void);
    virtual ~Receiver_exec_i (void);

    // Supported operations and attributes.

    // Component attributes.

    // Port operations.
    virtual ::Shapes::ShapesConnector::CCM_Listener_ptr
    get_info_out_data_listener (void);

    virtual ::CCM_DDS::CCM_PortStatusListener_ptr
    get_info_out_status (void);

    // Operations from Components::SessionComponent.
    virtual void
    set_session_context (
      ::Components::SessionContext_ptr ctx);

    virtual void configuration_complete (void);

    virtual void ccm_activate (void);
    virtual void ccm_passivate (void);
    virtual void ccm_remove (void);

  private:
    ::Shapes::CCM_Receiver_comp_Context_var context_;
  };

  extern "C" SHAPES_RECEIVER_COMP_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_Shapes_Receiver_comp_Impl (void);
}

#endif /* ifndef */

