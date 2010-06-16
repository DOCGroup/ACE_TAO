// -*- C++ -*-
// $Id$

#ifndef CIAO_CONTROL_EXEC_H_
#define CIAO_CONTROL_EXEC_H_

#include "ShapesControl_compEC.h"

#include /**/ "ShapesControl_exec_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/LocalObject.h"
#include "ace/Reactor.h"

namespace CIAO_Shapes_Control_comp_Impl
{
  class Control_exec_i;

  //============================================================
  // pulse_Generator
  //============================================================
  class pulse_Generator : public ACE_Event_Handler
  {
  public:
    pulse_Generator (Control_exec_i &callback);

    virtual ~pulse_Generator ();

    virtual int handle_timeout (const ACE_Time_Value &tv,
                                const void *arg);
  private:
    Control_exec_i &pulse_callback_;
  };

  //============================================================
  // Control_exec_i
  //============================================================
  class Control_exec_i
    : public virtual Control_comp_Exec,
      public virtual ::CORBA::LocalObject
  {
  public:
    Control_exec_i (void);
    virtual ~Control_exec_i (void);

    ::CORBA::ULong rate (void);

    void rate (::CORBA::ULong rate);

    ::CORBA::UShort max_x (void);

    void max_x (::CORBA::UShort max_x);

    ::CORBA::UShort max_y (void);

    void max_y (::CORBA::UShort max_y);

    ::CORBA::UShort max_size (void);

    void max_size (::CORBA::UShort max_size);

    void resize_shape (::CORBA::Boolean resize);

    ::CORBA::Boolean resize_shape (void);

    virtual ::Shapes::CCM_Control_obj_ptr
    get_control (void);

    virtual void set_session_context (::Components::SessionContext_ptr ctx);

    virtual void configuration_complete (void);

    virtual void ccm_activate (void);
    virtual void ccm_passivate (void);
    virtual void ccm_remove (void);

    void tick ();

  private:
    ::Shapes::CCM_Control_comp_Context_var context_;

    pulse_Generator * ticker_;

    ::CORBA::ULong rate_;
    ::CORBA::UShort max_x_;
    ::CORBA::UShort max_y_;
    ::CORBA::UShort max_size_;
    ::CORBA::Boolean resize_;

    bool x_increasing_;
    bool y_increasing_;
    bool size_increasing_;

    ::CORBA::UShort current_size_;
    ::Shapes::COLOR current_color_;
    ::CORBA::UShort current_x_;
    ::CORBA::UShort current_y_;

    void start (void);
    void stop (void);
  };

  extern "C" SHAPES_CONTROL_COMP_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_Shapes_Control_comp_Impl (void);
}

#endif /* ifndef */

