// $Id$

#include "RateGen_exec.h"
#include "ace/Timer_Queue.h"
#include "ace/Reactor.h"

//=================================================================

MyImpl::Pulse_Handler::Pulse_Handler (MyImpl::RateGen_exec_i *cb)
  : active_ (0),
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
MyImpl::Pulse_Handler::open_h ()
{
  return this->activate ();
}

int
MyImpl::Pulse_Handler::close_h ()
{
  this->done_ = 1;
  this->reactor ()->notify ();

  ACE_DEBUG ((LM_DEBUG, "Waiting\n"));

  return this->wait ();
}

int
MyImpl::Pulse_Handler::start (CORBA::Long hertz)
{
  if (hertz == 0 || this->active_ != 0)        // Not valid
    {
      return -1;
    }

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
    {
      return -1;
    }

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
MyImpl::Pulse_Handler::handle_timeout (const ACE_Time_Value &/*tv*/,
                                       const void *)
{
  this->pulse_callback_->pulse ();

/*   ACE_DEBUG ((LM_DEBUG,
               ACE_TEXT ("[%x] with count #%05d timed out at %d.%d!\n"),
               this,
               tv.sec (),
               tv.usec ()));
*/
  return 0;
}

int
MyImpl::Pulse_Handler::svc (void)
{
  this->reactor ()->owner (ACE_OS::thr_self ());

  while (!this->done_)
    {
      this->reactor ()->handle_events ();
    }

  return 0;
}

//=================================================================

MyImpl::RateGen_exec_i::RateGen_exec_i ()
  : hertz_ (0),
    pulser_ (this)
{

}

MyImpl::RateGen_exec_i::RateGen_exec_i (CORBA::Long hz)
  : hertz_ (hz),
    pulser_ (this)
{
}

MyImpl::RateGen_exec_i::~RateGen_exec_i ()
{
}

CORBA::Long
MyImpl::RateGen_exec_i::hertz ()
{
  return this->hertz_;
}

void
MyImpl::RateGen_exec_i::hertz (CORBA::Long hertz)
{
  this->hertz_ = hertz;
}

// Operations from supported interface(s)

void
MyImpl::RateGen_exec_i::start ()
{
  if (this->hertz_ == 0 || this->pulser_.active())
    {
      throw CORBA::BAD_INV_ORDER ();
    }

  // @@ Start the rate generator
  this->pulser_.start (this->hertz_);
}

void
MyImpl::RateGen_exec_i::stop ()
{
  if (! this->pulser_.active ())
    {
      throw CORBA::BAD_INV_ORDER ();
    }

  // @@ stop the rate generator
  this->pulser_.stop ();
}

CORBA::Boolean
MyImpl::RateGen_exec_i::active ()
{
  return this->pulser_.active ();
}

// Operations from Components::SessionComponent

void
MyImpl::RateGen_exec_i::set_session_context (
    Components::SessionContext_ptr ctx
  )
{
  ACE_DEBUG ((LM_DEBUG,
              "MyImpl::RateGen_exec_i::set_session_context\n"));

  this->context_ =
    HUDisplay::CCM_RateGen_Context::_narrow (ctx);

  if (CORBA::is_nil (this->context_.in ()))
    {
      ACE_DEBUG ((LM_DEBUG,
                   "MyImpl::RateGen_exec_i::context IS NIL!\n"));

      throw CORBA::INTERNAL ();
    }
}

void
MyImpl::RateGen_exec_i::configuration_complete ()
{
}

void
MyImpl::RateGen_exec_i::ccm_activate ()
{
  ACE_DEBUG ((LM_DEBUG,
              "MyImpl::RateGen_exec_i::ccm_activate\n"));

  this->pulser_.open_h ();
}

void
MyImpl::RateGen_exec_i::ccm_passivate ()
{
  ACE_DEBUG ((LM_DEBUG,
              "MyImpl::RateGen_exec_i::ccm_passivate\n"));

  this->pulser_.close_h ();
}

void
MyImpl::RateGen_exec_i::ccm_remove ()
{
  ACE_DEBUG ((LM_DEBUG,
              "MyImpl::RateGen_exec_i::ccm_remove\n"));
}

void
MyImpl::RateGen_exec_i::pulse (void)
{
  try
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("Pushing HUDisplay::tick event!\n")));
      HUDisplay::tick_var ev = new OBV_HUDisplay::tick ();

      this->context_->push_Pulse (ev.in ());
    }
  catch (const CORBA::Exception&)
    {
      // @@ do nothing?
    }

}

MyImpl::RateGenHome_exec_i::RateGenHome_exec_i ()
{
}

MyImpl::RateGenHome_exec_i::~RateGenHome_exec_i ()
{
}

::Components::EnterpriseComponent_ptr
MyImpl::RateGenHome_exec_i::new_RateGen (CORBA::Long /* hertz */)
{
  Components::EnterpriseComponent_ptr tmp = 0;
  ACE_NEW_THROW_EX (tmp,
                    MyImpl::RateGen_exec_i,
                    CORBA::NO_MEMORY ());

  return tmp;
}

::Components::EnterpriseComponent_ptr
MyImpl::RateGenHome_exec_i::create ()
{
  return new MyImpl::RateGen_exec_i ();
}


extern "C" RATEGEN_EXEC_Export ::Components::HomeExecutorBase_ptr
createRateGenHome_Impl (void)
{
  return new MyImpl::RateGenHome_exec_i ();
}
