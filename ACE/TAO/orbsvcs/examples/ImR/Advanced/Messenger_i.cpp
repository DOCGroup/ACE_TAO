// -*- C++ -*-
//
// $Id$

#include "Messenger_i.h"

#include <ace/streams.h>

Messenger_i::Messenger_i (CORBA::ORB_ptr orb, long instance)
: instance_(instance)
, orb_(CORBA::ORB::_duplicate(orb))
, isHit_(false)
{
}

Messenger_i::~Messenger_i (void)
{
}

bool Messenger_i::acknowledgeHit()
{
  bool isHit = isHit_;
  isHit_ = false;
  return isHit;
}

CORBA::Long Messenger_i::send_message (CORBA::Long thread,
                                       CORBA::Long iter,
                                       CORBA::Long obj,
                                       CORBA::Long req)
{
  cout << "* Invoked: (" << instance_ << "." << thread << "." << iter
    << "." << obj << "." << req << ")" << endl;
  isHit_ = true;
  return instance_;
}

void Messenger_i::shutdownOrb ()
{
  orb_->shutdown(0);
}
