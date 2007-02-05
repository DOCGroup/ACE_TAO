//
// $Id$
//
#include "LongDoubleTest.h"
#include "Global.h"

ACE_RCSID (LongDoubleTest, LongDoubleTest, "$Id$")

LongDoubleTest::LongDoubleTest (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

CORBA::LongDouble
LongDoubleTest::get_long_double ()
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  CORBA::LongDouble ld;
  ACE_CDR_LONG_DOUBLE_ASSIGNMENT (ld, Global::get_long_double ());
  return ld;
}

void
LongDoubleTest::shutdown ()
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->orb_->shutdown (0);
}
