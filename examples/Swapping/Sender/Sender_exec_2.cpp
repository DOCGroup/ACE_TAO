//$Id$

#include "Sender_exec_2.h"
#include "Sender_exec.h"

namespace CIDL_Sender_Impl
{
  char*
  Message_Impl_2::get_message ()
  {
    ACE_DEBUG ((LM_DEBUG, "Sender 2 sending out message. \n"));
    return CORBA::string_dup (component_.message_.in ());
  }

  Sender_exec_2_i::~Sender_exec_2_i ()
  {
  }

  void
  Sender_exec_2_i::local_message (const char * local_message)
  {
    message_ = CORBA::string_dup (local_message);
  }

  char *
  Sender_exec_2_i::local_message ()
  {
    return CORBA::string_dup(message_.in ());
  }

  Hello::CCM_ReadMessage_ptr
  Sender_exec_2_i::get_push_message ()
  {
    ACE_DEBUG ((LM_DEBUG,
                "Sender_exec_2_i::get_push_message called\n"));
    return ( new Message_Impl_2 (*this) );
  }

  void
  Sender_exec_2_i::start ()
  {
    Hello::TimeOut_var event = new OBV_Hello::TimeOut;
    ACE_DEBUG ((LM_DEBUG, "Sender 2 initiates the process.\n"));
    this->context_->push_click_out (event);
  }

  void
  Sender_exec_2_i::set_session_context (Components::SessionContext_ptr ctx)
  {
    ACE_DEBUG ((LM_DEBUG, "Sender_exec_2_i::set_session_context\n"));

    this->context_ =
          Sender_Exec_Context::_narrow (ctx);

    if (CORBA::is_nil (this->context_.in ()))
      {
        throw CORBA::INTERNAL ();
      }
  }

  void
  Sender_exec_2_i::ciao_preactivate ()
  {
    ACE_DEBUG ((LM_DEBUG,
                "Sender_exec_2_i::ciao_preactivate\n"));
  }

  void
  Sender_exec_2_i::ccm_activate ()
  {
    ACE_DEBUG ((LM_DEBUG,
                "Sender_exec_2_i::ccm_activate\n"));

    ::Components::ConsumerDescriptions_var c =
        this->base_exec_->consumers ();

    if (CORBA::is_nil (this->context_.in ()))
      {
        throw CORBA::INTERNAL ();
      }

    CORBA::Object_var o =
      this->context_->get_CCM_object ();

    Hello::Sender_var sender =
      Hello::Sender::_narrow (o.in ());

    for (CORBA::ULong cnt = 0; cnt != c->length (); ++cnt)
      {
        sender->subscribe ("click_out",
                           (*c)[cnt]->consumer ());
      }
  }

  void
  Sender_exec_2_i::ciao_postactivate ()
  {
    ACE_DEBUG ((LM_DEBUG,
                "Sender_exec_2_i::ciao_postactivate\n"));
  }

  void
  Sender_exec_2_i::ccm_passivate ()
  {
    ACE_DEBUG ((LM_DEBUG, "Sender_exec_2_i::ccm_passivate\n"));
  }

  void
  Sender_exec_2_i::ccm_remove ()
  {
    ACE_DEBUG ((LM_DEBUG, "Sender_exec_2_i::ccm_remove\n"));
  }

  extern "C" SENDER_EXEC_2_Export ::Components::EnterpriseComponent_ptr
  createSenderExec_Impl (SenderSwap_exec_i *p)
  {
    Sender_exec_2_i *tmp =
      new Sender_exec_2_i ();

    tmp->swap_exec (p);

    return tmp;
  }
}

