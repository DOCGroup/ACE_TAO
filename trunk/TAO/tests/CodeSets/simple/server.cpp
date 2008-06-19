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

// FUZZ: disable check_for_streams_include
#include "ace/streams.h"

#include "ace/OS_NS_stdio.h"

// ------------------------------------------------------------
// Servant for associated CORBA object
// ------------------------------------------------------------
class SimpleImpl : public POA_simple
{
public:
  SimpleImpl (CORBA::ORB_ptr orb)
    : orb_ (CORBA::ORB::_duplicate (orb))
  {}

  // implementation of corba interface
  char * op1 (const char * name,
              const CORBA::Any & inany,
              CORBA::Any_out outany)
  {
    ACE_DEBUG ((LM_DEBUG,
                "Server: bare string: %s\n", name));
    const char *any_str;
    inany >>= any_str;
    ACE_DEBUG ((LM_DEBUG,
                "Server: inserted string: %s\n\n", any_str));

    CORBA::Any *out_ptr = 0;
    ACE_NEW_RETURN (out_ptr,
                    CORBA::Any,
                    0);
    (*out_ptr) <<= any_str;
    outany = out_ptr;

    return CORBA::string_dup (name);
  };

  ACE_CDR::WChar * op2 (const ACE_CDR::WChar *s1)
  {
    return CORBA::wstring_dup (s1);
  };

  //FUZZ: disable check_for_lack_ACE_OS
  void shutdown (void)
  {
    this->orb_->shutdown (0);
  };
  //FUZZ: enable check_for_lack_ACE_OS

private:
  /// Use an ORB reference to shutdown
  /// the application.
  CORBA::ORB_var orb_;
};

// ------------------------------------------------------------
// Main routine
// ------------------------------------------------------------
int main(int argc, char *argv[])
{

  try
    {
      // Init the orb
      CORBA::ORB_var orb= CORBA::ORB_init (argc,
                                           argv);

      // Initialize POA
      CORBA::Object_var poa_object=
        orb->resolve_initial_references ("RootPOA");

      // Check POA
      if (CORBA::is_nil (poa_object.in ()))
        {
          ACE_DEBUG ((LM_DEBUG,
                     "Couldn't initialize POA\n"));
          return 1;
        }

      // Get the ROOT POA
      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (poa_object.in ());

      // Get the POA manager
      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager ();

      // Create a C++ implementation of CORBA object
      SimpleImpl* my_impl = 0;
      ACE_NEW_RETURN (my_impl,
                      SimpleImpl (orb.in ()),
                      -1);

      // Create CORBA object for servant and REGISTER with POA
      PortableServer::ObjectId_var id =
        root_poa->activate_object (my_impl);

      CORBA::Object_var object_act = root_poa->id_to_reference (id.in ());

      simple_var server = simple::_narrow (object_act.in ());

      // Get the IOR for our object
      CORBA::String_var ior = orb->object_to_string (server.in ());

      FILE *output_file= ACE_OS::fopen ("server.ior", "w");
      if (output_file == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Cannot open output file for writing IOR: %s",
                           "server.ior"),
                              1);
      ACE_OS::fprintf (output_file, "%s", ior.in ());
      ACE_OS::fclose (output_file);

      // Activate POA manager
      poa_manager->activate ();

      // Wait for calls
      orb->run ();

      root_poa->destroy (1, 1);

      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught in server:");
      return 1;
    }

  return 0;
}
