// $Id$

#include "CIAO_common.h"
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
MyImpl::NavDisplay_exec_impl::push_Refresh (HUDisplay::tick_ptr ev
                                            ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_UNUSED_ARG (ev);

  // Refresh position
  HUDisplay::position_var loc
    = this->context_->get_connection_GPSLocation (ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (CORBA::is_nil (loc.in ()))
    ACE_THROW (CORBA::BAD_INV_ORDER ());

  CORBA::Long x = loc->posx (ACE_ENV_ARG_PARAMETER) % 500;
  ACE_CHECK;
  CORBA::Long y = loc->posy (ACE_ENV_ARG_PARAMETER) % 300;
  ACE_CHECK;

  ACE_DEBUG ((LM_DEBUG, "DISPLAY: Current Location is: (%d, %d)\n",
              x,
              y));
}

// Operations from Components::SessionComponent
void
MyImpl::NavDisplay_exec_impl::set_session_context (Components::SessionContext_ptr ctx
                                            ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
  ACE_DEBUG ((LM_DEBUG, "MyImpl::NavDisplay_exec_impl::set_session_context\n"));

  this->context_ =
    HUDisplay::CCM_NavDisplay_Context::_narrow (ctx
                                         ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (CORBA::is_nil (this->context_.in ()))
    ACE_THROW (CORBA::INTERNAL ());
  // Urm, we actually discard exceptions thown from this operation.
}

void
MyImpl::NavDisplay_exec_impl::ccm_activate (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
  ACE_DEBUG ((LM_DEBUG, "MyImpl::NavDisplay_exec_impl::ccm_activate\n"));

  // @@ This hack work around a missing feature in CIAO's assembly
  // mechanism where a Softpkg descriptor can specify it's dependency
  // to a valuetype factory and instruct the deployment framework to
  // initialize and register the corresponding valuefactory in the
  // component server.  Here, we are registering the valuefactory
  // explicitly to work around this problem.

  char *argv[1] = { "NavDisplay_exec"};

  int argc = sizeof(argv)/sizeof(argv[0]);
  CORBA::ORB_var orb = CORBA::ORB_init(argc, argv ACE_ENV_ARG_PARAMETER);

  CIAO_REGISTER_VALUE_FACTORY (orb.in(), HUDisplay::tick_init,
                               HUDisplay::tick);
}

void
MyImpl::NavDisplay_exec_impl::ccm_passivate (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
  ACE_DEBUG ((LM_DEBUG, "MyImpl::NavDisplay_exec_impl::ccm_passivate\n"));
}

void
MyImpl::NavDisplay_exec_impl::ccm_remove (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
  ACE_DEBUG ((LM_DEBUG, "MyImpl::NavDisplay_exec_impl::ccm_remove\n"));
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
MyImpl::NavDisplayHome_exec_impl::create (ACE_ENV_SINGLE_ARG_DECL)
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
