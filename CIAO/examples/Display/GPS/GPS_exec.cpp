// $Id$

#include "GPS_exec.h"

#include "ace/OS_NS_time.h"

#define DISPLACEMENT 256

// Operations from HUDisplay::position

HUDisplay::GPS_position
MyImpl::Position_Impl::posxy ()
{
  return component_.posxy();
}

CORBA::UShort
MyImpl::Position_Impl::id ()
{
  return component_.id();
}

CORBA::Boolean
MyImpl::Position_Impl::started ()
{
  return component_.started ();
}

/// Default constructor.
MyImpl::GPS_exec_i::GPS_exec_i ()
: id_(1), started_(false)
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
                 "GPS_exec::get_MyLocation facet called by NavDisplay\n"));
  return (new Position_Impl (*this));
}

void
MyImpl::GPS_exec_i::push_Refresh (HUDisplay::tick *)
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("GPS: Received Refresh Event from RateGen for GPS %u\n"),
              this->id_));

  // Refresh position
  this->positionx_ += ACE_OS::rand () % DISPLACEMENT - (DISPLACEMENT/2);
  this->positiony_ += ACE_OS::rand () % DISPLACEMENT - (DISPLACEMENT/2);
  started_= true;

  // Notify others
  HUDisplay::tick_var event = new OBV_HUDisplay::tick ();

  ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("GPS: Notify NavDisplay via Ready event.\n")));
  this->context_->push_Ready (event);
}

HUDisplay::GPS_position
MyImpl::GPS_exec_i::posxy ()
{
  HUDisplay::GPS_position pos;
  pos.pos_x =  this->positionx_;
  pos.pos_y =  this->positiony_;
  return pos;
}

CORBA::UShort
MyImpl::GPS_exec_i::id ()
{
  return this->id_;
}

void
MyImpl::GPS_exec_i::id (
  CORBA::UShort id)
{
  this->id_ = id;
}

CORBA::Boolean
MyImpl::GPS_exec_i::started ()
{
  return started_;
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
