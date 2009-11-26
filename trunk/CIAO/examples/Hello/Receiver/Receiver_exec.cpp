// $Id$

#include "Receiver_exec.h"
#include "ace/High_Res_Timer.h"
#include "ace/Date_Time.h"
#include "ace/OS_NS_unistd.h"
#include "tao/ORB_Core.h"

namespace CIAO_Hello_Receiver_Impl
{
  read_action_Generator::read_action_Generator (Receiver_exec_i &callback)
    : pulse_callback_ (callback)
  {
  }

  read_action_Generator::~read_action_Generator ()
  {
  }

  int
  read_action_Generator::handle_timeout (const ACE_Time_Value &, const void *)
  {
    this->pulse_callback_.tick ();
    return 0;
  }

  
  Receiver_exec_i::Receiver_exec_i ()
    : iterations_ (10),
      iteration_ (0),
      rate_ (1)
  {
    this->ticker_ = new read_action_Generator (*this);
  }

  Receiver_exec_i::~Receiver_exec_i ()
  {
  }

  void
  Receiver_exec_i::iterations (CORBA::Short iterations)
  {
    this->iterations_ = iterations;
  }

  CORBA::Short
  Receiver_exec_i::iterations ()
  {
    return this->iterations_;
  }

  void
  Receiver_exec_i::rate (CORBA::Short rate)
  {
    this->rate_ = rate;
  }

  CORBA::Short
  Receiver_exec_i::rate ()
  {
    return this->rate_;
  }

  void
  Receiver_exec_i::tick ()
  {
    if (this->iteration_ < this->iterations_)
      {
        ++this->iteration_;
        CORBA::String_var str = this->rev_->get_message ();
        ACE_hrtime_t now = ACE_OS::gethrtime();
        ACE_CString tm_rec (str.in ());
        ACE_hrtime_t received = ACE_OS::strtoull (tm_rec.substr(0, 15).c_str(), 0, 0);
        if (received > 0)
          {
            ACE_Time_Value tv (0, 0);
            ACE_High_Res_Timer::hrtime_to_tv (tv,
                                              now - received);
            ACE_DEBUG ((LM_DEBUG,
                      "Receiver - Got message from the sender [%C] - timing difference <%d>\n",
                      str.in (), tv.usec ()));
          }
        else
          {
            ACE_DEBUG ((LM_DEBUG,
                      "Receiver - Got message from the sender [%C].\n",
                      str.in ()));
          }
      }
  }
  
  void
  Receiver_exec_i::push_click_in (::Hello::TimeOut * ev)
  {
    //Get the message from the Sender first.
    ACE_DEBUG ((LM_DEBUG,
                "Receiver - Informed by the Sender with message [%C]\n",
                ev->data ()));
    // scheduling a timer, using the orb reactor thread, using the 
    // read_action_Generator as it's callback
    // that way, one don't have to create it's own thread (although allowed)
    // and CCM isn't blocked in this method.

    // calculate the interval time
    long usec = 1000000 / this->rate_;
    if (this->context_->get_CCM_object()->_get_orb ()->orb_core ()->reactor ()->schedule_timer (
                this->ticker_,
                0,
                ACE_Time_Value (0, usec),
                ACE_Time_Value (0, usec)) == -1)
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("Receiver_exec_i::start : ")
                             ACE_TEXT ("Error scheduling timer")));
    }
  }

  // Operations from Components::SessionComponen
  void
  Receiver_exec_i::set_session_context (::Components::SessionContext_ptr ctx)
  {
    ACE_DEBUG ((LM_DEBUG, "Receiver_exec_i::set_session_context\n"));

    this->context_ =
      ::Hello::CCM_Receiver_Context::_narrow (ctx);

    if (CORBA::is_nil (this->context_.in ()))
    {
      throw ::CORBA::INTERNAL ();
    }
  }

  void
  Receiver_exec_i::configuration_complete ()
  {
    ACE_DEBUG ((LM_DEBUG,
                "Receiver_exec_i::configuration_complete\n"));
  }

  void
  Receiver_exec_i::ccm_activate ()
  {
    ACE_DEBUG ((LM_DEBUG, "Receiver_exec_i::ccm_activate\n"));
    // get the connection to the sender, providing the 
    // read_message interface
    this->rev_ = this->context_->get_connection_read_message();
    if (CORBA::is_nil (this->rev_.in ()))
      throw CORBA::BAD_INV_ORDER ();
  }

  void
  Receiver_exec_i::ccm_passivate ()
  {
    ACE_DEBUG ((LM_DEBUG, "Receiver_exec_i::ccm_passivate\n"));
  }

  void
  Receiver_exec_i::ccm_remove ()
  {
    ACE_DEBUG ((LM_DEBUG, "Receiver_exec_i::ccm_remove\n"));
    this->context_->get_CCM_object()->_get_orb ()->orb_core ()->reactor ()->cancel_timer (this->ticker_);
    delete this->ticker_;
  }


  ReceiverHome_exec_i::ReceiverHome_exec_i ()
  {
  }

  ReceiverHome_exec_i::~ReceiverHome_exec_i ()
  {
  }

  ::Components::EnterpriseComponent_ptr
  ReceiverHome_exec_i::create ()
  {
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_THROW_EX (
      retval,
      Receiver_exec_i,
      ::CORBA::NO_MEMORY ());

    return retval;
  }

  extern "C" RECEIVER_EXEC_Export ::Components::HomeExecutorBase_ptr
  create_Hello_ReceiverHome_Impl (void)
  {
    ::Components::HomeExecutorBase_ptr retval =
      ::Components::HomeExecutorBase::_nil ();

    ACE_NEW_RETURN (
      retval,
      ReceiverHome_exec_i,
      ::Components::HomeExecutorBase::_nil ());

    return retval;
  }
}

