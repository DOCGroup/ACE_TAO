//$Id$
#include "timer.h"

#if defined (ACE_HAS_XT)

Timer_imp::Timer_imp (XtAppContext &app,
                      CORBA::Long interval,
                      Stopwatch_display *stopwatch)
  :stopwatch_ (stopwatch),
   counter_ (0),
   interval_ (interval),
   id_ (0),
   app_ (app)
{
}

void
Timer_imp::start (void)
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
                               (XtPointer) this );
}

void
Timer_imp::stop (void)
{
    // Remove the current timeout function, if any
  if (this->id_)
    XtRemoveTimeOut (this->id_);

    this->id_ = 0;
}

CORBA::Float
Timer_imp::elapsed_time(void)
{
    return ((CORBA::Float) counter_ * interval_ / 1000.0 );
}

void
Timer_imp::tick_callback (XtPointer client_data,
                          XtIntervalId * )
{
  // Get the object pointer and call the corresponding tick function
  Timer_imp *obj = ACE_static_cast (Timer_imp *,
                                    client_data);
  obj->tick ();
}


void
Timer_imp::tick (void)
{
  // Increment a counter for each tick
  counter_++;

  // Call derived class function to report time
  this->report_time (this->elapsed_time ());

  // Reinstall the timeout callback
  this->id_ = XtAppAddTimeOut (app_,
                               interval_,
                               &Timer_imp::tick_callback,
                               (XtPointer) this);
}

void
Timer_imp::report_time (CORBA::Float time)
{
  stopwatch_->set_time (time);
}


#endif /*ACE_HAS_XT*/
