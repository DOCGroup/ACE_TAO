//$Id$

#include "Sender_exec.h"

char*
Sender_Impl::Message_Impl::get_message (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_DEBUG ((LM_DEBUG, "Sender sending out message. \n"));
  return CORBA::string_dup (component_.message_.in ());
}

Sender_Impl::Sender_exec_i::~Sender_exec_i ()
{
}

void
Sender_Impl::Sender_exec_i::local_message (const char * local_message
                                           ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  message_ = CORBA::string_dup (local_message);
}

char *
Sender_Impl::Sender_exec_i::local_message (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return CORBA::string_dup(message_);
}

Hello::CCM_ReadMessage_ptr
Sender_Impl::Sender_exec_i::get_push_message (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_DEBUG ((LM_DEBUG, 
              "Sender_Impl::Sender_exec.i::get_push_message called\n "));
  return ( new Message_Impl (*this) );
}

void
Sender_Impl::Sender_exec_i::start (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  Hello::TimeOut_var event = new OBV_Hello::TimeOut;
  ACE_DEBUG ((LM_DEBUG, "Sender initiates the process.\n"));
  this->context_->push_click_out (event ACE_ENV_ARG_PARAMETER);
}

void
Sender_Impl::Sender_exec_i::set_session_context 
   (Components::SessionContext_ptr ctx
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
  ACE_DEBUG ((LM_DEBUG, "Sender_Impl::Sender_exec_i::set_session_context\n"));

  this->context_ =
        Sender_Impl::Sender_Exec_Context::_narrow (ctx
                                                   ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (CORBA::is_nil (this->context_.in ()))
       ACE_THROW (CORBA::INTERNAL ());
}

void
Sender_Impl::Sender_exec_i::ciao_preactivate (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
  ACE_DEBUG ((LM_DEBUG,
              "Sender_Impl::Sender_exec_i::ciao_preactivate\n"));
}

void
Sender_Impl::Sender_exec_i::ccm_activate (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
  ACE_DEBUG ((LM_DEBUG,
              "Sender_Impl::Sender_exec_i::ccm_activate\n"));
}

void
Sender_Impl::Sender_exec_i::ciao_postactivate (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
  ACE_DEBUG ((LM_DEBUG,
              "Sender_Impl::Sender_exec_i::ciao_postactivate\n"));
}

void
Sender_Impl::Sender_exec_i::ccm_passivate (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
  ACE_DEBUG ((LM_DEBUG, "Sender_Impl::Sender_exec_i::ccm_passivate\n"));
}

void
Sender_Impl::Sender_exec_i::ccm_remove (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
  ACE_DEBUG ((LM_DEBUG, "Sender_Impl::Sender_exec_i::ccm_remove\n"));
}

Sender_Impl::SenderHome_exec_i::SenderHome_exec_i ()
{
}

Sender_Impl::SenderHome_exec_i::~SenderHome_exec_i ()
{
}

::Components::EnterpriseComponent_ptr
Sender_Impl::SenderHome_exec_i::create (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
  ACE_DEBUG ((LM_DEBUG, "%P|%t) creating SenderHome \n")); 
  return new Sender_Impl::Sender_exec_i;
}

extern "C" SENDER_EXEC_Export ::Components::HomeExecutorBase_ptr
createSenderHome_Impl (void)
{
  return new Sender_Impl::SenderHome_exec_i ();
}
