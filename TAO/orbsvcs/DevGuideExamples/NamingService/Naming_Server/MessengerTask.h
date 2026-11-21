#ifndef MESSENGERTASK_H
#define MESSENGERTASK_H

#include "tao/corba.h"
#include "ace/Task.h"

class MessengerTask : public ACE_Task_Base
{
public:
  MessengerTask ();
  int svc () override;

  MessengerTask (MessengerTask const &) = delete;
  MessengerTask (MessengerTask &&) = delete;
  MessengerTask &operator= (MessengerTask const &) = delete;
  MessengerTask &operator= (MessengerTask &&) = delete;

private:
  CORBA::ORB_var const orb_;
};

#endif
