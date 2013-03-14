// $Id$

#include "test_i.h"

test_i::test_i (CORBA::ORB_ptr orb)
  : orb_ (orb)
{
}

CORBA::Any *
test_i::get_value (CORBA::Long offset)
{
  CORBA::Any_ptr retval_ptr = 0;
  ACE_NEW_THROW_EX (retval_ptr,
                    CORBA::Any,
                    CORBA::NO_MEMORY ());
  CORBA::Any_var retval = retval_ptr;

  switch (offset)
  {
    case 1:
    {
      obvmod::tcOffset1 *vt1 = 0;
      ACE_NEW_RETURN (vt1,
                      tcOffset1_i,
                      0);
      vt1->mem (offset);
      retval.inout () <<= &vt1;
      break;
    }
    case 3:
    {
      obvmod::tcOffset003 *vt3 = 0;
      ACE_NEW_RETURN (vt3,
                      tcOffset3_i,
                      0);
      vt3->mem (offset);
      retval.inout () <<= &vt3;
      break;
    }
    case 5:
    {
      obvmod::tcOffset00005 *vt5 = 0;
      ACE_NEW_RETURN (vt5,
                      tcOffset5_i,
                      0);
      vt5->mem (offset);
      retval.inout () <<= &vt5;
      break;
    }
    case 7:
    {
      obvmod::tcOffset0000007 *vt7 = 0;
      ACE_NEW_RETURN (vt7,
                      tcOffset7_i,
                      0);
      vt7->mem (offset);
      retval.inout () <<= &vt7;
      break;
    }
    default:
      return 0;
  }

  return retval._retn ();
}

void
test_i::shutdown (void)
{
  this->orb_->shutdown ();
}
