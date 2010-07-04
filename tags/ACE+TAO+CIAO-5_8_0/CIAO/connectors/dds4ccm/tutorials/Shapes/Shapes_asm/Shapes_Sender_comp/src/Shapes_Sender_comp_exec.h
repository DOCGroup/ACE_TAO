// -*- C++ -*-
// $Id$

#ifndef CIAO_SENDER_EXEC_H_
#define CIAO_SENDER_EXEC_H_

#include "Shapes_Sender_compEC.h"
#include "Shapes_Control_objEC.h"

#include /**/ "Shapes_Sender_comp_exec_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/LocalObject.h"
#include "ace/Reactor.h"

namespace CIAO_Shapes_Sender_comp_Impl
{
  class Sender_exec_i;

  //============================================================
  // Controller_exec_i
  //============================================================
  class Controller_exec_i
    : public virtual ::Shapes::CCM_Control_obj,
      public virtual ::CORBA::LocalObject
  {
  public:
    Controller_exec_i (Sender_exec_i &callback);
    virtual ~Controller_exec_i (void);

    virtual ::Shapes::ReturnStatus 
    setSize (::CORBA::UShort size);

    virtual ::Shapes::ReturnStatus 
    setLocation (::CORBA::UShort x, ::CORBA::UShort y);

  private:
    Sender_exec_i &callback_;
  };

  //============================================================
  // Sender_exec_i
  //============================================================
  class Sender_exec_i
    : public virtual Sender_comp_Exec,
      public virtual ::CORBA::LocalObject
  {
  public:
    Sender_exec_i (void);
    virtual ~Sender_exec_i (void);

    virtual ::Shapes::CCM_Control_obj_ptr
    get_control (void);

    virtual void set_session_context (::Components::SessionContext_ptr ctx);

    virtual void configuration_complete (void);

    virtual void ccm_activate (void);
    virtual void ccm_passivate (void);
    virtual void ccm_remove (void);

    ::Shapes::ReturnStatus
    setSize (::CORBA::UShort size);

    ::Shapes::ReturnStatus
    setLocation (::CORBA::UShort x, ::CORBA::UShort y);

  private:
    ::Shapes::CCM_Sender_comp_Context_var context_;
    ::DDS::InstanceHandle_t instance_handle_;

    ShapeType_msg square_;

    ::Shapes::ReturnStatus
    update_square (void);
  };

  extern "C" SHAPES_SENDER_COMP_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_Shapes_Sender_comp_Impl (void);
}

#endif /* ifndef */

