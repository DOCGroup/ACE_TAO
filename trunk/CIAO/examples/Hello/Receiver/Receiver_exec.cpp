// $Id$

#include "Receiver_exec.h"
#include "ace/OS_NS_unistd.h"

namespace CIAO_Hello_Receiver_Impl
{
  Receiver_exec_i::Receiver_exec_i ()
    : iterations_ (10)
  {
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
  Receiver_exec_i::push_click_in (::Hello::TimeOut * ev)
  {
    ACE_DEBUG ((LM_DEBUG,
          ACE_TEXT ("Receiver - Informed by the Sender with message [%C]\n"),
          ev->data ()));
    Hello::ReadMessage_var rev = this->context_->get_connection_read_message();
    if (CORBA::is_nil (rev.in ()))
      throw CORBA::BAD_INV_ORDER ();
    for (CORBA::Short i = 0; i < this->iterations_; ++i)
      {
        CORBA::String_var str = rev->get_message ();
        ACE_DEBUG ((LM_DEBUG,
                  "Receiver - Got message from the sender [%C].\n",
                  str.in ()));
        ACE_Time_Value tv (1, 0);
        ACE_OS::sleep (tv);
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

