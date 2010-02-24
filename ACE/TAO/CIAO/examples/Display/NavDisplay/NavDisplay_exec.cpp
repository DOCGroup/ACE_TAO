// $Id$

#include "NavDisplay_exec.h"

/// Default constructor.
MyImpl::NavDisplay_exec_impl::NavDisplay_exec_impl (void)
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

//   ACE_DEBUG ((LM_DEBUG,
//   ACE_TEXT ("NAVDISPLAY: Received Refresh Event\n")));

  // Refresh position
  HUDisplay::position_var loc =
    this->context_->get_connection_GPSLocation ();

  if (CORBA::is_nil (loc.in ()))
    {
      throw CORBA::BAD_INV_ORDER ();
    }

  CORBA::Long x = loc->posx () % 500;

  CORBA::Long y = loc->posy () % 300;

  ACE_DEBUG ((LM_DEBUG,
              "NAVDISPLAY: Current Location is: (%d, %d)\n",
              x,
              y));
}

// Operations from Components::SessionComponent
void
MyImpl::NavDisplay_exec_impl::set_session_context (
  Components::SessionContext_ptr ctx)
{
  ACE_DEBUG ((LM_DEBUG,
              "MyImpl::NavDisplay_exec_impl::set_session_context\n"));
  this->context_ =
    HUDisplay::CCM_NavDisplay_Context::_narrow (ctx);

  if (CORBA::is_nil (this->context_.in ()))
    {
      throw CORBA::INTERNAL ();
    }
  // Urm, we actually discard exceptions thown from this operation.
}

void
MyImpl::NavDisplay_exec_impl::configuration_complete (void)
{
}

void
MyImpl::NavDisplay_exec_impl::ccm_activate (void)
{
  ACE_DEBUG ((LM_DEBUG,
              "MyImpl::NavDisplay_exec_impl::ccm_activate\n"));
}

void
MyImpl::NavDisplay_exec_impl::ccm_remove (void)
{
  ACE_DEBUG ((LM_DEBUG,
              "MyImpl::NavDisplay_exec_impl::ccm_remove\n"));
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
