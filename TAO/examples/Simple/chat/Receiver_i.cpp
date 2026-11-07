
//=============================================================================
/**
 *  @file    Receiver_i.cpp
 *
 *  Implements the Receiver_i class, which is used by the chat client.
 *
 *  @author Pradeep Gore <pradeep@cs.wustl.edu>
 */
//=============================================================================


#include "Receiver_i.h"

void
Receiver_i::message (char const *msg)
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT (": %C\n"),
              msg));
}

void
Receiver_i::shutdown ()
{
  // Instruct the ORB to shutdown.
  this->orb_->shutdown ();
}

void
Receiver_i::orb (CORBA::ORB_ptr o)
{
  this->orb_ = CORBA::ORB::_duplicate (o);
}
