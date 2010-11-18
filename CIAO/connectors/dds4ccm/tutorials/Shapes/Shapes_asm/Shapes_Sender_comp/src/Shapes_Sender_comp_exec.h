// -*- C++ -*-
// $Id$

#ifndef CIAO__________TUTORIAL_CODE_SHAPES_SENDER_COMP_SHAPES_SENDER_COMP_EXEC_FZBRNR_H_
#define CIAO__________TUTORIAL_CODE_SHAPES_SENDER_COMP_SHAPES_SENDER_COMP_EXEC_FZBRNR_H_

#include /**/ "ace/pre.h"

#include "Shapes_Sender_compEC.h"

#include /**/ "Shapes_Sender_comp_exec_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/LocalObject.h"

namespace CIAO_Sender_comp_Impl
{
  class Sender_comp_exec_i;
  //============================================================
  // Provider Executor Implementation Class: control_exec_i
  //============================================================

  class SHAPES_SENDER_COMP_EXEC_Export control_exec_i
    : public virtual ::CCM_Control_obj,
      public virtual ::CORBA::LocalObject
  {
  public:
    control_exec_i (
      ::CCM_Sender_comp_Context_ptr ctx,
      Sender_comp_exec_i &callback);
    virtual ~control_exec_i (void);

    // Operations and attributes from ::Control_obj

    virtual
    ::ReturnStatus setSize (::CORBA::UShort size);

    virtual
    ::ReturnStatus setLocation (::CORBA::UShort x,
    ::CORBA::UShort y);

  private:
    ::CCM_Sender_comp_Context_var ciao_context_;
    Sender_comp_exec_i &callback_;
  };

  //============================================================
  // Component Executor Implementation Class: Sender_comp_exec_i
  //============================================================

  class SHAPES_SENDER_COMP_EXEC_Export Sender_comp_exec_i
    : public virtual Sender_comp_Exec,
      public virtual ::CORBA::LocalObject
  {
  public:
    Sender_comp_exec_i (void);
    virtual ~Sender_comp_exec_i (void);

    //@{
    /** Supported operations and attributes. */

    //@}

    //@{
    /** Component attributes and port operations. */

    virtual ::CCM_Control_obj_ptr
    get_control (void);
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
    ::ReturnStatus setSize (::CORBA::UShort size);

    ::ReturnStatus setLocation (::CORBA::UShort x,
                                ::CORBA::UShort y);

    //@}

  private:
    ::CCM_Sender_comp_Context_var ciao_context_;
    ::CCM_Control_obj_var ciao_control_;

    ::DDS::InstanceHandle_t instance_handle_;
    ShapeType square_;

    ::ReturnStatus update_square (void);
  };

  extern "C" SHAPES_SENDER_COMP_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_Shapes_Sender_comp_Impl (void);
}

#include /**/ "ace/post.h"

#endif /* ifndef */
