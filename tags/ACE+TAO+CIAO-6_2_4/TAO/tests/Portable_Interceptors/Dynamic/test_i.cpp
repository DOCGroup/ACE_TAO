// $Id$

#include "test_i.h"

Visual_i::Visual_i (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}
  // ctor

void
Visual_i::normal (CORBA::Long arg,
                  CORBA::String_out msg)
{
  ACE_DEBUG ((LM_DEBUG, "Visual::normal called with %d\n", arg));
  msg = CORBA::string_dup ("DO_NOT_INSULT_MY_INTELLIGENCE");
}

CORBA::Long
Visual_i::calculate (CORBA::Long one,
                     CORBA::Long two)
{
  ACE_DEBUG ((LM_DEBUG, "Visual::calculate\n"));
  return (one + two);
}

Test_Interceptors::Visual::VarLenStruct *
Visual_i::the_structure (void)
{
  Test_Interceptors::Visual::VarLenStruct * s;

  ACE_NEW_THROW_EX (s,
                    Test_Interceptors::Visual::VarLenStruct,
                    CORBA::NO_MEMORY ());

  Test_Interceptors::Visual::VarLenStruct_var safe_s = s;

  s->flag    = 135;
  s->message = CORBA::string_dup ("Oh Captain, my Captain!");

  return safe_s._retn ();
}

void
Visual_i::user (void)
{
  ACE_DEBUG ((LM_DEBUG, "Visual::user, throwing Silly\n"));
  throw Test_Interceptors::Silly ();
}

void
Visual_i::system (void)
{
  ACE_DEBUG ((LM_DEBUG, "Visual::user, throwing INV_OBJREF\n"));
  throw CORBA::INV_OBJREF ();
}

void
Visual_i::shutdown (void)
{
  this->orb_->shutdown ();
}
