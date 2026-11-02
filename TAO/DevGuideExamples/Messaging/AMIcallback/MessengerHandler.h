#ifndef MESSENGERHANDLER_H
#define MESSENGERHANDLER_H

#include "MessengerS.h"

class  MessengerHandler : public virtual POA_AMI_MessengerHandler
{
public:
  //Constructor
  explicit MessengerHandler (CORBA::ORB_ptr orb);

  //Destructor
  ~MessengerHandler () override = default;

  void send_message (
    CORBA::Boolean ami_return_val,
    char const *message,
    CORBA::Long time) override;

  void send_message_excep (
    Messaging::ExceptionHolder *excep_holder) override;

  bool message_was_sent () const
  {
    return time_ > 0;
  }

  MessengerHandler (MessengerHandler const &) = delete;
  MessengerHandler (MessengerHandler &&) = delete;
  MessengerHandler &operator= (MessengerHandler const &) = delete;
  MessengerHandler &operator= (MessengerHandler &&) = delete;

private:
  CORBA::Long time_ {};
  CORBA::ORB_var orb_;
};

#endif
