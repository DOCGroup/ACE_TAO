// $Id$

#include "EC_exec.h"
#include "ace/Timer_Queue.h"
#include "ace/Reactor.h"

//=================================================================

MyImpl::timeout_Handler::timeout_Handler (MyImpl::EC_exec_i *cb)
  : active_ (0),
    done_ (0),
    tid_ (0),
    pulse_callback_ (cb)
{
  // Nothing
  this->reactor (new ACE_Reactor);
}

MyImpl::timeout_Handler::~timeout_Handler ()
{
  delete this->reactor ();
  this->reactor (0);
}

int
MyImpl::timeout_Handler::open ()
{
  return this->activate ();
}

int
MyImpl::timeout_Handler::close ()
{
  this->done_ = 1;
  this->reactor ()->notify ();

  ACE_DEBUG ((LM_DEBUG, "Waiting\n"));
  return this->wait ();
}

int
MyImpl::timeout_Handler::start (CORBA::Long hertz)
{
  if (hertz == 0 || this->active_ != 0)        // Not valid
    return -1;

  long usec = 1000000 / hertz;

  this->tid_ = this->reactor ()->schedule_timer (this,
                                                 0,
                                                 ACE_Time_Value (0, usec),
                                                 ACE_Time_Value (0, usec));

  this->active_ = 1;
  return 0;
}

int
MyImpl::timeout_Handler::stop (void)
{
  if (this->active_ == 0)       // Not valid.
    return -1;

  this->reactor ()->cancel_timer (this);

  this->active_ = 0;
  return 0;
}

int
MyImpl::timeout_Handler::active (void)
{
  return this->active_;
}

int
MyImpl::timeout_Handler::handle_close (ACE_HANDLE handle,
                                     ACE_Reactor_Mask close_mask)
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("[%x] handle = %d, close_mask = %d\n"),
              this,
              handle,
              close_mask));

  return 0;
}

int
MyImpl::timeout_Handler::handle_timeout (const ACE_Time_Value &,
                                       const void *)
{
  this->pulse_callback_->pulse ();

//   ACE_DEBUG ((LM_DEBUG,
//               ACE_TEXT ("[%x] with count #%05d timed out at %d.%d!\n"),
//               this,
//               tv.sec (),
//               tv.usec ()));

  return 0;
}

int
MyImpl::timeout_Handler::svc (void)
{
  this->reactor ()->owner (ACE_OS::thr_self ());

  while (!this->done_)
    this->reactor ()->handle_events ();

  return 0;
}

//=================================================================

MyImpl::EC_exec_i::EC_exec_i ()
  : hertz_ (0),
    pulser_ (this)
{

}

MyImpl::EC_exec_i::EC_exec_i (CORBA::Long hz)
  : hertz_ (hz),
    pulser_ (this)
{
}

MyImpl::EC_exec_i::~EC_exec_i ()
{
}

CORBA::Long
MyImpl::EC_exec_i::hertz (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->hertz_;
}

void
MyImpl::EC_exec_i::hertz (CORBA::Long hertz
                                  ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->hertz_ = hertz;
}

// Operations from supported interface(s)

void
MyImpl::EC_exec_i::start (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  if (this->hertz_ == 0 || this->pulser_.active())
    ACE_THROW (CORBA::BAD_INV_ORDER ());

  // @@ Start the rate generator
  this->pulser_.start (this->hertz_);
}

void
MyImpl::EC_exec_i::stop (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  if (! this->pulser_.active ())
    ACE_THROW (CORBA::BAD_INV_ORDER ());

  // @@ stop the rate generator
  this->pulser_.stop ();
}

CORBA::Boolean
MyImpl::EC_exec_i::active (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->pulser_.active ();
}

// Operations from Components::SessionComponent

void
MyImpl::EC_exec_i::set_session_context (Components::SessionContext_ptr ctx
                                                ACE_ENV_ARG_DECL_WITH_DEFAULTS)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
  ACE_DEBUG ((LM_DEBUG, "MyImpl::EC_exec_i::set_session_context\n"));

  this->context_ =
    BasicSP::CCM_EC_Context::_narrow (ctx
                                             ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (CORBA::is_nil (this->context_.in ()))
    ACE_THROW (CORBA::INTERNAL ());
  // Urm, we actually discard exceptions thown from this operation.

}

void
MyImpl::EC_exec_i::ccm_activate (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
  ACE_DEBUG ((LM_DEBUG, "MyImpl::EC_exec_i::ccm_activate\n"));

  this->pulser_.open ();
}

void
MyImpl::EC_exec_i::ccm_passivate (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
  ACE_DEBUG ((LM_DEBUG, "MyImpl::EC_exec_i::ccm_passivate\n"));
  this->pulser_.close ();
}

void
MyImpl::EC_exec_i::ccm_remove (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
  ACE_DEBUG ((LM_DEBUG, "MyImpl::EC_exec_i::ccm_remove\n"));
}

void
MyImpl::EC_exec_i::pulse (void)
{
  ACE_TRY_NEW_ENV
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("Pushing BasicSP::TimeOut event!\n")));

      BasicSP::TimeOut_var ev = new OBV_BasicSP::TimeOut ();

      this->context_->push_timeout (ev.in ()
                                  ACE_ENV_ARG_PARAMETER);
    }
  ACE_CATCHANY
    {
      // @@ do nothing?
    }
  ACE_ENDTRY;

}

MyImpl::ECHome_exec_i::ECHome_exec_i ()
{
}

MyImpl::ECHome_exec_i::~ECHome_exec_i ()
{
}

::Components::EnterpriseComponent_ptr
MyImpl::ECHome_exec_i::new_EC (CORBA::Long hertz
                               ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return new MyImpl::EC_exec_i (hertz);
}

::Components::EnterpriseComponent_ptr
MyImpl::ECHome_exec_i::create (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
  return new MyImpl::EC_exec_i ();
}


extern "C" EC_EXEC_Export ::Components::HomeExecutorBase_ptr
createECHome_Impl (void)
{
  return new MyImpl::ECHome_exec_i ();
}
