// -*- C++ -*-
#include <cstdio>
#include <memory>

#include "ace/Get_Opt.h"
#include "ace/OS_NS_stdio.h"
#include "test_i.h"

int
parse_args (int argc, ACE_TCHAR *argv[], ACE_TCHAR const *&ior_output_file)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("o:"));
  int option {};

  while ((option = get_opts ()) != -1)
    switch (option)
      {
      case 'o':
        ior_output_file = get_opts.opt_arg ();
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("usage:  %s ")
                           ACE_TEXT ("-o <iorfile>")
                           ACE_TEXT ("\n"),
                           argv [0]),
                          -1);
      }
  // Indicates successful parsing of the command line
  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR * argv[])
{
  ACE_TCHAR const *ior_output_file = ACE_TEXT ("test.ior");
  try
    {
      CORBA::ORB_var const orb = CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv, ior_output_file) != 0)
        return 1;

      ::Test::vtp_init *vtp_factory {};
      ACE_NEW_THROW_EX (vtp_factory,
                        ::Test::vtp_init,
                        CORBA::NO_MEMORY ());
      // The CORBA wrapper owns the factory; the raw pointer is borrowed below.
      CORBA::ValueFactoryBase_var const vtp_factory_owner = vtp_factory;

      orb->register_value_factory (vtp_factory->tao_repository_id (),
                                   vtp_factory);

      CORBA::Object_var const poa_object =
        orb->resolve_initial_references ("RootPOA");

      if (CORBA::is_nil (poa_object.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("Unable to initialize the POA.\n")),
                          1);

      PortableServer::POA_var const root_poa =
        PortableServer::POA::_narrow (poa_object.in ());

      PortableServer::POAManager_var const poa_manager =
        root_poa->the_POAManager ();

      poa_manager->activate ();

      A_i *server_impl {};
      ACE_NEW_RETURN (server_impl,
                      A_i (orb.in (), vtp_factory),
                      -1);
      // ServantBase_var takes ownership of the raw servant pointer.
      PortableServer::ServantBase_var safe (server_impl);

      PortableServer::ObjectId_var const id =
        root_poa->activate_object (server_impl);

      CORBA::Object_var const test_obj =
        root_poa->id_to_reference (id.in ());

      Test::A_var const server =
        Test::A::_narrow (test_obj.in ());

      CORBA::String_var const ior =
        orb->object_to_string (server.in ());

      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("Test::A: <%C>\n"),
                  ior.in ()));

      // If the ior_output_file exists, output the ior to it
      {
        std::unique_ptr<FILE, int (*) (FILE *)> output_file (
          ACE_OS::fopen (ior_output_file, "w"), ACE_OS::fclose);
        if (!output_file)
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("Cannot open output file ")
                             ACE_TEXT ("for writing IOR: %s"),
                             ior_output_file),
                            1);
        ACE_OS::fprintf (output_file.get (), "%s", ior.in ());
      }

      orb->run ();

      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("event loop finished\n")));

      root_poa->destroy (true, true);

      orb->destroy ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception (
                           "Caught exception in server:");
      return 1;
    }

  return 0;
}
