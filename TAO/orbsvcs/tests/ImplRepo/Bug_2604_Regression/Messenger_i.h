#ifndef MESSENGERI_H_
#define MESSENGERI_H_

#include "MessengerS.h"

class Messenger_i : public virtual POA_Messenger
{
public:
  Messenger_i (CORBA::ORB_ptr orb)
    : count_ (0),
      orb_ (CORBA::ORB::_duplicate (orb))
  {}

  CORBA::Boolean
  send_message (char const *user_name, char const *subject, char *&message) override;

private:
  int count_;
  CORBA::ORB_var orb_;
};

#endif
