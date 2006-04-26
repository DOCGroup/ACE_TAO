//$Id$

#include "Sender_exec_2.h"
#include "Sender_exec.h"

namespace CIDL_Sender_Impl
{
  char*
  Message_Impl_2::get_message (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException))
  {
    ACE_DEBUG ((LM_DEBUG, "Sender 2 sending out message. \n"));
    return CORBA::string_dup (component_.message_.in ());
  }

  Sender_exec_2_i::~Sender_exec_2_i ()
  {
  }

  void
  Sender_exec_2_i::local_message (const char * local_message
                                  ACE_ENV_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException))
  {
    message_ = CORBA::string_dup (local_message);
  }

  char *
  Sender_exec_2_i::local_message (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException))
  {
    return CORBA::string_dup(message_.in ());
  }

  Hello::CCM_ReadMessage_ptr
  Sender_exec_2_i::get_push_message (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException))
  {
    ACE_DEBUG ((LM_DEBUG,
                "Sender_exec_2_i::get_push_message called\n"));
    return ( new Message_Impl_2 (*this) );
  }

  void
  Sender_exec_2_i::start (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
  {
    Hello::TimeOut_var event = new OBV_Hello::TimeOut;
    ACE_DEBUG ((LM_DEBUG, "Sender 2 initiates the process.\n"));
    this->context_->push_click_out (event ACE_ENV_ARG_PARAMETER);
    ACE_CHECK;
  }

  void
  Sender_exec_2_i::set_session_context (Components::SessionContext_ptr ctx
                                        ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                    Components::CCMException))
  {
    ACE_DEBUG ((LM_DEBUG, "Sender_exec_2_i::set_session_context\n"));

    this->context_ =
          Sender_Exec_Context::_narrow (ctx
                                        ACE_ENV_ARG_PARAMETER);
    ACE_CHECK;

    if (CORBA::is_nil (this->context_.in ()))
      {
        ACE_THROW (CORBA::INTERNAL ());
      }
  }

  void
  Sender_exec_2_i::ciao_preactivate (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException,
                    Components::CCMException))
  {
    ACE_DEBUG ((LM_DEBUG,
                "Sender_exec_2_i::ciao_preactivate\n"));
  }

  void
  Sender_exec_2_i::ccm_activate (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                    Components::CCMException))
  {
    ACE_DEBUG ((LM_DEBUG,
                "Sender_exec_2_i::ccm_activate\n"));

    ::Components::ConsumerDescriptions_var c =
        this->base_exec_->consumers ();

    if (CORBA::is_nil (this->context_.in ()))
      {
        ACE_THROW (CORBA::INTERNAL ());
      }

    CORBA::Object_var o =
      this->context_->get_CCM_object (ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_CHECK;

    Hello::Sender_var sender =
      Hello::Sender::_narrow (o.in ()
                              ACE_ENV_ARG_PARAMETER);
    ACE_CHECK;

    for (CORBA::ULong cnt = 0; cnt != c->length (); ++cnt)
      {
        sender->subscribe ("click_out",
                           (*c)[cnt]->consumer ()
                           ACE_ENV_ARG_PARAMETER);
        ACE_CHECK;
      }
  }

  void
  Sender_exec_2_i::ciao_postactivate (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException,
                    Components::CCMException))
  {
    ACE_DEBUG ((LM_DEBUG,
                "Sender_exec_2_i::ciao_postactivate\n"));
  }

  void
  Sender_exec_2_i::ccm_passivate (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException,
                    Components::CCMException))
  {
    ACE_DEBUG ((LM_DEBUG, "Sender_exec_2_i::ccm_passivate\n"));
  }

  void
  Sender_exec_2_i::ccm_remove (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException,
                    Components::CCMException))
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

