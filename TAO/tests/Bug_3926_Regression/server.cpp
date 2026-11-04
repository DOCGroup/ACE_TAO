// -*- C++ -*-
#include "test_i.h"

#include "ace/Get_Opt.h"
#include "ace/OS_NS_stdio.h"

#include <memory>
#include <string>

int
parse_args (int argc, ACE_TCHAR *argv[], std::basic_string<ACE_TCHAR> &ior_file)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("o:"));
  int option {};

  while ((option = get_opts ()) != -1)
    switch (option)
      {
      case 'o':
        ior_file = get_opts.opt_arg ();
        break;
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("Usage:  %s ")
                           ACE_TEXT ("-o IOR\n"),
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
      CORBA::ORB_var const orb {CORBA::ORB_init (argc, argv)};

      CORBA::Object_var const poa_object =
        orb->resolve_initial_references ("RootPOA");

      if (CORBA::is_nil (poa_object.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("SERVER: Unable to initialize the POA.\n")),
                          1);

      PortableServer::POA_var const root_poa =
        PortableServer::POA::_narrow (poa_object.in ());

      PortableServer::POAManager_var const poa_manager =
        root_poa->the_POAManager ();

      std::basic_string<ACE_TCHAR> ior_file {};
      if (::parse_args (argc, argv, ior_file) != 0)
        return -1;

      // Servant
      test_i *servant {};
      ACE_NEW_RETURN (servant,
                      test_i (0, orb.in ()),
                      -1);
      // The servant var takes ownership of the new servant reference.
      PortableServer::ServantBase_var safe (servant);

      PortableServer::ObjectId_var const oid =
        root_poa->activate_object (servant);

      CORBA::Object_var const obj =
        root_poa->servant_to_reference (servant);

      CORBA::String_var const ior =
        orb->object_to_string (obj.in ());

      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("SERVER: test_i servant: <%C>\n"),
                  ior.in ()));

      poa_manager->activate ();

      // IOR
      {
        std::unique_ptr<FILE, int (*) (FILE *)> const output_file (
          ACE_OS::fopen (ior_file.c_str (), "w"), ACE_OS::fclose);
        if (!output_file)
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("SERVER: Cannot open output file <%s> ")
                             ACE_TEXT ("for writing IOR: %C"),
                             ior_file.c_str (),
                             ior.in ()),
                            1);
        ACE_OS::fprintf (output_file.get (), "%s", ior.in ());
      }

      // Run the ORB event loop.
      orb->run ();

      root_poa->destroy (true, true);

      orb->destroy ();

      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("SERVER: Event loop finished.\n")));
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Caught exception:");
      return -1;
    }

  return 0;
}
