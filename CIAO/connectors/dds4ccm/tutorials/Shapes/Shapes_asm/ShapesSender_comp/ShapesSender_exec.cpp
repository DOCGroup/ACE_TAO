// -*- C++ -*-
// $Id$

#include "ShapesSender_exec.h"
#include "ace/Guard_T.h"
#include "ciao/Logger/Log_Macros.h"
#include "tao/ORB_Core.h"

namespace CIAO_Shapes_Sender_comp_Impl
{
  //============================================================
  // pulse_Generator
  //============================================================
  pulse_Generator::pulse_Generator (Sender_exec_i &callback)
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
  // Sender_exec_i
  //============================================================
  Sender_exec_i::Sender_exec_i (void)
    : rate_ (1),
      max_x_ (100),
      max_y_ (100),
      max_size_ (25),
      resize_ (false),
      x_increasing_ (false),
      y_increasing_ (false),
      size_increasing_ (false)
  {
    square_ = new ShapeType;
    this->ticker_ = new pulse_Generator (*this);
  }

  Sender_exec_i::~Sender_exec_i (void)
  {
  }

  // Supported operations and attributes.

  void
  Sender_exec_i::tick ()
  {
    if (this->x_increasing_)
      {
        ++square_->x;
        this->x_increasing_ = square_->x + 1 <= this->max_x_;
      }
    else
      {
        --square_->x;
        this->x_increasing_ = square_->x - 1 < 0;
      }
    if (this->y_increasing_)
      {
        ++square_->y;
        this->y_increasing_ = square_->y + 1 <= this->max_y_;
      }
    else
      {
        --square_->y;
        this->y_increasing_ = square_->y - 1 < 0;
      }
    if (resize_shape ())
      {
        if (this->size_increasing_)
          {
            ++square_->shapesize;
            this->size_increasing_ = square_->shapesize + 1 <= this->max_size_;
          }
        else
          {
            --square_->shapesize;
            this->size_increasing_ = square_->shapesize - 1 < 0;
          }
      }
    try
      {
        this->writer_->write_one (*square_, this->instance_handle_);
        ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("UPDATED Shape_info for <%C> %u:%u:%u\n"),
                  square_->color.in (),
                  square_->x,
                  square_->y,
                  square_->shapesize));
      }
    catch (const CCM_DDS::NonExistent& )
      {
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("Shape_info for <%C> not updated: <%C> didn't exist.\n"),
                    square_->color.in (), square_->color.in ()));
      }
    catch (const CCM_DDS::InternalError& )
      {
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("Internal Error while updating Shape_info for <%C>.\n"),
                    square_->color.in ()));
      }
  }

  void
  Sender_exec_i::start (void)
  {
    // calculate the interval time
    long const usec = 1000000 / this->rate_;
    if (this->context_->get_CCM_object()->_get_orb ()->orb_core ()->reactor ()->schedule_timer (
                this->ticker_,
                0,
                ACE_Time_Value (0, usec),
                ACE_Time_Value (0, usec)) == -1)
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("Sender_exec_i::start : ")
                             ACE_TEXT ("Error scheduling timer")));
    }
  }

  void
  Sender_exec_i::stop (void)
  {
    this->context_->get_CCM_object()->_get_orb ()->orb_core ()->reactor ()->cancel_timer (this->ticker_);
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Sender_exec_i::stop : Timer canceled.\n")));
    try
      {
        this->writer_->unregister_instance (*this->square_, this->instance_handle_);
      }
    catch (const CCM_DDS::NonExistent& )
      {
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("Shape_info for <%C> not deleted: <%C> didn't exist.\n"),
                    square_->color.in (), square_->color.in ()));
      }
    catch (const CCM_DDS::InternalError& )
      {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("Internal Error while deleting Shape_info for <%C>.\n"),
                    square_->color.in ()));
      }
    delete this->ticker_;
    delete this->square_;
  }

  ::CORBA::ULong
  Sender_exec_i::rate (void)
  {
    return this->rate_;
  }

  void
  Sender_exec_i::rate (::CORBA::ULong rate)
  {
    this->rate_ = rate;
  }

  ::CORBA::UShort
  Sender_exec_i::max_x (void)
  {
    return this->max_x_;
  }

  void
  Sender_exec_i::max_x (::CORBA::UShort max_x)
  {
    this->max_x_ = max_x;
  }

  ::CORBA::UShort
  Sender_exec_i::max_y (void)
  {
    return this->max_y_;
  }

  void
  Sender_exec_i::max_y (::CORBA::UShort max_y)
  {
    this->max_y_ = max_y;
  }

  ::CORBA::UShort
  Sender_exec_i::max_size (void)
  {
    return this->max_size_;
  }

  void
  Sender_exec_i::max_size (::CORBA::UShort max_size)
  {
    this->max_size_ = max_size;
  }

  void
  Sender_exec_i::resize_shape (::CORBA::Boolean resize)
  {
    this->resize_ = resize;
  }

  ::CORBA::Boolean
  Sender_exec_i::resize_shape ()
  {
    return this->resize_;
  }

  ::Shapes::CCM_Control_obj_ptr
  Sender_exec_i::get_control (void)
  {
    return ::Shapes::CCM_Control_obj::_nil ();
  }

  void
  Sender_exec_i::set_session_context (::Components::SessionContext_ptr ctx)
  {
    this->context_ =
      ::Shapes::CCM_Sender_comp_Context::_narrow (ctx);

    if ( ::CORBA::is_nil (this->context_.in ()))
      {
        throw ::CORBA::INTERNAL ();
      }
  }

  void
  Sender_exec_i::configuration_complete (void)
  {
    this->writer_ = this->context_->get_connection_info_write_data ();
  }

  void
  Sender_exec_i::ccm_activate (void)
  {
    this->start ();
    square_->x = ACE_OS::rand () % this->max_x_;
    square_->y = ACE_OS::rand () % this->max_y_;
    square_->shapesize = max_size_;
    square_->color = CORBA::string_dup("GREEN");

    //Register shape with dds.
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("REGISTER Shape_info for <%C> %u:%u:%u\n"),
                  square_->color.in (),
                  square_->x,
                  square_->y,
                  square_->shapesize));
    try
      {
        this->instance_handle_ = this->writer_->register_instance (*square_);
      }
    catch (const CCM_DDS::AlreadyCreated& )
      {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("Shape_info for <%C> already created.\n"),
                    square_->color.in ()));
      }
    catch (const CCM_DDS::InternalError& )
      {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("Internal Error while creating Shape_info for <%C>.\n"),
                      square_->color.in ()));
      }
  }

  void
  Sender_exec_i::ccm_passivate (void)
  {
    this->stop ();
  }

  void
  Sender_exec_i::ccm_remove (void)
  {
  }

  extern "C" SHAPES_SENDER_COMP_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_Shape_Sender_Impl (void)
  {
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_NORETURN (
      retval,
      Sender_exec_i);

    return retval;
  }
}

