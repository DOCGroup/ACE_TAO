//
// $Id$
//
#include "Test_i.h"

BoundSequences::BoundSequences (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

::CORBA::ULong BoundSequences::SendSequenceOf10Long (const ::CORBA::ULong LengthSent, const ::Test::BoundSequences::SequenceOf10Long &inSeq)
{
  ACE_DEBUG ((LM_DEBUG,
              "Server - BoundSequences::SendSequenceOf10Long (%u, length(%u)) is %C\n",
              LengthSent, inSeq.length(),
              ((LengthSent == inSeq.length())? "Correct" : "**** Incorrect ****")));
  return inSeq.length();
}

void
BoundSequences::shutdown (void)
{
  this->orb_->shutdown (0);
}
