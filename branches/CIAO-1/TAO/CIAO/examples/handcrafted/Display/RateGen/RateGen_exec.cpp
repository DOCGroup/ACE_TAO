// $Id$

#include "RateGen_exec.h"

MyImpl::RateGen_exec_impl::RateGen_exec_impl ()
  : active_ (0),
    hertz_ (0)
{

}

MyImpl::RateGen_exec_impl::RateGen_exec_impl (CORBA::Long hz)
  : active_ (0),
    hertz_ (hz)
{
}

MyImpl::RateGen_exec_impl::~RateGen_exec_impl ()
{
}

CORBA::Long
MyImpl::RateGen_exec_impl::hertz (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->hertz_;
}

void
MyImpl::RateGen_exec_impl::hertz (CORBA::Long hertz
                                  ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->hertz_ = hertz;
}

// Operations from supported interface(s)

void
MyImpl::RateGen_exec_impl::start (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  if (this->hertz_ == 0 || this->active_)
    ACE_THROW (CORBA::BAD_INV_ORDER ());

  // @@ Start the rate generator

}

void
MyImpl::RateGen_exec_impl::stop (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  if (! this->active_)
    ACE_THROW (CORBA::BAD_INV_ORDER ());

  // @@ stop the rate generator
}

CORBA::Boolean
MyImpl::RateGen_exec_impl::active (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->active_ != 0;
}

// Operations from Components::SessionComponent

void
MyImpl::RateGen_exec_impl::set_session_context (Components::SessionContext_ptr ctx
                                                ACE_ENV_ARG_DECL_WITH_DEFAULTS)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
  ACE_DEBUG ((LM_DEBUG, "MyImpl::RateGen_exec_impl::set_session_context\n"));

  this->context_ =
    HUDisplay::CCM_RateGen_Context::_narrow (ctx
                                             ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (CORBA::is_nil (this->context_.in ()))
    ACE_THROW (CORBA::INTERNAL ());
  // Urm, we actually discard exceptions thown from this operation.

}

void
MyImpl::RateGen_exec_impl::ccm_activate (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
  ACE_DEBUG ((LM_DEBUG, "MyImpl::RateGen_exec_impl::ccm_activate\n"));
}

void
MyImpl::RateGen_exec_impl::ccm_passivate (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
  ACE_DEBUG ((LM_DEBUG, "MyImpl::RateGen_exec_impl::ccm_passivate\n"));
}

void
MyImpl::RateGen_exec_impl::ccm_remove (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
  ACE_DEBUG ((LM_DEBUG, "MyImpl::RateGen_exec_impl::ccm_remove\n"));
}

MyImpl::RateGenHome_exec_impl::RateGenHome_exec_impl ()
{
}

MyImpl::RateGenHome_exec_impl::~RateGenHome_exec_impl ()
{
}

::Components::EnterpriseComponent_ptr
MyImpl::RateGenHome_exec_impl::new_RateGen (CORBA::Long hertz
                                            ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CreateFailure))
{
  return new MyImpl::RateGen_exec_impl (hertz);
}

::Components::EnterpriseComponent_ptr
MyImpl::RateGenHome_exec_impl::create (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CreateFailure))
{
  return new MyImpl::RateGen_exec_impl ();
}


extern "C" RATEGEN_EXEC_Export ::Components::HomeExecutorBase_ptr
createRateGenHome_Impl (void)
{
  return new MyImpl::RateGenHome_exec_impl ();
}
