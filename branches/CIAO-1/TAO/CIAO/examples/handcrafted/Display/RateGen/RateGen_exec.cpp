// $Id$

#include "RateGen_exec.h"
#include "ace/Timer_Queue.h"
#include "ace/Reactor.h"

//=================================================================

MyImpl::Pulse_Handler::Pulse_Handler (MyImpl::RateGen_exec_impl *cb)
  : active_ (0),
    count_ (0),
    done_ (0),
    tid_ (0),
    pulse_callback_ (cb)
{
  // Nothing
  this->reactor (new ACE_Reactor);
}

MyImpl::Pulse_Handler::~Pulse_Handler ()
{
  delete this->reactor ();
  this->reactor (0);
}

int
MyImpl::Pulse_Handler::open ()
{
  return this->thr_mgr_.spawn (Pulse_Handler::svc_run,
                               this);
}

int
MyImpl::Pulse_Handler::close ()
{
  this->done_ = 1;
  this->reactor ()->notify ();

  ACE_DEBUG ((LM_DEBUG, "Waiting\n"));
  return this->thr_mgr_.wait ();
}

int
MyImpl::Pulse_Handler::start (CORBA::Long hertz)
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
MyImpl::Pulse_Handler::stop (void)
{
  if (this->active_ == 0)       // Not valid.
    return -1;

  this->reactor ()->cancel_timer (this);

  this->active_ = 0;
  return 0;
}

int
MyImpl::Pulse_Handler::active (void)
{
  return this->active_;
}

int
MyImpl::Pulse_Handler::handle_close (ACE_HANDLE handle,
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
MyImpl::Pulse_Handler::handle_timeout (const ACE_Time_Value &tv,
                                       const void *arg)
{
  this->pulse_callback_->pulse ();

//   ACE_DEBUG ((LM_DEBUG,
//               ACE_TEXT ("[%x] with count #%05d timed out at %d.%d!\n"),
//               this,
//               this->count_,
//               tv.sec (),
//               tv.usec ()));

  ++this->count_;
  return 0;
}

ACE_THR_FUNC_RETURN
MyImpl::Pulse_Handler::svc_run (void *args)
{
  Pulse_Handler *handler = (Pulse_Handler *) args;

  handler->reactor ()->owner (ACE_OS::thr_self ());

  while (!handler->done_)
    handler->reactor ()->handle_events ();

  return 0;
}

//=================================================================

MyImpl::RateGen_exec_impl::RateGen_exec_impl ()
  : hertz_ (0),
    pulser_ (this)
{

}

MyImpl::RateGen_exec_impl::RateGen_exec_impl (CORBA::Long hz)
  : hertz_ (hz),
    pulser_ (this)
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
  if (this->hertz_ == 0 || this->pulser_.active())
    ACE_THROW (CORBA::BAD_INV_ORDER ());

  // @@ Start the rate generator
  this->pulser_.start (this->hertz_);
}

void
MyImpl::RateGen_exec_impl::stop (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  if (! this->pulser_.active ())
    ACE_THROW (CORBA::BAD_INV_ORDER ());

  // @@ stop the rate generator
  this->pulser_.stop ();
}

CORBA::Boolean
MyImpl::RateGen_exec_impl::active (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->pulser_.active ();
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

  this->pulser_.open ();
}

void
MyImpl::RateGen_exec_impl::ccm_passivate (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
  ACE_DEBUG ((LM_DEBUG, "MyImpl::RateGen_exec_impl::ccm_passivate\n"));
  this->pulser_.close ();
}

void
MyImpl::RateGen_exec_impl::ccm_remove (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
  ACE_DEBUG ((LM_DEBUG, "MyImpl::RateGen_exec_impl::ccm_remove\n"));
}

void
MyImpl::RateGen_exec_impl::pulse (void)
{
  ACE_TRY_NEW_ENV
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("Pushing HUDisplay::tick event!\n")));

      HUDisplay::tick_var ev = new OBV_HUDisplay::tick ();

      this->context_->push_Pulse (ev.in ()
                                  ACE_ENV_ARG_PARAMETER);
    }
  ACE_CATCHANY
    {
      // @@ do nothing?
    }
  ACE_ENDTRY;

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
