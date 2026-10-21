#include "Test_i.h"

BoundSequences::BoundSequences (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

::CORBA::ULong
BoundSequences::SendSequenceOf10Long (
  ::CORBA::ULong const LengthSent,
  ::Test::BoundSequences::SequenceOf10Long const &inSeq)
{
  ACE_DEBUG ((LM_DEBUG,
              "Server - BoundSequences::SendSequenceOf10Long (%u, length(%u)) is %C\n",
              LengthSent, inSeq.length (),
              ((LengthSent == inSeq.length ()) ? "Correct" : "**** Incorrect ****")));
  return inSeq.length ();
}

void
BoundSequences::shutdown ()
{
  this->orb_->shutdown (false);
}
