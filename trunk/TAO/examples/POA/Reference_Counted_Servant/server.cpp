// $Id$

// ================================================================
//
// = LIBRARY
//    TAO/tests/POA/Reference_Counted_Servant
//
// = FILENAME
//    server.cpp
//
// = DESCRIPTION
//    This example shows how to use reference counted servants to
//    automatically manage dynamic memory for servants.
//    Stubs/Skeletons and client code is available in
//    ../Generic_Servant.
//
// = AUTHOR
//    Irfan Pyarali
//
// ================================================================

#include "ace/Get_Opt.h"
#include "Reference_Counted_Foo.h"

ACE_RCSID(Reference_Counted_Servant, server, "$Id$")

static char *ior_output_file = 0;

static int
parse_args (int argc, char **argv)
{
  ACE_Get_Opt get_opts (argc, argv, "f:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'f':
        ior_output_file = get_opts.optarg;
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "[-f ior_output_file]"
                           "\n",
                           argv [0]),
                          -1);
      }

  // Indicates successful parsing of command line.
  return 0;
}

static int
write_iors_to_file (const char *ior)
{
  if (ior_output_file == 0)
    // No filename was specified; simply return
    return 0;

  FILE *output_file = ACE_OS::fopen (ior_output_file, "w");

  if (output_file == 0)
    ACE_ERROR_RETURN ((LM_ERROR, "Cannot open output files for writing IORs: %s, %s %s\n",
                       ior_output_file),
                      -1);

  u_int result = 0;

  result = ACE_OS::fprintf (output_file,
                            "%s",
                            ior);
  if (result != ACE_OS::strlen (ior))
    ACE_ERROR_RETURN ((LM_ERROR,
                       "ACE_OS::fprintf failed while writing %s to %s\n",
                       ior,
                       ior_output_file),
                      -1);

  ACE_OS::fclose (output_file);

  return 0;
}

int
main (int argc, char **argv)
{
  ACE_DECLARE_NEW_CORBA_ENV;

  ACE_TRY
    {
      // Initialize the ORB first.
      CORBA::ORB_var orb = CORBA::ORB_init (argc,
                                            argv,
                                            0,
                                            ACE_TRY_ENV);
      ACE_TRY_CHECK;

      int result = parse_args (argc, argv);
      if (result != 0)
        return result;

      // Obtain the RootPOA.
      CORBA::Object_var obj = orb->resolve_initial_references ("RootPOA");

      // Get the POA_var object from Object_var.
      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (obj.in (),
                                      ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // Get the POAManager of the RootPOA.
      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // Create a servant.
      Reference_Counted_Foo *foo_impl = 0;
      ACE_NEW_RETURN (foo_impl,
                      Reference_Counted_Foo (orb.in (),
                                             root_poa.in (),
                                             27),
                      -1);

      // Get Object Reference for the foo_impl object.
      Foo_var foo = foo_impl->_this (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // This means that the ownership of <foo_impl> now belongs to
      // the POA.
      foo_impl->_remove_ref (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // Stringyfy all the object references and print them out.
      CORBA::String_var ior = orb->object_to_string (foo.in (), ACE_TRY_ENV);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG,
                  "%s\n",
                  ior.in ()));

      int write_result = write_iors_to_file (ior.in ());
      if (write_result != 0)
        return write_result;

      poa_manager->activate (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      if (orb->run () == -1)
        ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "CORBA::ORB::run"), -1);

      // Destroy RootPOA (also destroys all child POAs).
      root_poa->destroy (1,
                         1,
                         ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "Exception caught");
      return -1;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (-1);

  return 0;
}
