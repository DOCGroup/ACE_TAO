// -*- C++ -*-

#include "ace/Get_Opt.h"
#include "ace/OS_NS_stdio.h"
#include "Foo_i.h"

#include <memory>

struct Server_Options
{
  ACE_TCHAR const *ior_output_file {};
};

int
parse_args (int argc, ACE_TCHAR *argv[], Server_Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("o:"));
  int c;
  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':
        options.ior_output_file = get_opts.opt_arg ();
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("Usage: %s [-o <iorfile>]\n"),
                           argv[0]),
                          -1);
      }

  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);
      Server_Options options;
      if (parse_args (argc, argv, options) != 0)
        return 1;
      CORBA::Object_var obj = orb->resolve_initial_references ("RootPOA");
      if (CORBA::is_nil (obj.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("(%P|%t) ERROR: Unable to initialize the POA.\n")),
                          1);

      PortableServer::POA_var root_poa = PortableServer::POA::_narrow (obj.in ());
      PortableServer::POAManager_var poa_manager = root_poa->the_POAManager ();
      // Keep the servant's initial reference until _this () transfers it to the POA.
      Foo_i *server_impl {};
      ACE_NEW_RETURN (server_impl, Foo_i (orb.in ()), -1);
      PortableServer::ServantBase_var owner_transfer (server_impl);
      Foo_var server = server_impl->_this ();
      CORBA::String_var ior = orb->object_to_string (server.in ());

      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Activated as <%C>\n"), ior.in ()));
      if (options.ior_output_file != nullptr)
        {
          std::unique_ptr<FILE, int (*) (FILE *)> output_file (
            ACE_OS::fopen (options.ior_output_file, ACE_TEXT ("w")), ACE_OS::fclose);
          if (!output_file)
            ACE_ERROR_RETURN ((LM_ERROR,
                               ACE_TEXT ("Cannot open output file for writing IOR: %s\n"),
                               options.ior_output_file),
                              1);
          ACE_OS::fprintf (output_file.get (), "%s", ior.in ());
        }

      poa_manager->activate ();
      orb->run ();
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("\nEvent loop finished.\n")));
      root_poa->destroy (true, true);
      orb->destroy ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Caught exception:");
      return 1;
    }

  return 0;
}
