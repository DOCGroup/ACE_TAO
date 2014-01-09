
//=============================================================================
/**
 *  @file    server.cpp
 *
 *  $Id$
 *
 *  This example shows how to use reference counted servants to
 *  automatically manage dynamic memory for servants.
 *  Stubs/Skeletons and client code is available in
 *  ../Generic_Servant.
 *
 *
 *  @author Irfan Pyarali
 */
//=============================================================================


#include "ace/Get_Opt.h"
#include "test_i.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_string.h"

// This is to remove "inherits via dominance" warnings from MSVC.
// MSVC is being a little too paranoid.
#if defined (_MSC_VER)
# pragma warning (disable : 4250)
#endif /* _MSC_VER */

class reference_counted_test_i :
  public virtual test_i
{
public:
  /// Constructor - takes a POA and a value parameter
  reference_counted_test_i (CORBA::ORB_ptr orb,
                            PortableServer::POA_ptr poa);
};

reference_counted_test_i::reference_counted_test_i (CORBA::ORB_ptr orb,
                                                    PortableServer::POA_ptr poa)
  : test_i (orb,
            poa)
{
}

static const ACE_TCHAR *ior_output_file = ACE_TEXT ("ior");

static int
parse_args (int argc, ACE_TCHAR **argv)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("f:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'f':
        ior_output_file = get_opts.opt_arg ();
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
  FILE *output_file = ACE_OS::fopen (ior_output_file, "w");

  if (output_file == 0)
    ACE_ERROR_RETURN ((LM_ERROR, "Cannot open output files for writing IORs: %s\n",
                       ior_output_file),
                      -1);

  u_int result = 0;

  result = ACE_OS::fprintf (output_file,
                            "%s",
                            ior);
  ACE_OS::fclose (output_file);

  if (result != ACE_OS::strlen (ior))
    ACE_ERROR_RETURN ((LM_ERROR,
                       "ACE_OS::fprintf failed while writing %C to %s\n",
                       ior,
                       ior_output_file),
                      -1);

  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{

  try
    {
      // Initialize the ORB first.
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

      int result = parse_args (argc, argv);
      if (result != 0)
        return result;

      // Obtain the RootPOA.
      CORBA::Object_var obj =
        orb->resolve_initial_references ("RootPOA");

      // Get the POA_var object from Object_var.
      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (obj.in ());

      // Get the POAManager of the RootPOA.
      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager ();

      // Create a servant.
      reference_counted_test_i *servant = 0;
      ACE_NEW_RETURN (servant,
                      reference_counted_test_i (orb.in (),
                                                root_poa.in ()),
                      -1);

      // Get Object Reference for the servant object.
      test_var test = servant->_this ();

      // This means that the ownership of <servant> now belongs to
      // the POA.
      servant->_remove_ref ();

      // Stringyfy all the object references and print them out.
      CORBA::String_var ior = orb->object_to_string (test.in ());

      ACE_DEBUG ((LM_DEBUG,
                  "%C\n",
                  ior.in ()));

      int write_result = write_iors_to_file (ior.in ());
      if (write_result != 0)
        return write_result;

      poa_manager->activate ();

      orb->run ();

      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught");
      return -1;
    }

  return 0;
}
