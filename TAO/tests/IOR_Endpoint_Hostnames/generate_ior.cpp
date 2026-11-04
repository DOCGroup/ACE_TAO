//
// This initializes an ORB, a POA, an Object within that POA, and
// obtains and prints an IOR for that Object.
#include "tao/corba.h"
#include "tao/PortableServer/PortableServer.h"
#include "ace/Get_Opt.h"
#include <memory>

#include "bogus_i.h"

struct Server_Options
{
  ACE_TString ior_output_file {ACE_TEXT ("test.ior")};
};

int
parse_args (int argc, ACE_TCHAR *argv[], Server_Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("o:"));
  int c {};

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':
        options.ior_output_file = get_opts.opt_arg ();
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
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      Server_Options options;
      CORBA::ORB_var const orb = CORBA::ORB_init (argc, argv);

      CORBA::Object_var const poa_object =
        orb->resolve_initial_references ("RootPOA");

      PortableServer::POA_var const rp =
        PortableServer::POA::_narrow (poa_object.in ());
      if (CORBA::is_nil (rp.in ()))
        ACE_ERROR_RETURN ((LM_ERROR, "(%P|%t) panic: nil root poa\n"), 1);

      if (parse_args (argc, argv, options) != 0)
        return 1;

      bogus *bogus_impl {};
      ACE_NEW_RETURN (bogus_impl, bogus (), 1);
      // ServantBase_var takes ownership of this reference-counted TAO servant.
      PortableServer::ServantBase_var const owner_transfer (bogus_impl);
      PortableServer::ObjectId_var const id =
        rp->activate_object (bogus_impl);

      CORBA::Object_var const object = rp->id_to_reference (id.in ());

      Test::bogus_var const b = Test::bogus::_narrow (object.in ());
      CORBA::String_var const ior = orb->object_to_string (b.in ());

      // Output the IOR to the <ior_output_file>
      {
        std::unique_ptr<FILE, int (*) (FILE *)> output_file (
          ACE_OS::fopen (options.ior_output_file.c_str (), ACE_TEXT ("w")),
          ACE_OS::fclose);
        if (!output_file)
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("Cannot open output file for writing IOR: %s\n"),
                             options.ior_output_file.c_str ()),
                            1);
        ACE_OS::fprintf (output_file.get (), "%s", ior.in ());
      }

      orb->shutdown ();
      orb->destroy ();
    }
  catch (CORBA::Exception const &e)
    {
      e._tao_print_exception ("Caught exception:");
    }

  return 0;
}
