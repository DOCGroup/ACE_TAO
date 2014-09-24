// $Id$

#include "NavDisplay_exec.h"

/// Default constructor.
MyImpl::NavDisplay_exec_impl::NavDisplay_exec_impl (void)
:  number_of_GPS_(1)
{
}

/// Default destructor.
MyImpl::NavDisplay_exec_impl::~NavDisplay_exec_impl (void)
{
}

// Operations from HUDisplay::NavDisplay

void
MyImpl::NavDisplay_exec_impl::push_Refresh (HUDisplay::tick *ev)
{
  ACE_UNUSED_ARG (ev);

  ACE_DEBUG ((LM_DEBUG,
              "NavDisplay receives Refresh event from GPS supplier.\n"));
  ACE_DEBUG ((LM_DEBUG,
              "NavDisplay reads coordinates via GPS receptacle.\n"));
  ::HUDisplay::NavDisplay::GPSLocationConnections_var locs =
               this->context_->get_connections_GPSLocation ();
  for(CORBA::ULong i = 0; i < locs->length(); ++i)
    {
       HUDisplay::position_var loc = locs[i].objref;
       if (CORBA::is_nil (loc.in ()))
         {
           throw CORBA::BAD_INV_ORDER ();
         }
       HUDisplay::GPS_position posxy = loc->posxy();
       CORBA::Long x = posxy.pos_x  % 500;
       CORBA::Long y = posxy.pos_y  % 300;

       ACE_DEBUG ((LM_DEBUG,"NAVDISPLAY: Current Location is: (%u, %u)\n",
                   x,y));
    }
}
//needed for GUI implementation if more then one GPS device is used.
CORBA::Long
MyImpl::NavDisplay_exec_impl::number_of_GPS ()
{
  return this->number_of_GPS_;
}

void
MyImpl::NavDisplay_exec_impl::number_of_GPS (CORBA::Long number_of_GPS)
{
  this->number_of_GPS_ = number_of_GPS;
}

// Operations from Components::SessionComponent
void
MyImpl::NavDisplay_exec_impl::set_session_context (
  Components::SessionContext_ptr ctx)
{
  //ACE_DEBUG ((LM_DEBUG,
  //            "MyImpl::NavDisplay_exec_impl::set_session_context\n"));
  this->context_ =
    HUDisplay::CCM_NavDisplay_Context::_narrow (ctx);

  if (CORBA::is_nil (this->context_.in ()))
    {
      throw CORBA::INTERNAL ();
    }
}

void
MyImpl::NavDisplay_exec_impl::configuration_complete (void)
{
}

void
MyImpl::NavDisplay_exec_impl::ccm_activate (void)
{
  //ACE_DEBUG ((LM_DEBUG,
  //            "MyImpl::NavDisplay_exec_impl::ccm_activate\n"));
}

void
MyImpl::NavDisplay_exec_impl::ccm_remove (void)
{
  //ACE_DEBUG ((LM_DEBUG,
  //            "MyImpl::NavDisplay_exec_impl::ccm_remove\n"));
}

/// Default ctor.
MyImpl::NavDisplayHome_exec_impl::NavDisplayHome_exec_impl (void)
{
}

/// Default dtor.
MyImpl::NavDisplayHome_exec_impl::~NavDisplayHome_exec_impl (void)
{
}

// Explicit home operations.

// Implicit home operations.

::Components::EnterpriseComponent_ptr
MyImpl::NavDisplayHome_exec_impl::create (void)
{
  return new MyImpl::NavDisplay_exec_impl;
}


extern "C" NAVDISPLAY_EXEC_Export ::Components::HomeExecutorBase_ptr
createNavDisplayHome_Impl (void)
{
  return new MyImpl::NavDisplayHome_exec_impl;
}
