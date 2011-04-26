
// $Id$

#include "GPS_exec.h"

#include "ace/OS_NS_time.h"

#define DISPLACEMENT 256

// Operations from HUDisplay::position
CORBA::Long
MyImpl::Position_Impl::posx ()
{
  return component_.posx();
}

CORBA::Long
MyImpl::Position_Impl::posy ()
{
  return component_.posy();
}


/// Default constructor.
MyImpl::GPS_exec_i::GPS_exec_i ()
{
  ACE_OS::srand ((u_int) ACE_OS::time ());
  this->positionx_ = ACE_OS::rand ();
  this->positiony_ = ACE_OS::rand ();
}

/// Default destructor.
MyImpl::GPS_exec_i::~GPS_exec_i ()
{
}

// Operations from HUDisplay::GPS
  HUDisplay::CCM_position_ptr
  MyImpl::GPS_exec_i::get_MyLocation ()
  {
    ACE_DEBUG ((LM_DEBUG,
                 "GPS_exec::get_MyLocation called\n"));
    return (new Position_Impl (*this));
  }

void
MyImpl::GPS_exec_i::push_Refresh (HUDisplay::tick *)
{
  ACE_DEBUG ((LM_DEBUG,
               ACE_TEXT ("GPS: Received Refresh Event\n")));

  // Refresh position
  this->positionx_ += ACE_OS::rand () % DISPLACEMENT - (DISPLACEMENT/2);
  this->positiony_ += ACE_OS::rand () % DISPLACEMENT - (DISPLACEMENT/2);

  // Nitify others
  HUDisplay::tick_var event = new OBV_HUDisplay::tick;

  this->context_->push_Ready (event);
}

CORBA::Long
MyImpl::GPS_exec_i::posx ()
{
  return this->positionx_;
}

CORBA::Long
MyImpl::GPS_exec_i::posy ()
{
  return this->positiony_;
}


// Operations from Components::SessionComponent
void
MyImpl::GPS_exec_i::set_session_context (Components::SessionContext_ptr ctx)
{
  ACE_DEBUG ((LM_DEBUG, "MyImpl::GPS_exec_i::set_session_context\n"));

  this->context_ =
    HUDisplay::CCM_GPS_Context::_narrow (ctx);

  if (CORBA::is_nil (this->context_.in ()))
    {
      ACE_DEBUG ((LM_DEBUG, "MyImpl::GPS_exec_i::context is NIL !\n"));
      throw CORBA::INTERNAL ();
    }
}

void
MyImpl::GPS_exec_i::configuration_complete ()
{
}

void
MyImpl::GPS_exec_i::ccm_activate ()
{
  ACE_DEBUG ((LM_DEBUG, "MyImpl::GPS_exec_i::ccm_activate\n"));
}

void
MyImpl::GPS_exec_i::ccm_passivate ()
{
  ACE_DEBUG ((LM_DEBUG, "MyImpl::GPS_exec_i::ccm_passivate\n"));
}

void
MyImpl::GPS_exec_i::ccm_remove ()
{
  ACE_DEBUG ((LM_DEBUG, "MyImpl::GPS_exec_i::ccm_remove\n"));
}

/// Default ctor.
MyImpl::GPSHome_exec_i::GPSHome_exec_i ()
{
}

/// Default dtor.
MyImpl::GPSHome_exec_i::~GPSHome_exec_i ()
{
}

// Explicit home operations.

// Implicit home operations.

::Components::EnterpriseComponent_ptr
MyImpl::GPSHome_exec_i::create ()
{
  return new MyImpl::GPS_exec_i;
}


extern "C" GPS_EXEC_Export ::Components::HomeExecutorBase_ptr
createGPSHome_Impl (void)
{
  return new MyImpl::GPSHome_exec_i();
}
