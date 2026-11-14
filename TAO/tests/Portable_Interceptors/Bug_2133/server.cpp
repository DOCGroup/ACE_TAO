#include "Hello.h"
#include "ace/Get_Opt.h"
#include "ace/SString.h"
#include "ace/OS_NS_stdio.h"
#include <memory>

struct Options
{
  ACE_TString ior_output_file { ACE_TEXT ("test.ior") };
};

int
parse_args (int argc, ACE_TCHAR *argv[], Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("o:"));
  int c {};

  while ((c = get_opts ()) != -1)
    {
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
    }
  // Indicates successful parsing of the command line
  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var const orb =
        CORBA::ORB_init (argc, argv);

      CORBA::Object_var poa_object =
        orb->resolve_initial_references ("RootPOA");

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (poa_object.in ());

      if (CORBA::is_nil (root_poa.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Panic: nil RootPOA\n"),
                          1);

      Options options;
      if (parse_args (argc, argv, options) != 0)
        return 1;

      Hello *hello_impl {};
      ACE_NEW_RETURN (hello_impl,
                      Hello (orb.in ()),
                      1);
      // Keep a typed pointer for _this; the ServantBase var owns the object.
      PortableServer::ServantBase_var owner_transfer (hello_impl);

      Test::Hello_var hello =
        hello_impl->_this ();

      CORBA::String_var const ior =
        orb->object_to_string (hello.in ());

      // Output the IOR to the configured file.
      {
        std::unique_ptr<FILE, int (*) (FILE *)> output_file (
          ACE_OS::fopen (options.ior_output_file.c_str (), "w"), ACE_OS::fclose);
        if (!output_file)
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Cannot open output file for writing IOR: %s",
                             options.ior_output_file.c_str ()),
                            1);
        ACE_OS::fprintf (output_file.get (), "%s", ior.in ());
      }

      root_poa->destroy (true, true);

      orb->destroy ();

      ACE_DEBUG ((LM_DEBUG, "Event loop finished.\n"));
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  return 0;
}
