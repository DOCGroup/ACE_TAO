#include "Test_i.h"

BoundSequences::BoundSequences (CORBA::ORB_ptr const orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

::CORBA::ULong
BoundSequences::SendSequenceOf10Long (
  ::CORBA::ULong const length_sent,
  ::Test::BoundSequences::SequenceOf10Long const &in_seq)
{
  ACE_DEBUG ((LM_DEBUG,
              "Server - BoundSequences::SendSequenceOf10Long (%u, length(%u)) is %C\n",
              length_sent, in_seq.length (),
              ((length_sent == in_seq.length ()) ? "Correct" : "**** Incorrect ****")));
  return in_seq.length ();
}

void
BoundSequences::shutdown ()
{
  this->orb_->shutdown (false);
}
