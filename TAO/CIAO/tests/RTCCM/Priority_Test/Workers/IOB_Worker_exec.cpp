// $Id$

#include "IOB_Worker_exec.h"

/// Default constructor.
MyImpl::IOB_Worker_exec::IOB_Worker_exec ()
{
}

/// Default destructor.
MyImpl::IOB_Worker_exec::~IOB_Worker_exec ()
{
}

// Operations from Priority_Test::Worker

HUDisplay::CCM_Common_Ops_ptr
MyImpl::IOB_Worker_exec::get_some_device (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return Priority_Test::CCM_Common_Ops::_duplicate (this);
}

// Operations from Priority_Test::Common_Ops

CORBA::ULong
MyImpl::IOB_Worker_exec::do_work (CORBA::ULong work,
                                 CORBA::ULong aux
                                 ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  CORBA::ULong retv = work - aux;

  ACE_DEBUG ((LM_DEBUG,
              "MyImpl::IOB_Worker_exec::do_work (%d, %d) = %d\n",
              work, aux, retv));
  return retv;
}

// Operations from Components::SessionComponent
void
MyImpl::IOB_Worker_exec::set_session_context (Components::SessionContext_ptr ctx
                                             ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
  ACE_DEBUG ((LM_DEBUG, "MyImpl::IOB_Worker_exec::set_session_context\n"));

  this->context_ =
    Priority_Test::CCM_Worker_Context::_narrow (ctx
                                                ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (CORBA::is_nil (this->context_.in ()))
    ACE_THROW (CORBA::INTERNAL ());
  // Urm, we actually discard exceptions thown from this operation.
}

void
MyImpl::IOB_Worker_exec::ccm_activate (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
  ACE_DEBUG ((LM_DEBUG, "MyImpl::IOB_Worker_exec::ccm_activate\n"));
}

void
MyImpl::IOB_Worker_exec::ccm_passivate (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
  ACE_DEBUG ((LM_DEBUG, "MyImpl::IOB_Worker_exec::ccm_passivate\n"));
}

void
MyImpl::IOB_Worker_exec::ccm_remove (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
  ACE_DEBUG ((LM_DEBUG, "MyImpl::IOB_Worker_exec::ccm_remove\n"));
}

/// Default ctor.
MyImpl::IOB_Worker_Home_exec::IOB_Worker_Home_exec ()
{
}

/// Default dtor.
MyImpl::IOB_Worker_Home_exec::~IOB_Worker_Home_exec ()
{
}

// Explicit home operations.

// Implicit home operations.

::Components::EnterpriseComponent_ptr
MyImpl::IOB_Worker_Home_exec::create (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
  return new MyImpl::IOB_Worker_exec;
}


extern "C" WORKER_EXEC_Export ::Components::HomeExecutorBase_ptr
createWorkerHome_Impl (void)
{
  return new MyImpl::IOB_Worker_Home_exec;
}
