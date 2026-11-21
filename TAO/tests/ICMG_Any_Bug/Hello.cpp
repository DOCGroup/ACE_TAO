#include "Hello.h"
#include "HelloWorld.h"

Hello::Hello (CORBA::ORB_ptr orb, PortableServer::POA_ptr poa)
  : poa_ (PortableServer::POA::_duplicate (poa)),
    orb_ (CORBA::ORB::_duplicate (orb))
{
}

Test::HelloWorld_ptr
Hello::get_helloworld ()
{
  HelloWorld *hello_world {};
  ACE_NEW_THROW_EX (hello_world,
                    HelloWorld,
                    CORBA::NO_MEMORY ());
  // ServantBase_var takes ownership of this reference-counted TAO servant.
  PortableServer::ServantBase_var const owner_transfer (hello_world);

  PortableServer::ObjectId_var const id =
    this->poa_->activate_object (hello_world);

  CORBA::Object_var const object = this->poa_->id_to_reference (id.in ());

  Test::HelloWorld_var hw = Test::HelloWorld::_narrow (object.in ());
  return hw._retn ();
}

void
Hello::shutdown ()
{
  this->orb_->shutdown (false);
}
