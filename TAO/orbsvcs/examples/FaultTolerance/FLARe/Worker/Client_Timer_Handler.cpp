// $Id$

#include <fstream>
#include <ace/High_Res_Timer.h>
#include "Client_Timer_Handler.h"
#include "WorkerC.h"

extern double execution_time;

Client_Timer_Handler::Client_Timer_Handler (long iterations,
                                            const std::string & filename,
                                            const ACE_Time_Value & period,
                                            bool logging)
  : period_ (period),
    invocations_ (0),
    logfile_ (filename),
    max_iterations_ (iterations),
    logging_ (logging)
{
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
      timer_.start ();

      // we have to do some profiling first to see how we can achieve
      // the correct execution time.
      worker_->run_task (execution_time);

      timer_.stop ();
       
      ACE_Time_Value rt;
      timer_.elapsed_time (rt);

      if (logging_)
	history_.push_back (rt.msec ());
    }
  catch (CORBA::SystemException & ex)
    {
      ACE_DEBUG ((LM_WARNING, 
                   ACE_TEXT ("Client_Timer_Handler::handle_timeout () -"
                             "caught: %s"), ex._info ().c_str ()));

      orb_->shutdown ();

      return 1;
    }

  if ((max_iterations_ > 0) && (++invocations_ > max_iterations_))
    {
      worker_->stop ();
      
      orb_->shutdown ();
    }

  return 0;
}

int
Client_Timer_Handler::handle_signal (int, siginfo_t *, ucontext_t *)
{
  orb_->shutdown ();

  return 0;
}
