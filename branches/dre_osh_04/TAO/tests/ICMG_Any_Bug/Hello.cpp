//
// $Id$
//
#include "Hello.h"
#include "HelloWorld.h"

ACE_RCSID(ICMG_Any_Bug, Hello, "$Id")

Hello::Hello (CORBA::ORB_ptr orb, PortableServer::POA_ptr poa)
  : poa_ (PortableServer::POA::_duplicate (poa)),
    orb_ (CORBA::ORB::_duplicate (orb))
{
}

Test::HelloWorld_ptr
Hello::get_helloworld (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  HelloWorld *hello_world;
  ACE_NEW_THROW_EX (hello_world,
          HelloWorld,
		    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (Test::HelloWorld::_nil ());

  PortableServer::ObjectId_var oid =
    poa_->activate_object (hello_world
                           ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (Test::HelloWorld::_nil ());

  Test::HelloWorld_var hw = hello_world->_this ();
  return hw._retn ();
}

void
Hello::shutdown (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->orb_->shutdown (0 ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}
