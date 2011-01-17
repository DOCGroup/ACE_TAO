//
// $Id$
//
#include "Hello.h"

Hello::Hello (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

Test::ObjectSeq*
Hello::get_objects (void)
{
  Test::ObjectSeq_var my_seq = new Test::ObjectSeq (1);
  my_seq->length (1);

  (*my_seq)[0] = CORBA::Object::_duplicate (CORBA::Object::_nil ());
  return my_seq._retn ();
}

void
Hello::shutdown (void)
{
  this->orb_->shutdown (0);
}
