// $Id$

#ifndef FTCLIENT_TIMER_HANDLER_H_
#define FTCLIENT_TIMER_HANDLER_H_

#include <list>
#include "ace/Event_Handler.h"
#include "ace/High_Res_Timer.h"
#include "tao/ORB.h"
#include "WorkerC.h"

class FTClient_Timer_Handler : public ACE_Event_Handler
{
  struct Client_Sample
  {
    ACE_Time_Value start;
    ACE_Time_Value response_time;
  };
  
  typedef std::list <Client_Sample> CLIENT_SAMPLES;

 public:
  FTClient_Timer_Handler (void);
    
  void set_orb (CORBA::ORB_ptr orb);

  // Hook method that is called by the reactor when a timer expires.
  virtual int handle_timeout (const ACE_Time_Value &tv,
                              const void *);

  virtual int handle_signal (int signum, siginfo_t * = 0, ucontext_t * = 0);

  void set_worker (DeCoRAM::Worker_ptr worker);

  // writes client samples to a file
  void dump_to_file (const std::string & filename);
    
 private:
    CORBA::ORB_var orb_;
    
    DeCoRAM::Worker_var worker_;

    ACE_High_Res_Timer timer_;

    Client_Sample current_sample_;

    CORBA::ULong invocations_;
    
    CLIENT_SAMPLES response_times_;
};

#endif /* FTCLIENT_TIMER_HANDLER_H_ */
