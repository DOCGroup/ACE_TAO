// $Id$

// ================================================================
// LIBRARY
//   libTAO_CosNaming.so
//
// DESCRIPTION
//   This class allows for dynamically loading
//   the Naming Service.
//
// AUTHORS
//   Priyanka Gontla <pgontla@ece.uci.edu>
//   Carlos O'Ryan <coryan@uci.edu>
// ================================================================

#include "orbsvcs/Naming/Naming_Loader.h"
#include "ace/Dynamic_Service.h"
#include "ace/Argv_Type_Converter.h"

#include "orbsvcs/Naming/Naming_Server.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Naming_Loader::TAO_Naming_Loader (TAO_Naming_Server *server)
: naming_server_(server)
{
  // Constructor

  // If no server was provided, then we will construct one of the
  // base class type.
  if (naming_server_ == 0)
    ACE_NEW (naming_server_, TAO_Naming_Server);
}

TAO_Naming_Loader::~TAO_Naming_Loader (void)
{
  // Destroy the naming server that was created
  delete naming_server_;
}

int
TAO_Naming_Loader::init (int argc, ACE_TCHAR *argv[])
{
  try
    {
      // Copy command line parameter.
      ACE_Argv_Type_Converter command_line(argc, argv);

      // Initialize the ORB
      CORBA::ORB_var orb =
        CORBA::ORB_init (command_line.get_argc(),
                         command_line.get_TCHAR_argv());

      // This function call initializes the Naming Service
      CORBA::Object_var object =
        this->create_object (orb.in (),
                             command_line.get_argc(),
                             command_line.get_TCHAR_argv ());
    }
  catch (const CORBA::Exception&)
    {
      // @@ Should we log this???
      return -1;
    }
  return 0;
}

int
TAO_Naming_Loader::fini (void)
{
  // Remove the Naming Service.
  return this->naming_server_->fini ();
}

CORBA::Object_ptr
TAO_Naming_Loader::create_object (CORBA::ORB_ptr orb,
                                  int argc,
                                  ACE_TCHAR *argv[])
{

  if (this->naming_server_ == 0)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("TAO_Naming_Loader::create_object - naming_server_ ")
                  ACE_TEXT ("never set.\n")));
    }
  else
    {
      // Initializes the Naming Service. Returns -1
      // on an error.
      if (this->naming_server_->init_with_orb (argc, argv, orb) == -1)
        return CORBA::Object::_nil ();
    }

  return CORBA::Object::_nil ();
}

TAO_END_VERSIONED_NAMESPACE_DECL

ACE_FACTORY_DEFINE (TAO_Naming_Serv, TAO_Naming_Loader)
