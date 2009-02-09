// $Id$

#ifndef MESSENGER_I_H_
#define MESSENGER_I_H_

#include "MessengerS.h"

class  Messenger_i : public virtual POA_Messenger
{
public:
  virtual CORBA::Boolean send_message(Message*& msg);
};

#endif
