// $Id$

#include "GPS_tracing_exec.h"
#include "CIAO_common.h"

#include "ace/OS_NS_time.h"

#define DISPLACEMENT 256

/// Default constructor.
MyImpl::GPS_tracing_exec_i::GPS_tracing_exec_i () : dx_(1), dy_(1)
{
  ACE_OS::srand ((u_int) ACE_OS::time ());

  this->positionx_ = 20;
  this->positiony_ = 25;
}

/// Default destructor.
MyImpl::GPS_tracing_exec_i::~GPS_tracing_exec_i ()
{
}

// Operations from HUDisplay::GPS

HUDisplay::CCM_position_ptr
MyImpl::GPS_tracing_exec_i::get_MyLocation ()
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return HUDisplay::CCM_position::_duplicate (this);
}

void
MyImpl::GPS_tracing_exec_i::push_Refresh (HUDisplay::tick *)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // Refresh position
  if(this->positionx_ > 500) this->dx_ = -1;
  if(this->positionx_ < 10) this->dx_ = 1;
  if(this->positiony_ > 300) this->dy_ = -1;
  if(this->positiony_ < 10) this->dy_ = 1;

  this->positionx_ += this->dx_;
  this->positiony_ += this->dy_;

  // Nitify others
  HUDisplay::tick_var event = new OBV_HUDisplay::tick;

  this->context_->push_Ready (event);
}

// Operations from HUDisplay::position

CORBA::Long
MyImpl::GPS_tracing_exec_i::posx ()
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->positionx_;
}

CORBA::Long
MyImpl::GPS_tracing_exec_i::posy ()
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->positiony_;
}

// Operations from Components::SessionComponent
void
MyImpl::GPS_tracing_exec_i::set_session_context (
    Components::SessionContext_ptr ctx)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
  if (CIAO::debug_level () > 0)
    ACE_DEBUG ((LM_DEBUG,
                "MyImpl::GPS_tracing_exec_i::set_session_context\n"));

  this->context_ =
    HUDisplay::CCM_GPS_Context::_narrow (ctx);

  if (CORBA::is_nil (this->context_.in ()))
    throw CORBA::INTERNAL ();
  // Urm, we actually discard exceptions thown from this operation.
}

void
MyImpl::GPS_tracing_exec_i::ciao_preactivate ()
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
}

void
MyImpl::GPS_tracing_exec_i::ccm_activate ()
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
  if (CIAO::debug_level () > 0)
    ACE_DEBUG ((LM_DEBUG, "MyImpl::GPS_tracing_exec_i::ccm_activate\n"));
}

void
MyImpl::GPS_tracing_exec_i::ciao_postactivate ()
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
}

void
MyImpl::GPS_tracing_exec_i::ccm_passivate ()
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
  if (CIAO::debug_level () > 0)
    ACE_DEBUG ((LM_DEBUG, "MyImpl::GPS_tracing_exec_i::ccm_passivate\n"));
}

void
MyImpl::GPS_tracing_exec_i::ccm_remove ()
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
  if (CIAO::debug_level () > 0)
    ACE_DEBUG ((LM_DEBUG, "MyImpl::GPS_tracing_exec_i::ccm_remove\n"));
}

/// Default ctor.
MyImpl::GPSHome_tracing_exec_i::GPSHome_tracing_exec_i ()
{
}

/// Default dtor.
MyImpl::GPSHome_tracing_exec_i::~GPSHome_tracing_exec_i ()
{
}

// Explicit home operations.

// Implicit home operations.

::Components::EnterpriseComponent_ptr
MyImpl::GPSHome_tracing_exec_i::create ()
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
  return new MyImpl::GPS_tracing_exec_i;
}


extern "C" GPS_EXEC_Export ::Components::HomeExecutorBase_ptr
createGPSHome_Tracing_Impl (void)
{
  return new MyImpl::GPSHome_tracing_exec_i;
}
