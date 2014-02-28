// -*- C++ -*-
// $Id$

/**
 * @author Marijke Hengstmengel (mhengstmengel@remedy.nl)
 */


#include "Shapes_Controller_exec.h"
#include "tao/ORB_Core.h"
#include "ace/Reactor.h"

namespace CIAO_Shapes_ShapesController_Impl
{
  CORBA::UShort
  shape_attributes_get_i::max_size ()
  {
    return component_.max_size ();
  }

  CORBA::UShort
  shape_attributes_get_i::max_x ()
  {
    return component_.max_x ();
  }

  CORBA::UShort
  shape_attributes_get_i::max_y ()
  {
    return component_.max_y ();
  }

  CORBA::Boolean
  shape_attributes_get_i::resize ()
  {
    return component_.resize_shape();
  }
  CORBA::ULong
  shape_attributes_get_i::publish_rate ()
  {
    return (component_.publish_rate());
  }

  void
  shape_attributes_set_i::max_size ( CORBA::UShort max_size)
  {
    component_.max_size (max_size);
  }

  void
  shape_attributes_set_i::max_x ( CORBA::UShort max_x)
  {
    component_.max_x (max_x);
  }

  void
  shape_attributes_set_i::max_y ( CORBA::UShort max_y)
  {
    component_.max_y (max_y);
  }

  void
  shape_attributes_set_i::resize (CORBA::Boolean resize)
  {
    component_.resize_shape(resize);
  }

  void
  shape_attributes_set_i::publish_rate (CORBA::ULong pub_rate)
  {
    component_.publish_rate(pub_rate);
  }

  void
  shape_attributes_set_i::get_attr_values (CORBA::UShort_out max_size,
                                           CORBA::UShort_out max_x,
                                           CORBA::UShort_out max_y,
                                           CORBA::ULong_out publish_rate,
                                           CORBA::Boolean_out resize)
  {
    max_size = component_.max_size ();
    max_x = component_.max_x ();
    max_y = component_.max_y ();
    publish_rate = component_.publish_rate ();
    resize = component_.resize_shape();
  }

  void
  shape_attributes_set_i::update_changes ()
  {
    component_.invoke_shape_changes ();
  }

  /**
   * Component Executor Implementation Class: Controller_exec_i
   */

  Controller_exec_i::Controller_exec_i (void)
      : publish_rate_ (0)
      , max_size_ (50)
      , max_x_ (200)
      , max_y_ (200)
      , resize_shape_ (false)
      , use_web_interface_ (true)
  {
  }

  Controller_exec_i::~Controller_exec_i (void)
  {
  }

  // Supported operations and attributes.

  // Component attributes and port operations.

  ::CORBA::ULong
  Controller_exec_i::publish_rate (void)
  {
    return this->publish_rate_;
  }

  void
   Controller_exec_i::publish_rate (
     const ::CORBA::ULong publish_rate)
   {
     this->publish_rate_ = publish_rate;
   }

  ::CORBA::UShort
  Controller_exec_i::max_size (void)
  {
    return this->max_size_;
  }

  void
  Controller_exec_i::max_size (
    const ::CORBA::UShort max_size)
  {
    this->max_size_ = max_size;
  }

  ::CORBA::UShort
  Controller_exec_i::max_x (void)
  {
    return this->max_x_;
  }

  void
  Controller_exec_i::max_x (
     const ::CORBA::UShort max_x)
  {
    this->max_x_ = max_x;
  }

  ::CORBA::UShort
  Controller_exec_i::max_y (void)
  {
    return this->max_y_;
  }

  void
  Controller_exec_i::max_y (
    const ::CORBA::UShort max_y)
  {
    this->max_y_ = max_y;
  }

  ::CORBA::Boolean
  Controller_exec_i::resize_shape (void)
  {
    return this->resize_shape_;
  }

  void
  Controller_exec_i::resize_shape (
    const ::CORBA::Boolean resize_shape)
  {
    this->resize_shape_ = resize_shape;
  }

  ::CORBA::Boolean
   Controller_exec_i::use_web_interface (void)
   {
     return this->use_web_interface_;
   }

   void
   Controller_exec_i::use_web_interface (
     const ::CORBA::Boolean use_web_interface)
   {
     this->use_web_interface_ = use_web_interface;
   }

  void
  Controller_exec_i::invoke_shape_changes (void)
  {
    shape_changes_var invoker =
      this->ciao_context_->get_connection_shapeChangesSub ();

     invoker->pass_change_event ();
  }

  CCM_shape_attributes_get*
  Controller_exec_i::get_shapeAttribsPub ()
  {
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Controller_exec_i::get_shapeAttribsPub\n")));
    return ( new shape_attributes_get_i (*this) );

  }

  CCM_shape_attributes_set*
  Controller_exec_i::get_shapeAttribsSet ()
  {
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Controller_exec_i::get_shapeAttribsSet\n")));
    return ( new shape_attributes_set_i (*this) );

  }

  // Operations from Components::SessionComponent.

  void
  Controller_exec_i::set_session_context (
    ::Components::SessionContext_ptr ctx)
  {
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Controller_exec_i::set_session_context\n")));

    this->ciao_context_ =
      ::Shapes::CCM_ShapesController_Context::_narrow (ctx);

    if ( ::CORBA::is_nil (this->ciao_context_.in ()))
      {
        throw ::CORBA::INTERNAL ();
      }
  }

  void
  Controller_exec_i::configuration_complete (void)
  {
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Controller_exec_i::configuration_complete\n")));
  }

  void
  Controller_exec_i::ccm_activate (void)
  {
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Controller_exec_i::ccm_activate\n")));
    // If no extern web interface is used to start and stop  publishing,
    // start publishing now.
    if (!use_web_interface_)
    {
      this->publish_rate_ = 1;
      this->invoke_shape_changes();
    }
  }

  void
  Controller_exec_i::ccm_passivate (void)
  {
  }

  void
  Controller_exec_i::ccm_remove (void)
  {
    /* Your code here. */
  }

  extern "C" CONTROLLER_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_Shapes_Controller_Impl (void)
  {
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_NORETURN (
      retval,
      Controller_exec_i);

    return retval;
  }
}
