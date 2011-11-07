// $Id$

#include "Sender_i.h"

Sender_i::Sender_i (CORBA::ORB_ptr orb,
                    CORBA::ULong no)
  : orb_ (CORBA::ORB::_duplicate (orb)),
    number_ (no),
    active_objects_ (0)
{
}

Sender_i::~Sender_i (void)
{
}

void
Sender_i::active_objects (CORBA::Short no_threads
 /* */ )
{
  this->active_objects_ = no_threads;
}


void
Sender_i::send_ready_message (Test::Receiver_ptr receiver)
{
  ACE_DEBUG ((LM_DEBUG,
              "Received a call ...\n"));

  ACE_DEBUG ((LM_DEBUG,
              "Calling the Receiver ..\n"));

  for (CORBA::ULong i = 0;
       i < this->number_;
       ++i)
    {
      receiver->receive_call ();
    }

}

void
Sender_i::ping ( /**/)
{
  return;
}

void
Sender_i::shutdown (void)
{
  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) About to invoke shutdown...\n"));
  this->orb_->shutdown (0);
}
