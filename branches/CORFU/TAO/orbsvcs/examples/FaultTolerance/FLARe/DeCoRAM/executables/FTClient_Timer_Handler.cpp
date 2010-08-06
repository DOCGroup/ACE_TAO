// $Id$

#include <fstream>
#include <ace/High_Res_Timer.h>
#include "FTClient_Timer_Handler.h"
#include "WorkerC.h"

extern double execution_time;

FTClient_Timer_Handler::FTClient_Timer_Handler (void)
  : invocations_ (0)
{
}

void
FTClient_Timer_Handler::set_orb (CORBA::ORB_ptr orb)
{
  orb_ = CORBA::ORB::_duplicate (orb);
}

void
FTClient_Timer_Handler::set_worker (DeCoRAM::Worker_ptr worker)
{
  worker_ = DeCoRAM::Worker::_duplicate (worker);
}

int
FTClient_Timer_Handler::handle_timeout (const ACE_Time_Value & tv,
                                        const void *)
{
  ++invocations_;
  
  try
    {
      timer_.start ();

      // we have to do some profiling first to see how we can achieve
      // the correct execution time.
      worker_->run_task (execution_time);

      timer_.stop ();
        
      current_sample_.start = tv;
      timer_.elapsed_time (current_sample_.response_time);
      response_times_.push_back (current_sample_);
    }
  catch (CORBA::SystemException & ex)
    {
      ACE_DEBUG ((LM_WARNING, 
                   ACE_TEXT ("FTClient_Timer_Handler::handle_timeout () -"
                             "caught: %s"), ex._info ().c_str ()));
      return 1;
    }

  return 0;
}

int
FTClient_Timer_Handler::handle_signal (int, siginfo_t *, ucontext_t *)
{
  orb_->shutdown ();

  return 0;
}

void 
FTClient_Timer_Handler::dump_to_file (const std::string & filename)
{
  std::ofstream ostr (filename.c_str ());

  for (CLIENT_SAMPLES::iterator it = response_times_.begin ();
       it != response_times_.end ();
       ++it)
    {
      ostr << it->start.msec () << " " << it->response_time.msec ()
           << std::endl;
    }

  ostr.close ();
}

