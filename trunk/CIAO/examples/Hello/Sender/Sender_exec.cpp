//$Id$

#include "Sender_exec.h"

namespace CIDL_Sender_Impl
{
  char*
  Message_Impl::get_message (void)
    ACE_THROW_SPEC ((CORBA::SystemException))
  {
    ACE_DEBUG ((LM_DEBUG, "Sender sending out message: [%s]\n", component_.message_.in ()));
    return CORBA::string_dup (component_.message_.in ());
  }

  Sender_exec_i::~Sender_exec_i ()
  {
  }

  void
  Sender_exec_i::local_message (const char * local_message
                                ACE_ENV_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException))
  {
    message_ = CORBA::string_dup (local_message);
  }

  char *
  Sender_exec_i::local_message (void)
    ACE_THROW_SPEC ((CORBA::SystemException))
  {
    return CORBA::string_dup (message_.in ());
  }

  void
  Sender_exec_i::hertz (CORBA::Long hertz
                        ACE_ENV_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException))
  {
    this->hertz_ = hertz;
    ACE_DEBUG ((LM_DEBUG, "Sender_exec_i::Hertz:%d\n", this->hertz_));
  }

  CORBA::Long
  Sender_exec_i::hertz (void)
    ACE_THROW_SPEC ((CORBA::SystemException))
  {
    return this->hertz_;
  }

  ::Hello::COLOR_SELECTION
  Sender_exec_i::color (
  ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
  {
    // Your code here.
    return this->color_;
  }

  void
  Sender_exec_i::color (
  ::Hello::COLOR_SELECTION color
  ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
  {
    this->color_ = color;
  }

  Hello::CCM_ReadMessage_ptr
  Sender_exec_i::get_push_message (void)
        ACE_THROW_SPEC ((CORBA::SystemException))
  {
    ACE_DEBUG ((LM_DEBUG,
                "Sender_exec.i::get_push_message called\n "));
    return ( new Message_Impl (*this) );
  }

  void
  Sender_exec_i::start (void)
    ACE_THROW_SPEC ((CORBA::SystemException))
  {
    Hello::TimeOut_var event = new OBV_Hello::TimeOut;
    event->data ("ACE/TAO/CIAO");
    ACE_DEBUG ((LM_DEBUG, "Sender initiates the process.\n"));
    this->context_->push_click_out (event.in () ACE_ENV_ARG_PARAMETER);

    ACE_DEBUG ((LM_DEBUG, "My current color is:"));

    switch (this->color_)
      {
      case ::Hello::empty:
        ACE_DEBUG ((LM_DEBUG, "ERROR: Enum attribute initialization failed.\n"));
        break;

      case ::Hello::white:
        ACE_DEBUG ((LM_DEBUG, "white\n"));
        break;

      case ::Hello::red:
        ACE_DEBUG ((LM_DEBUG, "red\n"));
        break;

      case ::Hello::yellow:
        ACE_DEBUG ((LM_DEBUG, "yellow\n"));
        break;

      default:
        ACE_DEBUG ((LM_DEBUG, "Unknown color!\n"));

      }
  }

  void
  Sender_exec_i::set_session_context (Components::SessionContext_ptr ctx
                                      ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                    Components::CCMException))
  {
    ACE_DEBUG ((LM_DEBUG, "Sender_exec_i::set_session_context\n"));

    this->context_ =
          Sender_Exec_Context::_narrow (ctx
                                        ACE_ENV_ARG_PARAMETER);

    if (CORBA::is_nil (this->context_.in ()))
      {
        ACE_THROW (CORBA::INTERNAL ());
      }
  }

  void
  Sender_exec_i::ciao_preactivate (void)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     Components::CCMException))
  {
    ACE_DEBUG ((LM_DEBUG,
                "Sender_exec_i::ciao_preactivate\n"));
  }

  void
  Sender_exec_i::ccm_activate (void)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     Components::CCMException))
  {
    ACE_DEBUG ((LM_DEBUG,
                "Sender_exec_i::ccm_activate\n"));
  }

  void
  Sender_exec_i::ciao_postactivate (void)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     Components::CCMException))
  {
    ACE_DEBUG ((LM_DEBUG,
                "Sender_exec_i::ciao_postactivate\n"));
  }

  void
  Sender_exec_i::ccm_passivate (void)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     Components::CCMException))
  {
    ACE_DEBUG ((LM_DEBUG, "Sender_exec_i::ccm_passivate\n"));
  }

  void
  Sender_exec_i::ccm_remove (void)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     Components::CCMException))
  {
    ACE_DEBUG ((LM_DEBUG, "Sender_exec_i::ccm_remove\n"));
  }

  SenderHome_exec_i::SenderHome_exec_i ()
  {
  }

  SenderHome_exec_i::~SenderHome_exec_i ()
  {
  }

  ::Components::EnterpriseComponent_ptr
  SenderHome_exec_i::create (void)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     Components::CCMException))
  {
    ACE_DEBUG ((LM_DEBUG, "(%P|%t) creating SenderHome \n"));
    return new Sender_exec_i;
  }

  extern "C" SENDER_EXEC_Export ::Components::HomeExecutorBase_ptr
  createSenderHome_Impl (void)
  {
    return new SenderHome_exec_i ();
  }
}

