//
// $Id$
//
#include "Hello.h"



CORBA::Boolean
Hello::normal_context_ok_ = 0;

CORBA::Boolean
Hello::extended_context_ok_ = 0;

Hello::Hello (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

void
Hello::shutdown ()
{
  this->orb_->shutdown (0 );
}

void
Hello::check_normal_context ()
{
  if (! normal_context_ok_)
    throw Test::MyException ();
}

void
Hello::check_extended_context ()
{
  if (! extended_context_ok_)
    throw Test::MyException ();
}
