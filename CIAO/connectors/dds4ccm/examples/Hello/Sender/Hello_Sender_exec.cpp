// -*- C++ -*-
// $Id$

#include "Hello_Sender_exec.h"
#include "ace/Guard_T.h"
#include "ciao/Logger/Log_Macros.h"
#include "tao/ORB_Core.h"

namespace CIAO_Hello_Sender_Impl
{
  //============================================================
  // Pulse generator
  //============================================================

  pulse_Generator::pulse_Generator (Sender_exec_i &callback)
    : pulse_callback_ (callback)
  {
  }

  pulse_Generator::~pulse_Generator ()
  {
  }

  int
  pulse_Generator::handle_timeout (const ACE_Time_Value &,
                                   const void *)
  {
    // Notify the subscribers
    this->pulse_callback_.tick ();
    return 0;
  }

  //============================================================
  // Component Executor Implementation Class: Sender_exec_i
  //============================================================

  Sender_exec_i::Sender_exec_i (void)
    : rate_ (1),
      iteration_ (0)
  {
    this->ticker_ = new pulse_Generator (*this);
  }

  Sender_exec_i::~Sender_exec_i (void)
  {
  }

  // Supported operations and attributes.

  void
  Sender_exec_i::tick ()
  {
    DDSHello * new_msg = new DDSHello();

    new_msg->hello = "Hello, world!";
    new_msg->iterator = ++this->iteration_;
    printf ("TICK\n");
    this->writer_->write (*new_msg);
  }

  void
  Sender_exec_i::start (void)
  {
    // calculate the interval time
    long usec = 1000000 / this->rate_;
    if (this->context_->get_CCM_object()->_get_orb ()->orb_core ()->reactor ()->schedule_timer (
                this->ticker_,
                0,
                ACE_Time_Value (0, usec),
                ACE_Time_Value (0, usec)) == -1)
    {
      CIAO_ERROR ((LM_ERROR, ACE_TEXT ("Sender_exec_i::start : ")
                             ACE_TEXT ("Error scheduling timer")));
    }
  }

  void
  Sender_exec_i::stop (void)
  {
    this->context_->get_CCM_object()->_get_orb ()->orb_core ()->reactor ()->cancel_timer (this->ticker_);
    CIAO_DEBUG ((LM_DEBUG, ACE_TEXT ("Sender_exec_i::stop : Timer canceled.\n")));
    delete this->ticker_;
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

  // Port operations.

  // Operations from Components::SessionComponent.
  void
  Sender_exec_i::set_session_context (
    ::Components::SessionContext_ptr ctx)
  {
    this->context_ =
      ::Hello::CCM_Sender_Context::_narrow (ctx);

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

  extern "C" HELLO_SENDER_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_Hello_Sender_Impl (void)
  {
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_NORETURN (
      retval,
      Sender_exec_i);

    return retval;
  }
}

