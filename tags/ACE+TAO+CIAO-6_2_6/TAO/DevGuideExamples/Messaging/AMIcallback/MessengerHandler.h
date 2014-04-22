// $Id$

#ifndef MESSENGERHANDLER_H
#define MESSENGERHANDLER_H

#include "MessengerS.h"

class  MessengerHandler : public virtual POA_AMI_MessengerHandler
{
public:
  //Constructor
  MessengerHandler (CORBA::ORB_ptr orb);

  //Destructor
  virtual ~MessengerHandler (void);

virtual void send_message (
    CORBA::Boolean ami_return_val,
    const char * message,
    CORBA::Long time
  );

virtual void send_message_excep (
    Messaging::ExceptionHolder * excep_holder
  );


  int message_was_sent()
  {
    return time_ > 0;
  }

private:
  CORBA::Long time_;
  CORBA::ORB_var orb_;
};

#endif
