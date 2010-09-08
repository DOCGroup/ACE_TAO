//
// $Id$
//
#include "tao/ORB_Core.h"
#include "tao/debug.h"
#include "tao/Transport_Cache_Manager.h"
#include "Object_Factory_i.h"
#include "ace/Synch.h"

Object_Factory_i::Object_Factory_i (CORBA::ORB_ptr orb, CORBA::ULong len)
  : orb_ (CORBA::ORB::_duplicate (orb)),
    length_(len)
{
}

//factory method to create first object
Two_Objects_Test::First_ptr
Object_Factory_i::create_first (void)
{
  First_i *first_impl;

  ACE_NEW_THROW_EX (first_impl,
                    First_i (orb_.in(), two_way_done_ ),
                    CORBA::NO_MEMORY() );
  // Ownership is transfered to the POA.
  PortableServer::ServantBase_var safe (first_impl);

  CORBA::Object_var poa_object =
    this->orb_->resolve_initial_references("RootPOA");

  PortableServer::POA_var root_poa =
    PortableServer::POA::_narrow (poa_object.in ());

  PortableServer::ObjectId_var id =
    root_poa->activate_object (first_impl);

  CORBA::Object_var object = root_poa->id_to_reference (id.in ());

  Two_Objects_Test::First_var first =
    Two_Objects_Test::First::_narrow (object.in ());

  return first._retn();
}

//factory method to create second object
Two_Objects_Test::Second_ptr
Object_Factory_i::create_second (void)
{
  Second_i *second_impl;

  ACE_NEW_THROW_EX (second_impl,
                    Second_i (orb_.in(),
                              length_, two_way_done_),
                    CORBA::NO_MEMORY ());
  // Ownership is transfered to the POA.
  PortableServer::ServantBase_var safe (second_impl);

  CORBA::Object_var poa_object =
    this->orb_->resolve_initial_references("RootPOA");

  PortableServer::POA_var root_poa =
    PortableServer::POA::_narrow (poa_object.in ());

  PortableServer::ObjectId_var id =
    root_poa->activate_object (second_impl);

  CORBA::Object_var object = root_poa->id_to_reference (id.in ());

  Two_Objects_Test::Second_var second =
    Two_Objects_Test::Second::_narrow (object.in ());

  return second._retn();
}
