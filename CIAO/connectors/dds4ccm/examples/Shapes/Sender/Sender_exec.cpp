// -*- C++ -*-
// $Id$

#include "Sender_exec.h"
#include "ace/Guard_T.h"
#include "ciao/Logger/Log_Macros.h"

namespace CIAO_Shapes_Sender_Impl
{
  //============================================================
  // Pulse generator
  //============================================================

  pulse_Generator::pulse_Generator (Sender_exec_i &callback)
    : active_ (0),
      pulse_callback_ (callback)
  {
    // initialize the reactor
    this->reactor (ACE_Reactor::instance ());
  }

  pulse_Generator::~pulse_Generator ()
  {
  }

  int
  pulse_Generator::open_h ()
  {
    // convert the task into a active object that runs in separate thread
    return this->activate ();
  }

  int
  pulse_Generator::close_h ()
  {
    this->reactor ()->end_reactor_event_loop ();

    // wait for all threads in the task to exit before it returns
    return this->wait ();
  }

  int
  pulse_Generator::start (CORBA::ULong hertz)
  {
    // return if not valid
    if (hertz == 0 || this->active_ != 0)
      {
        return -1;
      }

    // calculate the interval time
    long usec = 1000000 / hertz;

    std::cerr << "Starting pulse_generator with hertz of " << hertz << ", interval of "
              << usec << std::endl;

    if (this->reactor ()->schedule_timer (this,
                                          0,
                                          ACE_Time_Value (0, usec),
                                          ACE_Time_Value (0, usec)) == -1)
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                          "Unable to setup Timer\n"),
                            -1);
      }
    this->active_ = 1;
    return 0;
  }

  int
  pulse_Generator::stop (void)
  {
    // return if not valid.
    if (this->active_ == 0)
      {
        return -1;
      }
    // cancle the timer
    this->reactor ()->cancel_timer (this);
    this->active_ = 0;
    return 0;
  }

  int
  pulse_Generator::active (void)
  {
    return this->active_;
  }

  int
  pulse_Generator::handle_close (ACE_HANDLE handle,
                                 ACE_Reactor_Mask close_mask)
  {
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("[%x] handle = %d, close_mask = %d\n"),
                this,
                handle,
                close_mask));
    return 0;
  }

  int
  pulse_Generator::handle_timeout (const ACE_Time_Value &,
                                   const void *)
  {
    // Notify the subscribers
    this->pulse_callback_.tick ();
    return 0;
  }

  int
  pulse_Generator::svc (void)
  {
    // define the owner of the reactor thread
    this->reactor ()->owner (ACE_OS::thr_self ());

    // run event loop to wait for event, and then dispatch them to corresponding handlers
    this->reactor ()->run_reactor_event_loop ();

    return 0;
  }

  //============================================================
  // Component Executor Implementation Class: Sender_exec_i
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
        this->updater_->update (*square_);
        CIAO_DEBUG ((LM_DEBUG, ACE_TEXT ("UPDATED Shape_info for <%C> %u:%u:%u\n"),
                  square_->color.in (),
                  square_->x,
                  square_->y,
                  square_->shapesize));
      }
    catch (CCM_DDS::NonExistent& )
      {
        CIAO_ERROR ((LM_ERROR, ACE_TEXT ("Shape_info for <%C> not updated: <%C> didn't exist.\n"),
                    square_->color.in (), square_->color.in ()));
      }
    catch (CCM_DDS::InternalError& )
      {
        CIAO_ERROR ((LM_ERROR, ACE_TEXT ("Internal Error while updating Shape_info for <%C>.\n"),
                    square_->color.in ()));
      }
  }

  void
  Sender_exec_i::start (void)
  {
    this->ticker_->start (this->rate_);
  }

  void
  Sender_exec_i::stop (void)
  {
    this->ticker_->stop ();
  }

  // Component attributes.
  ::CORBA::ULong
  Sender_exec_i::rate (void)
  {
    return this->rate_;
  }

  void
  Sender_exec_i::rate (
    ::CORBA::ULong rate)
  {
    this->rate_ = rate;
  }


  ::CORBA::UShort
  Sender_exec_i::max_x (void)
  {
    return this->max_x_;
  }

  void
  Sender_exec_i::max_x (
    ::CORBA::UShort max_x)
  {
    this->max_x_ = max_x;
  }

  ::CORBA::UShort
  Sender_exec_i::max_y (void)
  {
    return this->max_y_;
  }

  void
  Sender_exec_i::max_y (
    ::CORBA::UShort max_y)
  {
    this->max_y_ = max_y;
  }

  ::CORBA::UShort
  Sender_exec_i::max_size (void)
  {
    return this->max_size_;
  }

  void
  Sender_exec_i::max_size (
    ::CORBA::UShort max_size)
  {
    this->max_size_ = max_size;
  }

  void
  Sender_exec_i::resize_shape (
      ::CORBA::Boolean resize)
  {
    this->resize_ = resize;
  }

  ::CORBA::Boolean
  Sender_exec_i::resize_shape ()
  {
    return this->resize_;
  }

  // Port operations.

  // Operations from Components::SessionComponent.
  void
  Sender_exec_i::set_session_context (
    ::Components::SessionContext_ptr ctx)
  {
    this->context_ =
      ::Shapes::CCM_Sender_Context::_narrow (ctx);

    if ( ::CORBA::is_nil (this->context_.in ()))
      {
        throw ::CORBA::INTERNAL ();
      }
  }

  void
  Sender_exec_i::configuration_complete (void)
  {
    this->updater_ = this->context_->get_connection_info_update_data ();
    this->ticker_->activate ();
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
    CIAO_DEBUG ((LM_DEBUG, ACE_TEXT ("REGISTER Shape_info for <%C> %u:%u:%u\n"),
                  square_->color.in (),
                  square_->x,
                  square_->y,
                  square_->shapesize));
    try
      {
        this->updater_->create (*square_);
      }
    catch (CCM_DDS::AlreadyCreated& )
      {
        CIAO_ERROR ((LM_ERROR, ACE_TEXT ("Shape_info for <%C> already created.\n"),
                    square_->color.in ()));
      }
    catch (CCM_DDS::InternalError& )
      {
        CIAO_ERROR ((LM_ERROR, ACE_TEXT ("Internal Error while creating Shape_info for <%C>.\n"),
                      square_->color.in ()));
      }
  }

  void
  Sender_exec_i::ccm_passivate (void)
  {
  }

  void
  Sender_exec_i::ccm_remove (void)
  {
  }

  extern "C" SENDER_EXEC_Export ::Components::EnterpriseComponent_ptr
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

