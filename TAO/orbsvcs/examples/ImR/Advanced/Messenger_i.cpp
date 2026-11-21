// -*- C++ -*-
#include "Messenger_i.h"

#include <iostream>

Messenger_i::Messenger_i (CORBA::ORB_ptr orb, long instance)
  : instance_ (instance),
    orb_ (CORBA::ORB::_duplicate (orb))
{
}

bool Messenger_i::acknowledgeHit ()
{
  bool const isHit = isHit_;
  isHit_ = false;
  return isHit;
}

CORBA::Long Messenger_i::send_message (CORBA::Long thread,
                                       CORBA::Long iter,
                                       CORBA::Long obj,
                                       CORBA::Long req)
{
  std::cout << "* Invoked: (" << instance_ << "." << thread << "." << iter
            << "." << obj << "." << req << ")" << std::endl;
  isHit_ = true;
  return instance_;
}

void Messenger_i::shutdownOrb ()
{
  orb_->shutdown (false);
}
