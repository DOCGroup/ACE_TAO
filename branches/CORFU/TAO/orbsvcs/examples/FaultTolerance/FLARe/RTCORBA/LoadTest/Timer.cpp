
#include "Timer.h"

#include "ace/Timer_Queue.h"
#include "ace/Reactor.h"

//=================================================================

// constructor
Timer::Timer (void)
  : done_ (0),
    active_ (0),
    hertz_ (0),
    tid_ (0)
{
  // Setup a reactor different from what the ORB is using that we shall use
  // exclusively for timer capabilities.
  this->reactor (new ACE_Reactor);
}

// destructor
Timer::~Timer (void)
{
  delete this->reactor ();
  this->reactor (0);
}

// get our attribute
double
Timer::hertz (void)
{
  return this->hertz_;
}

// set our attribute
void
Timer::hertz (double h)
{
  this->hertz_ = h;
}

// start the timer
void
Timer::start (void)
{
  // we are using this step in case we want to restart the timer capabilities
  // again
  this->done_ = 0;

  // check if parameters are valid
  if (this->hertz_ == 0 || this->active_ !=0) {        // Not valid
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("[Timer::start] ")
                ACE_TEXT ("Bad Parameters\n")));
    // we should throw some exception
    return;
  }

  // start an active object that will handle events - particularly timeout
  // events
  if (this->activate () == -1) {
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("[Timer::start] ")
                ACE_TEXT ("Active object activation failed (%p)\n")));
    // we should throw some exception
    return;
  }

  // we are now active
  this->active_ = 1;

  long interval = static_cast<long> (1000000 / this->hertz_);

  // start a periodic timer
  this->tid_ = this->reactor ()->schedule_timer (this,
                                                 0,
                                                 ACE_Time_Value (0, interval),
                                                 ACE_Time_Value (0, interval));

  if (this->tid_ == -1) {
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("[Timer::start] ")
                ACE_TEXT ("Scheduling timer failed (%p)\n")));
    // we should throw some exception
    return;
  }

  // success
}

// stopping the timer
void
Timer::stop (void)
{
  if (this->active_ == 0) {      // Not valid.
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("[Timer::stop] ")
                ACE_TEXT ("bad parameter\n")));
    // we should throw some exception
    return;
  }

  // cancel the timer
  this->reactor ()->cancel_timer (this);

  // we are no longer active
  this->active_ = 0;

  // asynchronous notification to the active object thread
  this->done_ = 1;

  // send an event to the reactor which will invoke the handle_close
  this->reactor ()->notify ();

  ///ACE_DEBUG ((LM_DEBUG, "Waiting\n"));

  // wait for the active object thread to quit
  this->wait ();
}

// callback function from the timeout handler.
int
Timer::pulse (void)
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("obtain utilization input\n")));

  return 0;
}

// handling the timeout
int
Timer::handle_timeout (const ACE_Time_Value &,
                                        const void *)
{
  // ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("[Timer::handle_timeout] - timeout\n")));
  // timeout has occured. Time to callback. The callback function will do the
  // RM's job of getting utilization info from the hosts.
  return this->pulse ();
}

// cleanup
int
Timer::handle_close (ACE_HANDLE handle,
                                      ACE_Reactor_Mask close_mask)
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("[%x] handle = %d, close_mask = %d\n"),
              this,
              handle,
              close_mask));

  return 0;
}

// active object thread
int
Timer::svc (void)
{
  // set the owner
  this->reactor ()->owner (ACE_OS::thr_self ());

  // continue until someone stops us asynchronously
  while (!this->done_)
    this->reactor ()->handle_events ();

  return 0;
}

