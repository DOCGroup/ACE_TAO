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

#include "IFR_Service_Loader.h"
#include "ace/Dynamic_Service.h"
#include "ace/Argv_Type_Converter.h"

ACE_RCSID (IFR_Service, 
           IFR_Service_Loader, 
           "$Id$")

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
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      // Copy command line parameter.
      ACE_Argv_Type_Converter command_line (argc, 
                                            argv);

      // Initialize the ORB
      CORBA::ORB_var orb =
        CORBA::ORB_init (command_line.get_argc (), 
                         command_line.get_ASCII_argv (), 
                         0 
                         ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // This function call initializes the IFR_Service Service
      CORBA::Object_var object =
        this->create_object (orb.in (), 
                             command_line.get_argc (), 
                             command_line.get_TCHAR_argv () 
                             ACE_ENV_ARG_PARAMETER);
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
TAO_IFR_Service_Loader::fini (void)
{
  // Remove the IFR_Service Service.
  return this->ifr_server_.fini ();
}

CORBA::Object_ptr
TAO_IFR_Service_Loader::create_object (CORBA::ORB_ptr orb,
                                       int argc,
                                       ACE_TCHAR *argv[]
                                       ACE_ENV_ARG_DECL_NOT_USED)
   ACE_THROW_SPEC ((CORBA::SystemException))
{
  int result;

  // Initializes the IFR_Service Service. Returns -1
  // on an error.
  result = this->ifr_server_.init_with_orb (argc,
                                            argv,
                                            orb);
  if (result == -1)
    {
      return CORBA::Object::_nil ();
    }

  return 0;
}

ACE_FACTORY_DEFINE (TAO_IFRService, TAO_IFR_Service_Loader)

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Dynamic_Service<TAO_IFR_Service_Loader>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Dynamic_Service<TAO_IFR_Service_Loader>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
