// $Id$

#include "AnyS_impl.h"

Test_impl::Test_impl (CORBA::ORB_ptr orb)
    : orb_ (CORBA::ORB::_duplicate (orb))
{
}

CORBA::Any*
Test_impl::get_something (
    CORBA::Boolean need_vb)
{
  CORBA::Any_ptr ret_val_ptr = 0;
  ACE_NEW_RETURN (ret_val_ptr, CORBA::Any, 0);
  CORBA::Any_var ret_val = ret_val_ptr;

  const CORBA::ULong magic = 3145;

  if (need_vb)
    {
      OBV_AnyTest::VB_var vb;
      ACE_NEW_RETURN (vb.inout (), OBV_OBV_AnyTest::VB, 0);

      vb->id (magic);

      ret_val.inout () <<= vb.in ();
    }
  else
    {
      OBV_AnyTest::VA_var va;
      ACE_NEW_RETURN (va.inout (), OBV_OBV_AnyTest::VA, 0);

      va->id (magic);

      OBV_AnyTest::VA *va_ptr = va._retn ();
      ret_val.inout () <<= &va_ptr;
    }

  return ret_val._retn();
}

OBV_AnyTest::VB *
Test_impl::get_vb()
{
  const CORBA::ULong magic = 3145;

  OBV_AnyTest::VB_var vb;
  ACE_NEW_RETURN (vb.inout (), OBV_OBV_AnyTest::VB, 0);

  vb->id (magic);
  return vb._retn();
}

void
Test_impl::shutdown (void)
{
  this->orb_->shutdown (0);
}
