// $Id$

#include "Controller_exec.h"

/// Default constructor.
MyImpl::Controller_exec_i::Controller_exec_i ()
{
}

/// Default destructor.
MyImpl::Controller_exec_i::~Controller_exec_i ()
{
}

// Operations from Components::SessionComponent
void
MyImpl::Controller_exec_i::set_session_context (Components::SessionContext_ptr ctx
                                                ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
  ACE_DEBUG ((LM_DEBUG, "MyImpl::Controller_exec_i::set_session_context\n"));

  this->context_ =
    Priority_Test::CCM_Controller_Context::_narrow (ctx
                                                    ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (CORBA::is_nil (this->context_.in ()))
    ACE_THROW (CORBA::INTERNAL ());
  // Urm, we actually discard exceptions thown from this operation.
}

void
MyImpl::Controller_exec_i::ccm_activate (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
  ACE_DEBUG ((LM_DEBUG, "MyImpl::Controller_exec_i::ccm_activate\n"));
}

void
MyImpl::Controller_exec_i::ccm_passivate (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
  ACE_DEBUG ((LM_DEBUG, "MyImpl::Controller_exec_i::ccm_passivate\n"));
}

void
MyImpl::Controller_exec_i::ccm_remove (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
  ACE_DEBUG ((LM_DEBUG, "MyImpl::Controller_exec_i::ccm_remove\n"));
}

/// Default ctor.
MyImpl::Controller_Home_exec_i::Controller_Home_exec_i ()
{
}

/// Default dtor.
MyImpl::Controller_Home_exec_i::~Controller_Home_exec_i ()
{
}

// Explicit home operations.

// Implicit home operations.

::Components::EnterpriseComponent_ptr
MyImpl::Controller_Home_exec_i::create (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
  return new MyImpl::Controller_exec_i;
}


extern "C" CONTROLLER_EXEC_Export ::Components::HomeExecutorBase_ptr
createControllerHome_Impl (void)
{
  return new MyImpl::Controller_Home_exec_i;
}
