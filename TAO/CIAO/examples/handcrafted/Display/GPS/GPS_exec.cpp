// $Id$

#include "GPS_exec.h"

#define DISPLACEMENT 256

/// Default constructor.
MyImpl::GPS_exec_impl::GPS_exec_impl ()
{
  ACE_OS::srand ((u_int) ACE_OS::time ());
  this->positionx_ = ACE_OS::rand ();
  this->positiony_ = ACE_OS::rand ();
}

/// Default destructor.
MyImpl::GPS_exec_impl::~GPS_exec_impl ()
{
}

// Operations from HUDisplay::GPS

HUDisplay::CCM_position_ptr
MyImpl::GPS_exec_impl::get_MyLocation (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return HUDisplay::CCM_position::_duplicate (this);
}

void
MyImpl::GPS_exec_impl::push_Refresh (HUDisplay::tick_ptr ev
                                     ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // Refresh position
  this->positionx_ += ACE_OS::rand () % DISPLACEMENT - (DISPLACEMENT/2);
  this->positiony_ += ACE_OS::rand () % DISPLACEMENT - (DISPLACEMENT/2);

  // Nitify others
  HUDisplay::tick_var event = new OBV_HUDisplay::tick;

  this->context_->push_Ready (event
                              ACE_ENV_ARG_PARAMETER);
}

// Operations from HUDisplay::position

CORBA::Long
MyImpl::GPS_exec_impl::posx (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->positionx_;
}

CORBA::Long
MyImpl::GPS_exec_impl::posy (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->positiony_;
}

// Operations from Components::SessionComponent
void
MyImpl::GPS_exec_impl::set_session_context (Components::SessionContext_ptr ctx
                                            ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
  ACE_DEBUG ((LM_DEBUG, "MyImpl::GPS_exec_impl::set_session_context\n"));

  this->context_ =
    HUDisplay::CCM_GPS_Context::_narrow (ctx
                                         ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (CORBA::is_nil (this->context_.in ()))
    ACE_THROW (CORBA::INTERNAL ());
  // Urm, we actually discard exceptions thown from this operation.
}

void
MyImpl::GPS_exec_impl::ccm_activate (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
  ACE_DEBUG ((LM_DEBUG, "MyImpl::GPS_exec_impl::ccm_activate\n"));
}

void
MyImpl::GPS_exec_impl::ccm_passivate (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
  ACE_DEBUG ((LM_DEBUG, "MyImpl::GPS_exec_impl::ccm_passivate\n"));
}

void
MyImpl::GPS_exec_impl::ccm_remove (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
  ACE_DEBUG ((LM_DEBUG, "MyImpl::GPS_exec_impl::ccm_remove\n"));
}

/// Default ctor.
MyImpl::GPSHome_exec_impl::GPSHome_exec_impl ()
{
}

/// Default dtor.
MyImpl::GPSHome_exec_impl::~GPSHome_exec_impl ()
{
}

// Explicit home operations.

// Implicit home operations.

::Components::EnterpriseComponent_ptr
MyImpl::GPSHome_exec_impl::create (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CreateFailure))
{
  return new MyImpl::GPS_exec_impl;
}


extern "C" GPS_EXEC_Export ::Components::HomeExecutorBase_ptr
createGPSHome_Impl (void)
{
  return new MyImpl::GPSHome_exec_impl;
}
