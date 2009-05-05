// $Id$

#ifndef FTCLIENT_TIMER_HANDLER_H_
#define FTCLIENT_TIMER_HANDLER_H_

#include <list>
#include "ace/Event_Handler.h"
#include "ace/High_Res_Timer.h"
#include "tao/ORB.h"
#include "WorkerC.h"

class Client_Timer_Handler : public ACE_Event_Handler
{
 public:
  Client_Timer_Handler (unsigned long iterations,
                        unsigned long log_start,
                        const std::string & filename,
                        const ACE_Time_Value & period,
                        const ACE_Time_Value & execution_time,
                        bool logging = false);

  ~Client_Timer_Handler ();

  void set_orb (CORBA::ORB_ptr orb);

  // Hook method that is called by the reactor when a timer expires.
  virtual int handle_timeout (const ACE_Time_Value &tv,
                              const void *);

  virtual int handle_signal (int signum, siginfo_t * = 0, ucontext_t * = 0);

  void set_worker (DeCoRAM::Worker_ptr worker);
    
 private:
    CORBA::ORB_var orb_;
    
    DeCoRAM::Worker_var worker_;

    ACE_High_Res_Timer timer_;

    ACE_Time_Value period_;

    ACE_Time_Value execution_time_;

    unsigned long invocations_;

    std::string logfile_;

    unsigned long max_iterations_;

    unsigned long log_start_;

    bool logging_;

    typedef std::list<CORBA::ULong> TimingList;

    TimingList history_;
};

#endif /* FTCLIENT_TIMER_HANDLER_H_ */
