// $Id$

// ===========================================================
//
// = LIBRARY
//    TAO/tests/Simple/chat
//
// = FILENAME
//    Receiver_i.cpp
//
// = DESCRIPTION
//    Implements the Receiver_i class, which is used by the chat client.
//
// = AUTHOR
//    Pradeep Gore <pradeep@cs.wustl.edu>
//
// ===========================================================

#include "Receiver_i.h"

Receiver_i::Receiver_i (void)
  : orb_ (0)
{
}

Receiver_i::~Receiver_i (void)
{
}

void
Receiver_i::message (const char *msg,
                     CORBA::Environment &)
{
  ACE_DEBUG ((LM_DEBUG,
	      ": %s\n",
	      msg));
}

void
Receiver_i::shutdown (CORBA::Environment &)
{
  // Instruct the ORB to shutdown.
  this->orb_->shutdown ();
}

void
Receiver_i::orb (CORBA::ORB_ptr o)
{
  this->orb_ = CORBA::ORB::_duplicate (o);
}
