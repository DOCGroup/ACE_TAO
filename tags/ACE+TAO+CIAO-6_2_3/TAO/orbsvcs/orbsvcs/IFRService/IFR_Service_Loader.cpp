// $Id$

// ================================================================
// DESCRIPTION
//   This class allows for dynamically loading
//   the IFR Service.
//
// AUTHORS
//   Jaiganesh Balasubramanian <jai@doc.ece.uci.edu>
//   Priyanka Gontla <pgontla@ece.uci.edu>
// ================================================================

#include "orbsvcs/IFRService/IFR_Service_Loader.h"
#include "ace/Dynamic_Service.h"
#include "ace/Argv_Type_Converter.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_IFR_Service_Loader::TAO_IFR_Service_Loader (void)
{
  // Constructor
}

TAO_IFR_Service_Loader::~TAO_IFR_Service_Loader (void)
{
  // Destructor
}

int
TAO_IFR_Service_Loader::init (int argc,
                              ACE_TCHAR *argv[])
{
  try
    {
      // Copy command line parameter.
      ACE_Argv_Type_Converter command_line (argc, argv);

      // Initialize the ORB
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      // This function call initializes the IFR_Service Service
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
TAO_IFR_Service_Loader::fini (void)
{
  // Remove the IFR_Service Service.
  return this->ifr_server_.fini ();
}

CORBA::Object_ptr
TAO_IFR_Service_Loader::create_object (CORBA::ORB_ptr orb,
                                       int argc,
                                       ACE_TCHAR *argv[])
{
  // Initializes the IFR_Service Service. Returns -1
  // on an error.
  int result = this->ifr_server_.init_with_orb (argc,
                                                argv,
                                                orb);
  if (result != 0)
    {
      throw CORBA::BAD_PARAM (0, CORBA::COMPLETED_NO);
    }

  return CORBA::Object::_nil ();
}

TAO_END_VERSIONED_NAMESPACE_DECL

ACE_FACTORY_DEFINE (TAO_IFRService, TAO_IFR_Service_Loader)
