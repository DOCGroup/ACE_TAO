// $Id$

#include "Receiver_exec.h"

void
ReceiverImpl::test_i::method (void)
  throw (CORBA::SystemException)
{
  ACE_DEBUG ((LM_DEBUG, "ReceiverImpl::test_i::method\n"));
}

ReceiverImpl::ReceiverExec_i::ReceiverExec_i (void)
{
  ACE_DEBUG ((LM_DEBUG, "ReceiverImpl::ReceiverExec_i::ReceiverExec_i\n"));

  int argc = 0;
  char **argv = 0;

  this->orb_ =
    CORBA::ORB_init (argc,
                     argv,
                     "");

}

::DiffServ::CCM_test_ptr
ReceiverImpl::ReceiverExec_i::get_writer (void)
  throw (CORBA::SystemException)
{
  ACE_DEBUG ((LM_DEBUG, "ReceiverImpl::ReceiverExec_i::get_writer\n"));
  return new test_i;
}

void
ReceiverImpl::ReceiverExec_i::shutdown (void)
  throw (CORBA::SystemException)
{
  ACE_DEBUG ((LM_DEBUG, "ReceiverImpl::ReceiverExec_i::shutdown\n"));
  this->orb_->shutdown ();
}

void
ReceiverImpl::ReceiverExec_i::set_session_context (Components::SessionContext_ptr ctx)
  throw (CORBA::SystemException,
         Components::CCMException)
{
  ACE_DEBUG ((LM_DEBUG, "ReceiverImpl::ReceiverExec_i::set_session_context\n"));

  this->context_ =
    ReceiverImpl::ReceiverExec_Context::_narrow (ctx);

  if (CORBA::is_nil (this->context_.in ()))
    throw CORBA::INTERNAL ();
}

void
ReceiverImpl::ReceiverExec_i::ccm_activate (void)
  throw (CORBA::SystemException,
         Components::CCMException)
{
  ACE_DEBUG ((LM_DEBUG, "ReceiverImpl::ReceiverExec_i::ccm_activate\n"));
}

void
ReceiverImpl::ReceiverExec_i::ccm_passivate (void)
  throw (CORBA::SystemException,
         Components::CCMException)
{
  ACE_DEBUG ((LM_DEBUG, "ReceiverImpl::ReceiverExec_i::ccm_passivate\n"));
}

void
ReceiverImpl::ReceiverExec_i::ccm_remove (void)
  throw (CORBA::SystemException,
         Components::CCMException)
{
  ACE_DEBUG ((LM_DEBUG, "ReceiverImpl::ReceiverExec_i::ccm_remove\n"));
}

void
ReceiverImpl::ReceiverExec_i::ciao_preactivate (void)
  throw (CORBA::SystemException,
         Components::CCMException)
{
  ACE_DEBUG ((LM_DEBUG, "ReceiverImpl::ReceiverExec_i::ccm_preactivate\n"));
}

void
ReceiverImpl::ReceiverExec_i::ciao_postactivate (void)
  throw (CORBA::SystemException,
         Components::CCMException)
{
  ACE_DEBUG ((LM_DEBUG, "ReceiverImpl::ReceiverExec_i::ccm_postactivate\n"));
}

::Components::EnterpriseComponent_ptr
ReceiverImpl::ReceiverHomeExec_i::create (void)
  throw (CORBA::SystemException,
         Components::CCMException)
{
  ACE_DEBUG ((LM_DEBUG, "ReceiverImpl::ReceiverHome_exec::create\n"));
  return new ReceiverImpl::ReceiverExec_i;
}

extern "C" RECEIVER_EXEC_Export ::Components::HomeExecutorBase_ptr
createReceiverHome_Impl (void)
{
  ACE_DEBUG ((LM_DEBUG, "createReceiverHome_Impl\n"));
  return new ReceiverImpl::ReceiverHomeExec_i;
}
