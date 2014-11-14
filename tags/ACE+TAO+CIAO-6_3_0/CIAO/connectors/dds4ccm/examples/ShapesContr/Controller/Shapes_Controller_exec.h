// -*- C++ -*-
// $Id$

#ifndef CIAO_SHAPES_CONTROLLER_EXEC_ETSDUL_H_
#define CIAO_SHAPES_CONTROLLER_EXEC_ETSDUL_H_

#include /**/ "ace/pre.h"

#include "Shapes_ControllerEC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include /**/ "Controller_exec_export.h"
#include "tao/LocalObject.h"


namespace CIAO_Shapes_ShapesController_Impl
{

  class Controller_exec_i;

  class shape_attributes_get_i
      : public virtual CCM_shape_attributes_get,
        public virtual ::CORBA::LocalObject
  {
  public:
    shape_attributes_get_i (Controller_exec_i& component)
          : component_ (component)
    {
    }

    CORBA::UShort max_size (void);
    CORBA::UShort max_x (void);
    CORBA::UShort max_y (void);
    CORBA::Boolean resize (void);
    CORBA::ULong publish_rate (void);

  private:
    Controller_exec_i& component_;
  };

  class shape_attributes_set_i
        : public virtual CCM_shape_attributes_set,
          public virtual ::CORBA::LocalObject
  {
  public:
    shape_attributes_set_i (Controller_exec_i& component)
          : component_ (component)
    {
    }

    void max_size (CORBA::UShort);
    void max_x (CORBA::UShort);
    void max_y (CORBA::UShort);
    void resize (CORBA::Boolean);
    void publish_rate (CORBA::ULong);

    void get_attr_values (CORBA::UShort_out max_size,
                          CORBA::UShort_out max_x,
                          CORBA::UShort_out max_y,
                          CORBA::ULong_out publish_rate,
                          CORBA::Boolean_out resize);

    void update_changes ();

  private:
    Controller_exec_i& component_;
   };

  /**
   * Component Executor Implementation Class: Controller_exec_i
   */

  class Controller_exec_i
    : public virtual ShapesController_Exec,
      public virtual ::CORBA::LocalObject
  {
  public:
    Controller_exec_i (void);
    virtual ~Controller_exec_i (void);

    //@{
    /** Supported operations and attributes. */

    //@}

    //@{
    /** Component attributes and port operations. */

    virtual ::CORBA::ULong publish_rate (void);

    virtual void publish_rate (::CORBA::ULong publish_rate);

    virtual ::CORBA::UShort max_size (void);

    virtual void max_size (::CORBA::UShort max_size);

    virtual ::CORBA::UShort max_x (void);

    virtual void max_x (::CORBA::UShort max_x);

    virtual ::CORBA::UShort max_y (void);

    virtual void max_y (::CORBA::UShort max_y);

    virtual ::CORBA::Boolean resize_shape (void);

    virtual void resize_shape (::CORBA::Boolean resize_shape);

    virtual ::CORBA::Boolean use_web_interface (void);

    virtual void use_web_interface (::CORBA::Boolean use_web_interface);

    virtual CCM_shape_attributes_get* get_shapeAttribsPub (void);

    virtual CCM_shape_attributes_set* get_shapeAttribsSet (void);
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
    /** User defined public operations. */
    virtual void invoke_shape_changes (void);

    //@}

  private:
    ::Shapes::CCM_ShapesController_Context_var ciao_context_;

    //@{
    /** Component attributes. */
    ::CORBA::ULong publish_rate_;
    ::CORBA::UShort max_size_;
    ::CORBA::UShort max_x_;
    ::CORBA::UShort max_y_;
    ::CORBA::Boolean resize_shape_;
    ::CORBA::Boolean use_web_interface_;
    //@}

    //@{
    /** User defined members. */
    //@}

    //@{
    /** User defined private operations. */
    //@}
  };
}

extern "C" CONTROLLER_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_Shapes_Controller_Impl (void);


#include /**/ "ace/post.h"

#endif /* ifndef */
