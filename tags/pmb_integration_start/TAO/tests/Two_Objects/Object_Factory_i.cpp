//
// $Id$
//
#include "tao/ORB_Core.h"
#include "tao/debug.h"
#include "tao/Transport_Cache_Manager.h"
#include "Object_Factory_i.h"
#include "ace/Synch.h"

ACE_RCSID(Test, Test, "$Id$")


Object_Factory_i::Object_Factory_i (CORBA::ORB_ptr orb, CORBA::ULong len)
  : orb_ (CORBA::ORB::_duplicate (orb)),
    length_(len)
{
}

//factory method to create first object
Two_Objects_Test::First_ptr
Object_Factory_i::create_first (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException))
{
  First_i *first_impl;

  // @@ Dont you want to transfer ownership to the POA?
  ACE_NEW_THROW_EX (first_impl,
                    First_i (orb_.in(), two_way_done_ ),
                    CORBA::NO_MEMORY() );

  Two_Objects_Test::First_var first =
    first_impl->_this (ACE_ENV_SINGLE_ARG_PARAMETER );

  return first._retn();
}

//factory method to create second object
Two_Objects_Test::Second_ptr
Object_Factory_i::create_second (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException))
{
  Second_i *second_impl;

  ACE_NEW_THROW_EX (second_impl,
                    Second_i (orb_.in(),
                              length_, two_way_done_),
                    CORBA::NO_MEMORY ());

  Two_Objects_Test::Second_var second =
    second_impl->_this (ACE_ENV_SINGLE_ARG_PARAMETER);

  return second._retn();
}
