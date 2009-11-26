// $Id$

#include "Sender_exec.h"
#include "ace/Date_Time.h"
#include "ace/OS_NS_time.h"
#include "ace/High_Res_Timer.h"

namespace CIAO_Hello_Sender_Impl
{
  char*
  Message_Impl::get_message ()
  {
    CORBA::String_var message;
    if (this->component_.log_time ())
      {
        //Add extra time
        ACE_hrtime_t start = ACE_OS::gethrtime();
        char timestamp[16];
        ACE_OS::sprintf (timestamp,
                          "%lld",
                          start);
        CORBA::String_var tm = CORBA::string_dup(timestamp);
        int tm_length = strlen(tm);
        int message_length = strlen(this->component_.message_.in ());
        //one extra for a space character.
        message = CORBA::string_alloc( (tm_length + message_length + 2));
        strcpy(message, tm.in ());
        strcat(message, " ");
        strcat(message, this->component_.message_.in ());
        ACE_DEBUG ((LM_EMERGENCY, "Sender returning message: [%C]\n", component_.message_.in ()));
        return CORBA::string_dup(message);
      }
    else
      {
        ACE_DEBUG ((LM_EMERGENCY, "Sender returning message: [%C]\n", component_.message_.in ()));
        return CORBA::string_dup (message);
      }
  }

  Sender_exec_i::~Sender_exec_i ()
  {
  }

  void
  Sender_exec_i::local_message (const char * local_message)
  {
    message_ = CORBA::string_dup (local_message);
  }

  char *
  Sender_exec_i::local_message ()
  {
    return CORBA::string_dup (message_.in ());
  }

  void
  Sender_exec_i::log_time (bool log_time)
  {
    log_time_ = log_time;
  }

  bool
  Sender_exec_i::log_time ()
  {
    return log_time_;
  }

  ::Hello::COLOR_SELECTION
  Sender_exec_i::color ()
  {
    // Your code here.
    return this->color_;
  }

  void
  Sender_exec_i::color (::Hello::COLOR_SELECTION color)
  {
    this->color_ = color;
  }

  Hello::CCM_ReadMessage_ptr
  Sender_exec_i::get_push_message ()
  {
    ACE_DEBUG ((LM_EMERGENCY,
                "Sender_exec.i::get_push_message called\n"));
    return ( new Message_Impl (*this) );
  }

  void
  Sender_exec_i::start ()
  {
    Hello::TimeOut_var event = new OBV_Hello::TimeOut;
    event->data ("ACE/TAO/CIAO");
    ACE_DEBUG ((LM_EMERGENCY, "Sender initiates the process.\n"));
    this->context_->push_click_out (event.in ());

    ACE_DEBUG ((LM_EMERGENCY, "My current color is:"));

    switch (this->color_)
      {
      case ::Hello::empty:
        ACE_DEBUG ((LM_EMERGENCY, "ERROR: Enum attribute initialization failed.\n"));
        break;

      case ::Hello::white:
        ACE_DEBUG ((LM_EMERGENCY, "white\n"));
        break;

      case ::Hello::red:
        ACE_DEBUG ((LM_EMERGENCY, "red\n"));
        break;

      case ::Hello::yellow:
        ACE_DEBUG ((LM_EMERGENCY, "yellow\n"));
        break;

      default:
        ACE_DEBUG ((LM_EMERGENCY, "Unknown color!\n"));

      }
  }

  void
  Sender_exec_i::set_session_context (::Components::SessionContext_ptr ctx)
  {
    ACE_DEBUG ((LM_EMERGENCY, "Sender_exec_i::set_session_context\n"));

    this->context_ =
      ::Hello::CCM_Sender_Context::_narrow (ctx);

    if (CORBA::is_nil (this->context_.in ()))
    {
      throw ::CORBA::INTERNAL ();
    }
  }

  void
  Sender_exec_i::configuration_complete ()
  {
    ACE_DEBUG ((LM_EMERGENCY,
                "Sender_exec_i::configuration_complete\n"));
  }

  void
  Sender_exec_i::ccm_activate ()
  {
    ACE_DEBUG ((LM_EMERGENCY,
                "Sender_exec_i::ccm_activate\n"));
  }

  void
  Sender_exec_i::ccm_passivate ()
  {
    ACE_DEBUG ((LM_EMERGENCY, "Sender_exec_i::ccm_passivate\n"));
  }

  void
  Sender_exec_i::ccm_remove ()
  {
    ACE_DEBUG ((LM_EMERGENCY, "Sender_exec_i::ccm_remove\n"));
  }

  SenderHome_exec_i::SenderHome_exec_i ()
  {
  }

  SenderHome_exec_i::~SenderHome_exec_i ()
  {
  }

  ::Components::EnterpriseComponent_ptr
  SenderHome_exec_i::create ()
  {
    ACE_DEBUG ((LM_EMERGENCY, "(%P|%t) creating SenderHome\n"));

    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_THROW_EX (
      retval,
      Sender_exec_i,
      ::CORBA::NO_MEMORY ());

    return retval;
  }

  extern "C" SENDER_EXEC_Export ::Components::HomeExecutorBase_ptr
  create_Hello_SenderHome_Impl (void)
  {
    ::Components::HomeExecutorBase_ptr retval =
      ::Components::HomeExecutorBase::_nil ();

    ACE_NEW_RETURN (
      retval,
      SenderHome_exec_i,
      ::Components::HomeExecutorBase::_nil ());

    return retval;
  }
}

