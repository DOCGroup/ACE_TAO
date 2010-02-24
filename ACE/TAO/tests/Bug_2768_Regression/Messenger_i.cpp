// $Id$

#include "Messenger_i.h"

Messenger_i::Messenger_i (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

void Messenger_i::send (void)
{
  this->orb_->shutdown (0);
}

