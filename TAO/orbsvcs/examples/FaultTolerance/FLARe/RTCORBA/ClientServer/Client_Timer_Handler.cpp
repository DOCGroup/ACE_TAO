// $Id$

#include <fstream>
#include <ace/High_Res_Timer.h>
#include "Client_Timer_Handler.h"
#include "WorkerC.h"
#include "ace/Reactor.h"
#include "tao/ORB_Core.h"

extern double execution_time;

Client_Timer_Handler::Client_Timer_Handler (unsigned long iterations,
                                            unsigned long log_start,
                                            const std::string & filename,
                                            const ACE_Time_Value & period,
                                            const ACE_Time_Value & execution_time,
                                            bool logging)
  : period_ (period),
    execution_time_ (execution_time),
    invocations_ (0),
    logfile_ (filename),
    max_iterations_ (iterations),
    log_start_ (log_start),
    logging_ (logging)
{
  timer_.calibrate ();
}

Client_Timer_Handler::~Client_Timer_Handler ()
{
  if (logging_)
    {
      std::ofstream out (logfile_.c_str ());

      for (TimingList::iterator it = history_.begin ();
           it != history_.end ();
           ++it)
        {
          out << *it << std::endl;
        }

      out.close ();
    }
}

void
Client_Timer_Handler::set_orb (CORBA::ORB_ptr orb)
{
  orb_ = CORBA::ORB::_duplicate (orb);
}

void
Client_Timer_Handler::set_worker (DeCoRAM::Worker_ptr worker)
{
  worker_ = DeCoRAM::Worker::_duplicate (worker);
}

int
Client_Timer_Handler::handle_timeout (const ACE_Time_Value &,
                                      const void *)
{
  try
    {
      CORBA::ULong server_processing_time;

      timer_.start ();

      // we have to do some profiling first to see how we can achieve
      // the correct execution time.
      server_processing_time = worker_->run_task (execution_time_.msec ());

      timer_.stop ();

      ++invocations_;

      ACE_DEBUG ((LM_EMERGENCY, "m(%s,%d,%d,%d) ",
                  logfile_.c_str (),
                  max_iterations_,
                  log_start_,
                  invocations_));

      if (logging_ && (invocations_ >= log_start_))
        {
          ACE_Time_Value rt;
          timer_.elapsed_time (rt);

          history_.push_back (rt.msec () - server_processing_time);
        }
    }
  catch (CORBA::SystemException & ex)
    {
      ACE_DEBUG ((LM_EMERGENCY, 
                  ACE_TEXT ("Client_Timer_Handler::handle_timeout () -"
                            "caught: %s"), ex._info ().c_str ()));

      orb_->shutdown ();

      return 1;
    }

  try
    {
      if ((max_iterations_ > 0) && (invocations_ >= max_iterations_))
        {
          worker_->stop ();
     
          orb_->orb_core ()->reactor ()->cancel_timer (this);

          orb_->shutdown ();
        }
    }
  catch (CORBA::Exception & ex)
    {
      ACE_DEBUG ((LM_WARNING, 
                  "Client_Timer_Handler::handle_timeout () after run_task - "
                  "caught: %s", ex._info ().c_str ()));
    }

  return 0;
}

int
Client_Timer_Handler::handle_signal (int, siginfo_t *, ucontext_t *)
{
  orb_->orb_core ()->reactor ()->cancel_timer (this);

  orb_->shutdown ();

  return 0;
}
