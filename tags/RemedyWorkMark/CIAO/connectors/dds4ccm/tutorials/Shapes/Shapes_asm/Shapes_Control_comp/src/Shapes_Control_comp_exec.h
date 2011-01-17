// -*- C++ -*-
// $Id$

#ifndef CIAO__________TUTORIAL_CODE_SHAPES_CONTROL_COMP_SHAPES_CONTROL_COMP_EXEC_J4CMMK_H_
#define CIAO__________TUTORIAL_CODE_SHAPES_CONTROL_COMP_SHAPES_CONTROL_COMP_EXEC_J4CMMK_H_

#include /**/ "ace/pre.h"

#include "Shapes_Control_compEC.h"

#include /**/ "Shapes_Control_comp_exec_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/LocalObject.h"

namespace CIAO_Shapes_Control_comp_Impl
{
  class Control_comp_exec_i;

  //============================================================
  // pulse_Generator
  //============================================================
  class pulse_Generator : public ACE_Event_Handler
  {
  public:
    pulse_Generator (Control_comp_exec_i &callback);

    virtual ~pulse_Generator ();

    virtual int handle_timeout (const ACE_Time_Value &tv,
                                const void *arg);
  private:
    Control_comp_exec_i &pulse_callback_;
  };

  //============================================================
  // Component Executor Implementation Class: Control_comp_exec_i
  //============================================================

  class SHAPES_CONTROL_COMP_EXEC_Export Control_comp_exec_i
    : public virtual Control_comp_Exec,
      public virtual ::CORBA::LocalObject
  {
  public:
    Control_comp_exec_i (void);
    virtual ~Control_comp_exec_i (void);

    //@{
    /** Supported operations and attributes. */

    //@}

    //@{
    /** Component attributes and port operations. */

    virtual ::CORBA::ULong rate (void);

    virtual void rate (::CORBA::ULong rate);

    virtual ::CORBA::UShort max_x (void);

    virtual void max_x (::CORBA::UShort max_x);

    virtual ::CORBA::UShort max_y (void);

    virtual void max_y (::CORBA::UShort max_y);

    virtual ::CORBA::UShort max_size (void);

    virtual void max_size (::CORBA::UShort max_size);

    virtual ::CORBA::Boolean resize_shape (void);

    virtual void resize_shape (::CORBA::Boolean resize_shape);
    //@}

    //@{
    /** Operations from Components::SessionComponent. */

    virtual void set_session_context (::Components::SessionContext_ptr ctx);

    virtual void configuration_complete (void);

    virtual void ccm_activate (void);
    virtual void ccm_passivate (void);
    virtual void ccm_remove (void);
    //@}

    //@{
    /** User defined operations */
    void tick (void);

    //@}

  private:
    ::Shapes::CCM_Control_comp_Context_var ciao_context_;

    ::CORBA::ULong rate_;

    ::CORBA::UShort max_x_;

    ::CORBA::UShort max_y_;

    ::CORBA::UShort max_size_;

    ::CORBA::Boolean resize_shape_;

    bool x_increasing_;
    bool y_increasing_;
    bool size_increasing_;

    ::CORBA::UShort current_size_;
    ::CORBA::UShort current_x_;
    ::CORBA::UShort current_y_;

    pulse_Generator * ticker_;

    void start (void);
    void stop (void);

    ACE_Reactor* reactor (void);
  };

  extern "C" SHAPES_CONTROL_COMP_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_Shapes_Control_comp_Impl (void);
}

#include /**/ "ace/post.h"

#endif /* ifndef */
