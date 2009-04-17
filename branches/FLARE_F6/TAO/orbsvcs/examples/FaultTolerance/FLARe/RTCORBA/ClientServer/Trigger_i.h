#ifndef TRIGGER_I_H_
#define TRIGGER_I_H_

#include "TriggerS.h"
#include "Client_Timer_Handler.h"

class Trigger_i : public POA_Trigger
{
 public:
  Trigger_i (CORBA::ORB_ptr orb,
             Client_Timer_Handler & handler,
             unsigned long period);

  virtual ~Trigger_i ();

  void start (const char * prefix);

 private:
  CORBA::ORB_var orb_;
  Client_Timer_Handler & timer_handler_;
  bool started_;
  unsigned long period_;
};

#endif /* TRIGGER_I_H_ */
