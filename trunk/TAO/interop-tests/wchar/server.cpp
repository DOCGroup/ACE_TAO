// -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//    interop_test/wchar
//
// = FILENAME
//    server.cpp
//
// = DESCRIPTION
//    C++ server side for testing interoperability with wchar data.
//
// = AUTHOR
//    Phil Mesnier <mesnier_p@ociweb.com>
//
// ============================================================================

#include "interop_wchar_i.h"
#include <ace/streams.h>
#include <ace/Get_Opt.h>

const char *ior_output_file = "IOR";
int verbose = 0;

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "o:v");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':
        ior_output_file = get_opts.opt_arg ();
        break;
      case 'v':
        verbose = 1;
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-o <iorfile> "
                           "-v "
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates sucessful parsing of the command line
  return 0;
}

int
main( int argc, char *argv[] )
{
  if (parse_args(argc, argv))
    ACE_ERROR_RETURN ((LM_ERROR, "failed to parse args\n"), 1);
  try
    {
      // Initialize orb
      CORBA::ORB_var orb = CORBA::ORB_init( argc, argv );

      //Get reference to Root POA
      CORBA::Object_var obj = orb->resolve_initial_references( "RootPOA" );
      PortableServer::POA_var poa = PortableServer::POA::_narrow( obj.in() );

      PortableServer::POAManager_var mgr = poa->the_POAManager();

      // Activate POA Manager
      mgr->activate();

      // Create an object
      interop_WChar_Passer_i servant(orb.in(), verbose);

      // Register the servant with the RootPOA, obtain its object
      // reference, stringify it, and write it to a file.
      obj = poa->servant_to_reference( &servant );

      CORBA::String_var str = orb->object_to_string( obj.in() );

      if (parse_args(argc, argv))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "failed to parse args"),
                            1);
        }

      FILE *output_file= ACE_OS::fopen (ior_output_file, "w");
      if (output_file == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Cannot open output file for writing IOR: %s",
                           ior_output_file),
                          1);
      ACE_OS::fprintf (output_file, "%s", str.in ());
      ACE_OS::fclose (output_file);

      // Accept requests
      orb->run();
      orb->destroy();
    }

    catch( const CORBA::Exception &ex ) {
      ACE_PRINT_EXCEPTION(ex, "uncaught exception");
      return 1;
    }

    return 0;
}
