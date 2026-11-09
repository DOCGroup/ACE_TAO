#include "timer.h"

Timer_imp::Timer_imp (XtAppContext app,
                      CORBA::Long interval,
                      Stopwatch_display *stopwatch)
  : stopwatch_ (stopwatch),
    interval_ (interval),
    app_ (app)
{
}

void
Timer_imp::start ()
{
  // Reset the elapsed time
  this->counter_ = 0;

  // If a previous callback is still in effect, remove it
  if (this->id_)
    {
      XtRemoveTimeOut (this->id_);
      this->id_ = 0;
    }

  // Register a function to be called in interval_ milliseconds
  this->id_ = XtAppAddTimeOut (this->app_,
                               this->interval_,
                               &Timer_imp::tick_callback,
                               static_cast<XtPointer> (this));
}

void
Timer_imp::stop ()
{
    // Remove the current timeout function, if any
  if (this->id_)
    XtRemoveTimeOut (this->id_);

    this->id_ = 0;
}

CORBA::Float
Timer_imp::elapsed_time () const
{
  return static_cast<CORBA::Float> (this->counter_) * this->interval_ / 1000.0;
}

void
Timer_imp::tick_callback (XtPointer client_data,
                          XtIntervalId * )
{
  // Get the object pointer and call the corresponding tick function
  Timer_imp * const obj = static_cast<Timer_imp *> (client_data);
  obj->tick ();
}

void
Timer_imp::tick ()
{
  // Increment a counter for each tick
  ++this->counter_;

  // Call derived class function to report time
  this->report_time (this->elapsed_time ());

  // Reinstall the timeout callback
  this->id_ = XtAppAddTimeOut (this->app_,
                               this->interval_,
                               &Timer_imp::tick_callback,
                               static_cast<XtPointer> (this));
}

void
Timer_imp::report_time (CORBA::Float time)
{
  this->stopwatch_->set_time (time);
}
