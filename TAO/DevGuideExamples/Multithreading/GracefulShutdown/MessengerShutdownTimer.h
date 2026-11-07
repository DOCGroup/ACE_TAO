/* -*- C++ -*-  */

#ifndef MESSENGERSHUTDOWNTIMER_H_
#define MESSENGERSHUTDOWNTIMER_H_

#include "ace/Event_Handler.h"
#include "tao/corba.h"

class MessengerShutdownTimer : public ACE_Event_Handler
{
public:
  // Constructor
  explicit MessengerShutdownTimer (CORBA::ORB_ptr orb);

  int handle_timeout (
    ACE_Time_Value const &current_time,
    void const *act) override;

  MessengerShutdownTimer (MessengerShutdownTimer const &) = delete;
  MessengerShutdownTimer (MessengerShutdownTimer &&) = delete;
  MessengerShutdownTimer &operator= (MessengerShutdownTimer const &) = delete;
  MessengerShutdownTimer &operator= (MessengerShutdownTimer &&) = delete;

private:
  CORBA::ORB_var orb_;
};


#endif /* MESSENGERSHUTDOWNTIMER_H_  */
