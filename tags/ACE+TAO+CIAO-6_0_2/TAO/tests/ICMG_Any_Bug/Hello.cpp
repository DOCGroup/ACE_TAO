//
// $Id$
//
#include "Hello.h"
#include "HelloWorld.h"

Hello::Hello (CORBA::ORB_ptr orb, PortableServer::POA_ptr poa)
  : poa_ (PortableServer::POA::_duplicate (poa)),
    orb_ (CORBA::ORB::_duplicate (orb))
{
}

Test::HelloWorld_ptr
Hello::get_helloworld (void)
{
  HelloWorld *hello_world;
  ACE_NEW_THROW_EX (hello_world,
                    HelloWorld,
                    CORBA::NO_MEMORY ());

  PortableServer::ObjectId_var id =
    this->poa_->activate_object (hello_world);

  CORBA::Object_var object = this->poa_->id_to_reference (id.in ());

  Test::HelloWorld_var hw = Test::HelloWorld::_narrow (object.in ());
  return hw._retn ();
}

void
Hello::shutdown (void)
{
  this->orb_->shutdown (0);
}
