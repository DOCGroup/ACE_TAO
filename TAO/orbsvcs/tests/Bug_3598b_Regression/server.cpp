#include "Hello.h"
#include "ace/Get_Opt.h"
#include "ace/OS_NS_stdio.h"
#include "ace/SString.h"

#include <cstdio>
#include <memory>

struct Server_Options
{
  ACE_TString ior_output_file { ACE_TEXT ("test.ior") };
};

int
parse_args (int argc, ACE_TCHAR *argv[], Server_Options &options)
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
                             ACE_TEXT ("usage: %s -o <iorfile>\n"),
                             argv[0]),
                            -1);
        }
    }
  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var const orb = CORBA::ORB_init (argc, argv);
      Server_Options options;
      if (parse_args (argc, argv, options) != 0)
        return 1;

      CORBA::Object_var const poa_object =
        orb->resolve_initial_references ("RootPOA");

      PortableServer::POA_var const root_poa =
        PortableServer::POA::_narrow (poa_object.in ());

      if (CORBA::is_nil (root_poa.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT (" (%P|%t) Panic: nil RootPOA\n")),
                          1);

      Hello *hello_impl {};
      ACE_NEW_RETURN (hello_impl,
                      Hello (orb.in ()),
                      1);
      // ServantBase_var adopts the initial reference returned by ACE_NEW_RETURN.
      PortableServer::ServantBase_var owner_transfer (hello_impl);

      Test::Hello_var const hello =
        hello_impl->_this ();

      CORBA::String_var const ior =
        orb->object_to_string (hello.in ());

      {
        std::unique_ptr<FILE, int (*) (FILE *)> output_file (
          ACE_OS::fopen (options.ior_output_file.c_str (), ACE_TEXT ("w")), ACE_OS::fclose);
        if (!output_file)
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("Cannot open output file for writing IOR: %s"),
                             options.ior_output_file.c_str ()),
                            1);
        if (std::fprintf (output_file.get (), "%s", ior.in ()) < 0)
          ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("Cannot write IOR to output file\n")), 1);
      }

      root_poa->destroy (true, true);

      orb->destroy ();

      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Event loop finished.\n")));
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  return 0;
}
