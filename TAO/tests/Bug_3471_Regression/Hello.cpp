#include "Hello.h"

Hello::Hello (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

Test::ObjectSeq *
Hello::get_objects ()
{
  Test::ObjectSeq_var my_seq = new Test::ObjectSeq (1);
  my_seq->length (1);

  (*my_seq)[0] = CORBA::Object::_duplicate (CORBA::Object::_nil ());
  // The generated operation signature returns ownership to the caller.
  return my_seq._retn ();
}

void
Hello::shutdown ()
{
  this->orb_->shutdown (false);
}
