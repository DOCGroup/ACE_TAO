// $Id$

#include "ciao/CIAO_common.h"
#include "NavDisplay_exec.h"

/// Default constructor.
MyImpl::NavDisplay_exec_impl::NavDisplay_exec_impl ()
{
}

/// Default destructor.
MyImpl::NavDisplay_exec_impl::~NavDisplay_exec_impl ()
{
}

// Operations from HUDisplay::NavDisplay

void
MyImpl::NavDisplay_exec_impl::push_Refresh (HUDisplay::tick *ev
                                            ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_UNUSED_ARG (ev);

//   ACE_DEBUG ((LM_DEBUG,
// 	      ACE_TEXT ("NAVDISPLAY: Received Refresh Event\n")));

  // Refresh position
  HUDisplay::position_var loc =
    this->context_->get_connection_GPSLocation (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  if (CORBA::is_nil (loc.in ()))
    {
      ACE_THROW (CORBA::BAD_INV_ORDER ());
    }

  CORBA::Long x = loc->posx (ACE_ENV_SINGLE_ARG_PARAMETER) % 500;
  ACE_CHECK;

  CORBA::Long y = loc->posy (ACE_ENV_SINGLE_ARG_PARAMETER) % 300;
  ACE_CHECK;

  ACE_DEBUG ((LM_DEBUG, "NAVDISPLAY: Current Location is: (%d, %d)\n",
              x,
              y));
}

// Operations from Components::SessionComponent
void
MyImpl::NavDisplay_exec_impl::set_session_context (
    Components::SessionContext_ptr ctx
    ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
  if (CIAO::debug_level () > 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "MyImpl::NavDisplay_exec_impl::set_session_context\n"));
    }

  this->context_ =
    HUDisplay::CCM_NavDisplay_Context::_narrow (ctx
                                                ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (CORBA::is_nil (this->context_.in ()))
    {
      ACE_THROW (CORBA::INTERNAL ());
    }
  // Urm, we actually discard exceptions thown from this operation.
}

void
MyImpl::NavDisplay_exec_impl::ciao_preactivate (
    ACE_ENV_SINGLE_ARG_DECL_NOT_USED
  )
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
}

void
MyImpl::NavDisplay_exec_impl::ccm_activate (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
  if (CIAO::debug_level () > 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "MyImpl::NavDisplay_exec_impl::ccm_activate\n"));
    }
}

void
MyImpl::NavDisplay_exec_impl::ciao_postactivate (
    ACE_ENV_SINGLE_ARG_DECL_NOT_USED
  )
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
}

void
MyImpl::NavDisplay_exec_impl::ccm_passivate (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
  if (CIAO::debug_level () > 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "MyImpl::NavDisplay_exec_impl::ccm_passivate\n"));
    }
}

void
MyImpl::NavDisplay_exec_impl::ccm_remove (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
  if (CIAO::debug_level () > 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "MyImpl::NavDisplay_exec_impl::ccm_remove\n"));
    }
}

/// Default ctor.
MyImpl::NavDisplayHome_exec_impl::NavDisplayHome_exec_impl ()
{
}

/// Default dtor.
MyImpl::NavDisplayHome_exec_impl::~NavDisplayHome_exec_impl ()
{
}

// Explicit home operations.

// Implicit home operations.

::Components::EnterpriseComponent_ptr
MyImpl::NavDisplayHome_exec_impl::create (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
  return new MyImpl::NavDisplay_exec_impl;
}


extern "C" NAVDISPLAY_EXEC_Export ::Components::HomeExecutorBase_ptr
createNavDisplayHome_Impl (void)
{
  return new MyImpl::NavDisplayHome_exec_impl;
}
