//$Id$

#include "Sender_exec_1.h"
#include "Sender_exec.h"

char*
Sender_Impl::Message_Impl_1::get_message (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_DEBUG ((LM_DEBUG, "Sender sending out message. \n"));
  return CORBA::string_dup (component_.message_.in ());
}

Sender_Impl::Sender_exec_1_i::~Sender_exec_1_i ()
{
}

void
Sender_Impl::Sender_exec_1_i::local_message (const char * local_message
                                           ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  message_ = CORBA::string_dup (local_message);
}

char *
Sender_Impl::Sender_exec_1_i::local_message (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return CORBA::string_dup(message_);
}

Hello::CCM_ReadMessage_ptr
Sender_Impl::Sender_exec_1_i::get_push_message (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_DEBUG ((LM_DEBUG,
              "Sender_Impl::Sender_exec_1_i::get_push_message called\n"));
  return ( new Message_Impl_1 (*this) );
}

void
Sender_Impl::Sender_exec_1_i::start (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  Hello::TimeOut_var event = new OBV_Hello::TimeOut;
  ACE_DEBUG ((LM_DEBUG, "Sender initiates the process.\n"));
  this->context_->push_click_out (event ACE_ENV_ARG_PARAMETER);
}

void
Sender_Impl::Sender_exec_1_i::set_session_context
   (Components::SessionContext_ptr ctx
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
  ACE_DEBUG ((LM_DEBUG, "Sender_Impl::Sender_exec_1_i::set_session_context\n"));

  this->context_ =
        Sender_Impl::Sender_Exec_Context::_narrow (ctx
                                                   ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (CORBA::is_nil (this->context_.in ()))
       ACE_THROW (CORBA::INTERNAL ());
}

void
Sender_Impl::Sender_exec_1_i::ciao_preactivate (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
  ACE_DEBUG ((LM_DEBUG,
              "Sender_Impl::Sender_exec_1_i::ciao_preactivate\n"));
}

void
Sender_Impl::Sender_exec_1_i::ccm_activate (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
  ACE_DEBUG ((LM_DEBUG,
              "Sender_Impl::Sender_exec_1_i::ccm_activate\n"));

  CORBA::Object_var o =
    this->context_->get_CCM_object (ACE_ENV_SINGLE_ARG_PARAMETER);
}

void
Sender_Impl::Sender_exec_1_i::ciao_postactivate (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
  ACE_DEBUG ((LM_DEBUG,
              "Sender_Impl::Sender_exec_1_i::ciao_postactivate\n"));
}

void
Sender_Impl::Sender_exec_1_i::ccm_passivate (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
  ACE_DEBUG ((LM_DEBUG, "Sender_Impl::Sender_exec_1_i::ccm_passivate\n"));
  CORBA::Object_var comp_object =
    this->context_->get_CCM_object (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);
  Hello::Sender_var sender = Hello::Sender::_narrow (comp_object.in ()
                                                    ACE_ENV_ARG_PARAMETER);

  Components::ConsumerDescriptions_var retval =
    this->context_->get_registered_consumer ("click_out"
       ACE_ENV_ARG_PARAMETER);
  ACE_DEBUG ((LM_DEBUG, "length of consumers is %d\n", retval->length ()));
  this->base_exec_->consumers (retval._retn ());
  /*
  ::Components::ConsumerDescriptions_var cons_desc =
    sender->get_all_consumers (ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_CHECK_RETURN (0);

  ACE_DEBUG ((LM_DEBUG, "length of consumers is %d\n", cons_desc->length ()));
  */
  // this->base_exec_->consumers (cons_desc._retn ());
}

void
Sender_Impl::Sender_exec_1_i::ccm_remove (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
  ACE_DEBUG ((LM_DEBUG, "Sender_Impl::Sender_exec_1_i::ccm_remove\n"));
}

extern "C" SENDER_EXEC_Export ::Components::EnterpriseComponent_ptr
createSenderExec_Impl (Sender_Impl::SenderSwap_exec_i *p)
{
  Sender_Impl::Sender_exec_1_i *tmp =
    new Sender_Impl::Sender_exec_1_i ();

  tmp->swap_exec (p);

  return tmp;
}
