// -*- C++ -*-
// $Id$
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

  virtual CORBA::Boolean
  send_message (const char *user_name, const char *subject, char *&message);

private:
  int count_;
  CORBA::ORB_var orb_;
};

#endif
