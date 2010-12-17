// $Id$

// ================================================================
// LIBRARY
//   libTAO_CosConcurrency.so
//
// DESCRIPTION
//   This class allows for dynamically loading
//   the Concurrency Service.
//
// AUTHORS
//   Priyanka Gontla <pgontla@ece.uci.edu>
//   Jaiganesh Balasubramanian <jai@doc.ece.uci.edu>
// ================================================================

#include "orbsvcs/Concurrency/Concurrency_Loader.h"
#include "ace/Dynamic_Service.h"



TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Concurrency_Loader::TAO_Concurrency_Loader (void)
{
}

TAO_Concurrency_Loader::~TAO_Concurrency_Loader (void)
{
}

int
TAO_Concurrency_Loader::init (int argc, ACE_TCHAR *argv[])
{
  try
    {
      // Initialize the ORB
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      // This function call initializes the Concurrency Service
      CORBA::Object_var object =
        this->create_object (orb.in (), argc, argv);
    }
  catch (const CORBA::Exception&)
    {
      // @@ Should we log this???
      return -1;
    }
  return 0;
}

int
TAO_Concurrency_Loader::fini (void)
{
  // Remove the Concurrency Service.
  this->concurrency_server_.fini();
  return 0;
}

CORBA::Object_ptr
TAO_Concurrency_Loader::create_object (CORBA::ORB_ptr orb,
                                       int /* argc */,
                                       ACE_TCHAR * /* argv */ [])
{
  CORBA::Object_var object =
    orb->resolve_initial_references ("RootPOA");
  PortableServer::POA_var poa =
    PortableServer::POA::_narrow (object.in ());
  PortableServer::POAManager_var poa_manager =
    poa->the_POAManager ();
  poa_manager->activate ();

  return this->concurrency_server_.init (orb, poa.in ());
}

TAO_END_VERSIONED_NAMESPACE_DECL

ACE_FACTORY_DEFINE (TAO_Concurrency_Serv, TAO_Concurrency_Loader)
