
//=============================================================================
/**
 *  @file     Servant_Locator.cpp
 *
 *  $Id$
 *
 *   Implementation of MyFooServantLocator class , used with a POA
 *   having a NON_RETAIN policy.
 *
 *
 *  @author  Irfan Pyarali  Michael Kircher
 */
//=============================================================================


#include "Servant_Locator.h"
#include "ace/Log_Msg.h"
#include "testC.h"
#include "ace/OS_NS_string.h"

Servant_Locator::Servant_Locator (CORBA::ORB_ptr orb_ptr)

  : orb_var_ (CORBA::ORB::_duplicate (orb_ptr))
{
  // no-op
}

Servant_Locator::~Servant_Locator ()
{
  //delete servant_ptr_;
}


PortableServer::Servant
Servant_Locator::preinvoke (const PortableServer::ObjectId &oid,
                            PortableServer::POA_ptr /* poa_ptr */,
                            const char * /*operation*/,
                            PortableServer::ServantLocator::Cookie & /* cookie */)
{

  CORBA::String_var s = PortableServer::ObjectId_to_string (oid);

  ACE_DEBUG ((LM_DEBUG,
              "The OID is <%s>\n", s.in ()));
  if (ACE_OS::strstr (s.in (), "Simple_Server") == 0)
    {
      throw CORBA::OBJECT_NOT_EXIST ();
    }

  // Combined IOR stuff
  Simple_Server_var server =
    Simple_Server::_narrow (this->objref_.in ());

  ACE_DEBUG ((LM_DEBUG,
              "Got the narrowed secondary server too...\n"));

  ACE_DEBUG ((LM_DEBUG,
              "About to throw exception..\n"));

  throw PortableServer::ForwardRequest (
    CORBA::Object::_duplicate (
      server.in ()));

}

void
Servant_Locator::postinvoke (const PortableServer::ObjectId &,
                             PortableServer::POA_ptr ,
                             const char *,
                             PortableServer::ServantLocator::Cookie ,
                             PortableServer::Servant)
{
}

void
Servant_Locator::set (CORBA::Object_var objref)
{
  this->objref_ = CORBA::Object::_duplicate (objref.in ());
}
