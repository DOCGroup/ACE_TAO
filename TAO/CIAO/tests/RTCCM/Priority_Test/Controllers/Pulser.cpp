// $Id$

#include "Pulser.h"
#include "CIAO_common.h"
#include "ace/Timer_Queue.h"
#include "ace/Reactor.h"

//=================================================================

PTImpl::Pulser::Pulser (PTImpl::Pulse_Handler *cb)
  : active_ (0),
    done_ (0),
    tid_ (0),
    pulser_callback_ (cb)
{
  // Nothing
  this->reactor (new ACE_Reactor);
}

PTImpl::Pulser::~Pulser ()
{
  delete this->reactor ();
  this->reactor (0);
}

int
PTImpl::Pulser::open (void*)
{
  return this->activate ();
}

int
PTImpl::Pulser::close (u_long)
{
  this->done_ = 1;
  this->reactor ()->notify ();

  if (CIAO::debug_level () > 0)
    ACE_DEBUG ((LM_DEBUG, "Waiting\n"));
  return this->wait ();
}

int
PTImpl::Pulser::start (CORBA::Long hertz)
{
  if (hertz == 0 || this->active_ != 0)        // Not valid
    return -1;

  this->active_ = 1;
  long usec = 1000000 / hertz;

  this->tid_ = this->reactor ()->schedule_timer (this,
                                                 0,
                                                 ACE_Time_Value (0, usec),
                                                 ACE_Time_Value (0, usec));

  return 0;
}

int
PTImpl::Pulser::stop (void)
{
  if (this->active_ == 0)       // Not valid.
    return -1;
  else
    this->active_ = 0;
  return 0;
}

int
PTImpl::Pulser::active (void)
{
  return this->active_;
}

int
PTImpl::Pulser::handle_close (ACE_HANDLE handle,
                                     ACE_Reactor_Mask close_mask)
{
  if (CIAO::debug_level () > 0)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("[%x] handle = %d, close_mask = %d\n"),
                this,
                handle,
                close_mask));

  return 0;
}

int
PTImpl::Pulser::handle_timeout (const ACE_Time_Value &,
                                       const void *)
{
  if (this->active_ == 0)
    this->reactor ()->cancel_timer (this);
  else
    this->pulser_callback_->pulse ();

  return 0;
}

int
PTImpl::Pulser::svc (void)
{
  this->reactor ()->owner (ACE_OS::thr_self ());

  while (!this->done_)
    this->reactor ()->handle_events ();

  return 0;
}

//=================================================================
