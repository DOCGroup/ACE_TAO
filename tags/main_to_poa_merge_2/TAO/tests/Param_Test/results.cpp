// $Id:

// ============================================================================
//
// = LIBRARY
//    TAO/tests/Param_Test
//
// = FILENAME
//    results.cpp
//
// = DESCRIPTION
//    Printing the results
//
// = AUTHORS
//    Aniruddha Gokhale
//
// ============================================================================

#include "results.h"

Results::Results (void)
{
}

Results::~Results (void)
{
}

void
Results::print_stats (const char *call_name)
{
  double tmp;

  ACE_DEBUG ((LM_DEBUG,
	      "%s:\n",
	      call_name));

  if (this->call_count_ > 0  &&  this->error_count_ == 0)
    {
      tmp = 1000 / this->elapsed_time_.real_time;

      this->elapsed_time_.real_time *= ACE_ONE_SECOND_IN_MSECS;
      this->elapsed_time_.user_time *= ACE_ONE_SECOND_IN_MSECS;
      this->elapsed_time_.system_time *= ACE_ONE_SECOND_IN_MSECS;

      this->elapsed_time_.real_time /= this->call_count_;
      this->elapsed_time_.user_time /= this->call_count_;
      this->elapsed_time_.system_time /= this->call_count_;

      tmp = 1000 / this->elapsed_time_.real_time;

      ACE_DEBUG ((LM_DEBUG,
		  "\treal_time\t= %0.06f ms, \n\t"
		  "user_time\t= %0.06f ms, \n\t"
		  "system_time\t= %0.06f ms\n"
		  "\t%0.00f calls/second\n",
		  this->elapsed_time_.real_time < 0.0? 0.0:this->elapsed_time_.real_time,
		  this->elapsed_time_.user_time < 0.0? 0.0:this->elapsed_time_.user_time,
		  this->elapsed_time_.system_time < 0.0? 0.0:this->elapsed_time_.system_time,
		  tmp < 0.0? 0.0 : tmp));
    }
  else
    {
      ACE_ERROR ((LM_ERROR,
                  "\tNo time stats printed.  Call count zero or error ocurred.\n"));
    }

  ACE_DEBUG ((LM_DEBUG,
	      "\t%d calls, %d errors\n",
	      this->call_count_,
	      this->error_count_));
}

void
Results::print_exception (const char *call_name,
                          CORBA::Environment &env)
{
  env.print_exception (call_name);
}

void
Results::start_timer (void)
{
  this->timer_.start ();
}

void
Results::stop_timer (void)
{
  this->timer_.stop ();
  this->timer_.elapsed_time (this->elapsed_time_);
}

CORBA::ULong
Results::call_count (void)
{
  return this->call_count_;
}

void
Results::call_count (CORBA::ULong c)
{
  this->call_count_ = c;
}

CORBA::ULong
Results::error_count (void)
{
  return this->error_count_;
}

void
Results::error_count (CORBA::ULong c)
{
  this->error_count_ = c;
}
