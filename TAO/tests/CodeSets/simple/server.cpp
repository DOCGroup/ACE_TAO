// -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests/CodeSets/simple
//
// = FILENAME
//    client.cpp
//
// = DESCRIPTION
//    A simple client to demonstrate the use of codeset translation
//
// = AUTHORS
//    Phil Mesnier <mesnier_p@ociweb.com>
//
// ============================================================================

// IDL generated header
#include "simpleS.h"
#include "ace/streams.h"


// ------------------------------------------------------------
// Servant for associated CORBA object
// ------------------------------------------------------------
class SimpleImpl : public POA_simple
{
public:

  // implementation of corba interface
  char * op1 (const char * name,
              const CORBA::Any & inany,
              CORBA::Any_out outany
              ACE_ENV_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException))
  {
    cout << "Server: bare string: " << name << endl;
    const char *any_str;
    inany >>= any_str;
    cout << "Server: inserted string: " << any_str << endl << endl;

    CORBA::Any *out_ptr = 0;
    ACE_NEW_RETURN (out_ptr,
                    CORBA::Any,
                    0);
    (*out_ptr) <<= CORBA::string_dup (any_str);
    outany = out_ptr;

    return CORBA::string_dup (name);
  };

  ACE_CDR::WChar * op2 (const ACE_CDR::WChar *s1
                        ACE_ENV_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException))
  {
    return CORBA::wstring_dup (s1);
  };
};

// ------------------------------------------------------------
// Main routine
// ------------------------------------------------------------
int main(int argc, char *argv[])
{

  ACE_TRY_NEW_ENV
    {
      // Init the orb
      CORBA::ORB_var orb= CORBA::ORB_init (argc,
                                           argv,
                                           ""
                                           ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Initialize POA
      CORBA::Object_var poa_object=
        orb->resolve_initial_references ("RootPOA"
                                         ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Check POA
      if (CORBA::is_nil (poa_object.in ()))
        {
          cout << "Couldn't initialize POA" << endl;
          return 1;
        }

      // Get the ROOT POA
      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (poa_object.in ()
                                      ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Get the POA manager
      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Create a C++ implementation of CORBA object
      SimpleImpl* my_impl = 0;
      ACE_NEW_RETURN (my_impl,
                      SimpleImpl,
                      -1);

      // Create CORBA object for servant and REGISTER with POA
      simple_var server = my_impl->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Get the IOR for our object
      CORBA::String_var ior = orb->object_to_string (server.in ()
                                                     ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ofstream fstr;
      fstr.open ("server.ior");

      if (fstr.bad ())
        {
          cout << "Cannot open server.ior!" << endl;
          exit (1);
        }
      else
        {
          fstr << ior.in () << endl;
        }

      // Activate POA manager
      poa_manager->activate (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Wait for calls
      orb->run (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      root_poa->destroy (1, 1 ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      orb->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Exception caught:");
      return 1;
    }
  ACE_ENDTRY;

  return 0;
}
