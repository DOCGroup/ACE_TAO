// -*- C++ -*-
// $Id$

#include "Hello_Sender_exec.h"
#include "ace/Guard_T.h"
#include "ciao/Logger/Log_Macros.h"
#include "tao/ORB_Core.h"
#include "ace/Date_Time.h"
#include "ace/OS_NS_unistd.h"

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
      iteration_ (0),
      iterations_ (1000),
      log_time_ (false),
      msg_ ("Hello World!")
  {
    this->ticker_ = new pulse_Generator (*this);
  }

  Sender_exec_i::~Sender_exec_i (void)
  {
  }

  // Supported operations and attributes.
  ACE_CString Sender_exec_i::create_message (const ACE_CString &msg)
  {
    if (!this->log_time_)
      return msg;
    char timestamp[16];
    ACE_Date_Time now;
    ACE_OS::sprintf (timestamp,
                      "%02d.%06d",
                      now.second(),
                      now.microsec ());
    ACE_CString ret (timestamp);
    ret = ret + " " + msg;
    return ret.c_str ();
  }
  
  void
  Sender_exec_i::tick ()
  {
    if (this->iteration_ == 0)
      ACE_OS::sleep (1);
    if (this->iteration_ < this->iterations_)
      {
        DDSHello * new_msg = new DDSHello();
        ACE_CString msg = create_message (this->msg_);
        new_msg->hello = msg.c_str ();
        new_msg->iterator = ++this->iteration_;
        this->writer_->write_one (*new_msg, ::DDS::HANDLE_NIL);
      }
    else
      { //we're done
        this->context_->get_CCM_object()->_get_orb ()->orb_core ()->reactor ()->cancel_timer (this->ticker_);
      }
  }

  void
  Sender_exec_i::start (void)
  {
    // calculate the interval time
    long usec = 1000000 / this->rate_;
    if (this->context_->get_CCM_object()->_get_orb ()->orb_core ()->reactor ()->schedule_timer (
                this->ticker_,
                0,
                ACE_Time_Value (5, usec),
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
  Sender_exec_i::iterations (void)
  {
    return this->iterations_;
  }
  
  void
  Sender_exec_i::iterations (::CORBA::ULong iterations)
  {
    this->iterations_ = iterations;
  }
  
  char *
  Sender_exec_i::message (void)
  {
    return CORBA::string_dup (this->msg_.c_str());
  }
  void
  Sender_exec_i::message (const char *msg)
  {
    this->msg_ = msg;
  }

  ::CORBA::Boolean
  Sender_exec_i::log_time (void)
  {
    return this->log_time_;
  }

  void
  Sender_exec_i::log_time (::CORBA::Boolean log_time)
  {
    this->log_time_ = log_time;
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
    this->writer_ = this->context_->get_connection_info_in_data ();
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

