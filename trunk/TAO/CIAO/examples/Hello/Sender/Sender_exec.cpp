$Id$:
/*
 * @file Sender_exec.cpp
 *
 * @author Tao Lu <lu@dre.vanderbilt.edu>
 */

#include "Sender_exec.h"

MyImpl::Sender_exec_i::Sender_exec_i ()
  : message_ ("Sender's Data")
{
}

MyImpl::Sender_exec_i::~Sender_exec_i ()
{
}

Hello::CCM_message_ptr
MyImpl::Sender_exec_i::get_push_message (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException))
{
  return Hello::CCM_message::_duplicate (this);
}

char *
MyImpl::Sender_exec_i::get_message (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_DEBUG ((LM_DEBUG,
              "Sender sending out message. \n"));
  return CORBA::string_dup(this->message_.in ());
}


void
MyImpl::Sender_exec_i::start (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  Hello::timeout_var event = new OBV_Hello::timeout;

  ACE_DEBUG ((LM_DEBUG,
              "Sender initiates the process.\n"));

  this->context_->push_click_out (event
                                       ACE_ENV_ARG_PARAMETER);
  return;
}


// Operations from Components::SessionComponen
void
MyImpl::Sender_exec_i::set_session_context (Components::SessionContext_ptr ctx
                                                ACE_ENV_ARG_DECL_WITH_DEFAULTS)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
  ACE_DEBUG ((LM_DEBUG, "MyImpl::Sender_exec_i::set_session_context\n"));

  this->context_ =
    Hello::CCM_Sender_Context::_narrow (ctx
                                             ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (CORBA::is_nil (this->context_.in ()))
    ACE_THROW (CORBA::INTERNAL ());
  // Urm, we actually discard exceptions thown from this operation.
}

void
MyImpl::Sender_exec_i::ccm_activate (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
  ACE_DEBUG ((LM_DEBUG, "MyImpl::Sender_exec_i::ccm_activate\n"));
}

void
MyImpl::Sender_exec_i::ccm_passivate (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
  ACE_DEBUG ((LM_DEBUG, "MyImpl::Sender_exec_i::ccm_passivate\n"));
}

void
MyImpl::Sender_exec_i::ccm_remove (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
  ACE_DEBUG ((LM_DEBUG, "MyImpl::Sender_exec_i::ccm_remove\n"));
}


MyImpl::SenderHome_exec_i::SenderHome_exec_i ()
{
}

MyImpl::SenderHome_exec_i::~SenderHome_exec_i ()
{
}

::Components::EnterpriseComponent_ptr
MyImpl::SenderHome_exec_i::create (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
  return new MyImpl::Sender_exec_i;
}


extern "C" SENDER_EXEC_Export ::Components::HomeExecutorBase_ptr
createSenderHome_Impl (void)
{
  return new MyImpl::SenderHome_exec_i ();
}
