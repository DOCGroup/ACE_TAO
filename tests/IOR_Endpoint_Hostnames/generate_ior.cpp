//
// This initializes an ORB, a POA, an Object within that POA, and
// obtains and prints an IOR for that Object.
//
// $Id$

#include "tao/corba.h"
#include "tao/PortableServer/PortableServer.h"
#include "ace/Get_Opt.h"

#include "bogus_i.h"

const ACE_TCHAR *ior_output_file = ACE_TEXT ("test.ior");

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("o:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':
        ior_output_file = get_opts.opt_arg ();
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-o <iorfile>"
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates successful parsing of the command line
  return 0;
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

      CORBA::Object_var poa_object =
        orb->resolve_initial_references ("RootPOA");

      PortableServer::POA_var rp =
        PortableServer::POA::_narrow(poa_object.in());
      if  (CORBA::is_nil (rp.in()))
        ACE_ERROR_RETURN ((LM_ERROR, "(%P|%t) panic: nil root poa\n"), 1);

      if (parse_args (argc, argv) != 0)
        return 1;

      bogus* bogus_impl = new bogus();
      PortableServer::ServantBase_var owner_transfer(bogus_impl);
      PortableServer::ObjectId_var id =
        rp->activate_object (bogus_impl);

      CORBA::Object_var object = rp->id_to_reference (id.in ());

      Test::bogus_var b = Test::bogus::_narrow (object.in ());
      CORBA::String_var ior =
        orb->object_to_string (b.in());

      // Output the IOR to the <ior_output_file>
      FILE *output_file= ACE_OS::fopen (ior_output_file, "w");
      if (output_file == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Cannot open output file for writing IOR: %s\n",
                           ior_output_file),
                           1);
      ACE_OS::fprintf (output_file, "%s", ior.in ());
      ACE_OS::fclose (output_file);

      orb->shutdown();
      orb->destroy();
    }
  catch (const CORBA::Exception& e)
    {
      e._tao_print_exception ("Caught exception:");
    }

  return 0;
}
