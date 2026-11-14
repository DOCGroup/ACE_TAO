#include "Multiple_Impl.h"
#include "ace/Get_Opt.h"
#include "ace/OS_NS_stdio.h"
#include <memory>
#include <string>

static int
parse_args (int argc,
            ACE_TCHAR *argv[],
            std::basic_string<ACE_TCHAR> &ior_output_file)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("o:"));
  int option {};

  while ((option = get_opts ()) != -1)
    switch (option)
      {
      case 'o':
        ior_output_file.assign (get_opts.opt_arg ());
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("usage:  %s -o <iorfile>\n"),
                           argv[0]),
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
      // Orb Initialization
      CORBA::ORB_var const orb = CORBA::ORB_init (argc, argv, "TAO");

      std::basic_string<ACE_TCHAR> ior_output_file {ACE_TEXT ("test.ior")};
      if (parse_args (argc, argv, ior_output_file) != 0)
        return 1;

      CORBA::Object_var const object = orb->resolve_initial_references ("RootPOA");

      PortableServer::POA_var const poa = PortableServer::POA::_narrow (object.in ());

      // Get the POAManager
      PortableServer::POAManager_var const poa_manager = poa->the_POAManager ();

      // Create the servant.
      Bottom_Impl servant (orb.in ());

      // Create the delegated servant and intialize it
      // with the "real" servant.
      PortableServer::ObjectId_var id = poa->activate_object (&servant);
      CORBA::Object_var object_act = poa->id_to_reference (id.in ());
      Delegated_Bottom_Impl delegated_servant (
        Multiple::Bottom::_narrow (object_act.in ()));

      // Create the CORBA Object that is incarnated by the
      // delegated servant.
      id = poa->activate_object (&delegated_servant);
      object_act = poa->id_to_reference (id.in ());

      Multiple::Bottom_var const bottom =
        Multiple::Bottom::_narrow (object_act.in ());

      // Now we stringify the object reference.
      CORBA::String_var const ior =
        orb->object_to_string (bottom.in ());

      ACE_DEBUG ((LM_DEBUG, "Activated as <%C>\n", ior.in ()));

      // If the ior_output_file exists, output the ior to it

      {
        std::unique_ptr<FILE, int (*) (FILE *)> const output_file (
          ACE_OS::fopen (ior_output_file.c_str (), ACE_TEXT ("w")), ACE_OS::fclose);
        if (!output_file)
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("Cannot open output file for writing IOR: %s\n"),
                             ior_output_file.c_str ()),
                            1);
        ACE_OS::fprintf (output_file.get (), "%s", ior.in ());
      }

      // Activate the POAManager
      poa_manager->activate ();

      orb->run ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception (
        "Multiple Execution Interrupted Exception!\n");
      return 1;
    }
  return 0;
}
