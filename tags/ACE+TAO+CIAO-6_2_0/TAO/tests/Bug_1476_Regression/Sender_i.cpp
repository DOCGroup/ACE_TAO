// $Id$

#include "Sender_i.h"

Sender_i::Sender_i (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb)),
    active_objects_ (0),
    number_received_ (0)
{
}

Sender_i::~Sender_i (void)
{
}

void
Sender_i::active_objects (CORBA::ULong no_threads)
{
  this->active_objects_ = no_threads;
}


void
Sender_i::send_ready_message (void)
{
  ACE_DEBUG ((LM_DEBUG,
              "Received a call ...\n"));

  number_received_++;
}

CORBA::ULong
Sender_i::get_number_received (void) const
{
  return number_received_;
}

CORBA::ULong
Sender_i::get_active_objects (void) const
{
  return active_objects_;
}
