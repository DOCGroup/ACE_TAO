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

#include "Concurrency_Loader.h"
#include "ace/Dynamic_Service.h"

ACE_RCSID (Concurrency, Concurrency_Loader, "$Id$")

TAO_Concurrency_Loader::TAO_Concurrency_Loader (void)
{
  // Constructor
}

TAO_Concurrency_Loader::~TAO_Concurrency_Loader (void)
{
  // Destructor
}

int
TAO_Concurrency_Loader::init (int argc, char *argv[])
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      // Initialize the ORB
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, 0 ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // This function call initializes the Concurrency Service
      CORBA::Object_var object =
        this->create_object (orb.in (), argc, argv ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      // @@ Should we log this???
      return -1;
    }
  ACE_ENDTRY;
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
                                       char * /* argv */ []
                                       ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  CORBA::Object_var object =
    orb->resolve_initial_references ("RootPOA" ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::Object::_nil ());
  PortableServer::POA_var poa =
    PortableServer::POA::_narrow (object.in () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::Object::_nil ());
  PortableServer::POAManager_var poa_manager =
    poa->the_POAManager (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::Object::_nil ());
  poa_manager->activate (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::Object::_nil ());

  return this->concurrency_server_.init (orb, poa.in ());
}

ACE_FACTORY_DEFINE (TAO_Concurrency, TAO_Concurrency_Loader)

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Dynamic_Service<TAO_Concurrency_Loader>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Dynamic_Service<TAO_Concurrency_Loader>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
