#include "ace/Get_Opt.h"
#include "ace/OS_NS_stdio.h"
#include "Test_i.h"
#include <memory>

#include "tao/IORTable/IORTable.h"

namespace
{
ACE_TCHAR const *object_key {};
ACE_TCHAR const *ior_file = ACE_TEXT ("server.ior");

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("o:k"));
  int c {};

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':
        ior_file = get_opts.opt_arg ();
        break;
      case 'k':
        object_key = get_opts.opt_arg ();
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "SERVER (%P): usage:  %s "
                           "-k <object key> "
                           "-o <ior>"
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates successful parsing of the command line
  return 0;
}
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var const orb =
        CORBA::ORB_init (argc, argv);

      CORBA::Object_var const poa_object =
        orb->resolve_initial_references ("RootPOA");

      if (CORBA::is_nil (poa_object.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           "SERVER (%P): Unable to initialize the POA.\n"),
                          1);

      PortableServer::POA_var const root_poa =
        PortableServer::POA::_narrow (poa_object.in ());

      PortableServer::POAManager_var const poa_manager =
        root_poa->the_POAManager ();

      if (parse_args (argc, argv) != 0)
        return 1;

      Test_i *server_impl {};
      ACE_NEW_RETURN (server_impl,
                      Test_i (orb.in ()),
                      1);
      PortableServer::ServantBase_var owner_transfer (server_impl);

      PortableServer::ObjectId_var const id =
        root_poa->activate_object (server_impl);

      CORBA::Object_var const object = root_poa->id_to_reference (id.in ());

      CORBA::Object_var const server = CORBA::Object::_narrow (object.in ());

      CORBA::String_var const ior =
        orb->object_to_string (server.in ());

      CORBA::Object_var const table_object =
        orb->resolve_initial_references ("IORTable");

      IORTable::Table_var const adapter =
        IORTable::Table::_narrow (table_object.in ());

      adapter->bind ("Name-with-hyphens", ior.in ());

      {
        std::unique_ptr<FILE, int (*) (FILE *)> output_file (
          ACE_OS::fopen (ior_file, "w"), ACE_OS::fclose);
        if (!output_file)
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("SERVER (%P): Cannot open output file "
                                       "for writing IOR: %s"),
                             ior_file),
                            1);
        ACE_OS::fprintf (output_file.get (), "%s", ior.in ());
      }

      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("SERVER (%P): Activated as file://%s\n"),
                  ior_file));

      poa_manager->activate ();

      orb->run ();

      root_poa->destroy (true, true);

      orb->destroy ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("SERVER (%P): Caught exception:");
      return 1;
    }

  return 0;
}
