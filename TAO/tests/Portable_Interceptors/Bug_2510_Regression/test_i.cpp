// test_i.cpp,v 1.1 2003/08/27 22:43:35 edwardgt Exp

#include "test_i.h"
#include "tao/PI/PI.h"

extern PortableInterceptor::SlotId slotId;

Visual_i::Visual_i (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

void
Visual_i::normal (CORBA::Long arg)
{
  ACE_DEBUG ((LM_DEBUG, "Visual::normal called with %d\n", arg));

  // retrieve Slot
  CORBA::Object_var piobj = orb_->resolve_initial_references ("PICurrent");
  PortableInterceptor::Current_var pi_current =
      PortableInterceptor::Current::_narrow (piobj.in () );

  if (CORBA::is_nil (pi_current.in ()))
  {
    ACE_DEBUG ((LM_DEBUG, "Visual_i::normal : Unable to obtain PICurrent reference\n"));
    throw CORBA::INTERNAL();
  }

  CORBA::Any_var retrieved_any;
  try
  {
      retrieved_any = pi_current->get_slot(slotId);
  }
  catch (const CORBA::Exception& ex)
  {
      ex._tao_print_exception (
        "Visual_i::normal : get_slot() threw Exception\n");
      throw;
  }

  const char *str = 0;
  if (! (retrieved_any.in() >>= str) )
  {
    ACE_DEBUG ((LM_DEBUG, "Visual_i::normal : Problem extracting data from CORBA::Any\n"));
    throw CORBA::INTERNAL();
  }
  else
  {
    ACE_DEBUG ((LM_DEBUG, "Visual_i::normal : extracted %C data from CORBA::Any\n",str));
  }
}

void
Visual_i::shutdown (void)
{
  this->orb_->shutdown (0);
}
