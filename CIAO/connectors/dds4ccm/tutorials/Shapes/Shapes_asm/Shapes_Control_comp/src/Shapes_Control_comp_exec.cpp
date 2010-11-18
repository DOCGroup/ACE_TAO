// -*- C++ -*-
// $Id$

#include "Shapes_Control_comp_exec.h"
#include "tao/ORB_Core.h"

#include "ace/Guard_T.h"
#include "ace/Log_Msg.h"
#include "ace/Reactor.h"

#define MIN_SIZE 5

namespace CIAO_Control_comp_Impl
{
  //============================================================
  // pulse_Generator
  //============================================================
  pulse_Generator::pulse_Generator (Control_comp_exec_i &callback)
    : pulse_callback_ (callback)
  {
  }

  pulse_Generator::~pulse_Generator ()
  {
  }

  int
  pulse_Generator::handle_timeout (const ACE_Time_Value &, const void *)
  {
    this->pulse_callback_.tick ();
    return 0;
  }

  //============================================================
  // Component Executor Implementation Class: Control_comp_exec_i
  //============================================================

  Control_comp_exec_i::Control_comp_exec_i (void)
    : rate_ (0)
      , max_x_ (100)
      , max_y_ (100)
      , max_size_ (25)
      , resize_shape_ (false)
      , x_increasing_ (false)
      , y_increasing_ (false)
      , size_increasing_ (false)
      , current_size_ (ACE_OS::rand () % max_size_)
      , current_x_ (ACE_OS::rand () % max_x_)
      , current_y_ (ACE_OS::rand () % max_y_)
  {
    this->ticker_ = new pulse_Generator (*this);
  }

  Control_comp_exec_i::~Control_comp_exec_i (void)
  {
    delete this->ticker_;
  }

  // Supported operations and attributes.
  ACE_Reactor*
  Control_comp_exec_i::reactor (void)
  {
    ACE_Reactor* reactor = 0;
    ::CORBA::Object_var ccm_object =
      this->ciao_context_->get_CCM_object();
    if (! ::CORBA::is_nil (ccm_object.in ()))
      {
        ::CORBA::ORB_var orb = ccm_object->_get_orb ();
        if (! ::CORBA::is_nil (orb.in ()))
          {
            reactor = orb->orb_core ()->reactor ();
          }
      }
    if (reactor == 0)
      {
        throw ::CORBA::INTERNAL ();
      }
    return reactor;
  }

  void
  Control_comp_exec_i::tick ()
  {
    if (this->x_increasing_)
      {
        ++this->current_x_;
        this->x_increasing_ = this->current_x_ + 1 <= this->max_x_;
      }
    else
      {
        --this->current_x_;
        this->x_increasing_ = this->current_x_ - 1 < 0;
      }
    if (this->y_increasing_)
      {
        ++this->current_y_;
        this->y_increasing_ = this->current_y_ + 1 <= this->max_y_;
      }
    else
      {
        --this->current_y_;
        this->y_increasing_ = this->current_y_ - 1 < 0;
      }
    if (resize_shape ())
      {
        if (this->size_increasing_)
          {
            ++this->current_size_;
            this->size_increasing_ = this->current_size_ + 1 <= this->max_size_;
          }
        else
          {
            if (this->current_size_ == MIN_SIZE)
              {
                this->size_increasing_ = true;
              }
            else
              {
                --this->current_size_;
                this->size_increasing_ = this->current_size_ - 1 < 0;
              }
          }
      }
    ::Control_obj_var control =
      this->ciao_context_->get_connection_control ();
    if (! ::CORBA::is_nil (control))
      {
        if (control->setSize (this->current_size_) == ::RETURN_ERROR)
          {
            ACE_ERROR ((LM_ERROR, ACE_TEXT ("Control_comp_exec_i::tick : ")
                                  ACE_TEXT ("Setting shapes size\n")));
          }
        else
          {
            ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Control_comp_exec_i::tick : ")
                                  ACE_TEXT ("Size set to <%u>.\n"),
                                  this->current_size_));
          }
        if (control->setLocation (this->current_x_,
                                  this->current_y_) == ::RETURN_ERROR)
          {
            ACE_ERROR ((LM_ERROR, ACE_TEXT ("Control_comp_exec_i::tick : ")
                                  ACE_TEXT ("Setting shapes location\n")));
          }
        else
          {
            ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Control_comp_exec_i::tick : ")
                                  ACE_TEXT ("Location set to <%u>, <%u>.\n"),
                                  this->current_x_,
                                  this->current_y_));
          }
      }
    else
      {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("Unable to control shapes\n")));
      }
   }

  void
  Control_comp_exec_i::start (void)
  {
    // calculate the interval time
    long const usec = 1000000 / this->rate_;
    if (this->reactor ()->schedule_timer (
                this->ticker_,
                0,
                ACE_Time_Value (0, usec),
                ACE_Time_Value (0, usec)) == -1)
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("Control_comp_exec_i::start : ")
                            ACE_TEXT ("Error scheduling timer\n")));
    }
  }

  void
  Control_comp_exec_i::stop (void)
  {
    this->reactor ()->cancel_timer (this->ticker_);
  }

  // Component attributes and port operations.

  ::CORBA::ULong
  Control_comp_exec_i::rate (void)
  {
    return this->rate_;
  }

  void
  Control_comp_exec_i::rate (
    const ::CORBA::ULong rate)
  {
    this->rate_ = rate;
  }

  ::CORBA::UShort
  Control_comp_exec_i::max_x (void)
  {
    return this->max_x_;
  }

  void
  Control_comp_exec_i::max_x (
    const ::CORBA::UShort max_x)
  {
    this->max_x_ = max_x;
  }

  ::CORBA::UShort
  Control_comp_exec_i::max_y (void)
  {
    return this->max_y_;
  }

  void
  Control_comp_exec_i::max_y (
    const ::CORBA::UShort max_y)
  {
    this->max_y_ = max_y;
  }

  ::CORBA::UShort
  Control_comp_exec_i::max_size (void)
  {
    return this->max_size_;
  }

  void
  Control_comp_exec_i::max_size (
    const ::CORBA::UShort max_size)
  {
    this->max_size_ = max_size;
  }

  ::CORBA::Boolean
  Control_comp_exec_i::resize_shape (void)
  {
    return this->resize_shape_;
  }

  void
  Control_comp_exec_i::resize_shape (
    const ::CORBA::Boolean resize_shape)
  {
    this->resize_shape_ = resize_shape;
  }

  // Operations from Components::SessionComponent.

  void
  Control_comp_exec_i::set_session_context (
    ::Components::SessionContext_ptr ctx)
  {
    this->ciao_context_ =
      ::CCM_Control_comp_Context::_narrow (ctx);

    if ( ::CORBA::is_nil (this->ciao_context_.in ()))
      {
        throw ::CORBA::INTERNAL ();
      }
  }

  void
  Control_comp_exec_i::configuration_complete (void)
  {
    /* Your code here. */
  }

  void
  Control_comp_exec_i::ccm_activate (void)
  {
     this->start ();
  }

  void
  Control_comp_exec_i::ccm_passivate (void)
  {
    this->stop ();
  }

  void
  Control_comp_exec_i::ccm_remove (void)
  {
    /* Your code here. */
  }

  extern "C" SHAPES_CONTROL_COMP_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_Shapes_Control_comp_Impl (void)
  {
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_NORETURN (
      retval,
      Control_comp_exec_i);

    return retval;
  }
}
