#include "Messenger_i.h"
#include "ace/Get_Opt.h"
#include "ace/OS_NS_stdio.h"

#include <memory>
#include <string>

int
parse_args (int argc, ACE_TCHAR *argv[], std::basic_string<ACE_TCHAR> &ior_output_file)
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
      // Initialize orb
      CORBA::ORB_var const orb {CORBA::ORB_init (argc, argv)};
      std::basic_string<ACE_TCHAR> ior_output_file {ACE_TEXT ("Messenger.ior")};

      if (parse_args (argc, argv, ior_output_file) != 0)
        return 1;

      // Get reference to Root POA
      CORBA::Object_var const obj = orb->resolve_initial_references ("RootPOA");
      PortableServer::POA_var const poa = PortableServer::POA::_narrow (obj.in ());

      // Activate POA Manager
      PortableServer::POAManager_var const mgr = poa->the_POAManager ();
      mgr->activate ();

      // Create an object
      Messenger_i messenger_servant (orb.in ());

      // Register the servant, stringify its reference, and write it to a file.
      PortableServer::ObjectId_var const oid = poa->activate_object (&messenger_servant);
      CORBA::Object_var const messenger_obj = poa->id_to_reference (oid.in ());
      CORBA::String_var const str = orb->object_to_string (messenger_obj.in ());
      {
        std::unique_ptr<FILE, int (*) (FILE *)> const output_file (
          ACE_OS::fopen (ior_output_file.c_str (), "w"), ACE_OS::fclose);
        if (!output_file)
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Cannot open output file for writing IOR: %s\n",
                             ior_output_file.c_str ()),
                            1);
        ACE_OS::fprintf (output_file.get (), "%s", str.in ());
      }

      // Accept requests
      orb->run ();
      orb->destroy ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Caught CORBA exception: ");
      return 1;
    }

  return 0;
}
