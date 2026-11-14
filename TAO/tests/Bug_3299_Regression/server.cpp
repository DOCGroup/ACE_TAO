#include <cstdio>
#include <memory>
#include <string>

#include "Hello.h"
#include "ace/Get_Opt.h"
#include "ace/OS_NS_stdio.h"
#include "tao/IORTable/IORTable.h"

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
                           "usage:  %s "
                           "-o <iorfile>"
                           "\n",
                           argv [0]),
                          -1);
      }
  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  ACE_TCHAR const *ior_output_file = ACE_TEXT ("test.ior");
  try
    {
      CORBA::ORB_var const orb = CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv, ior_output_file) != 0)
        return 1;

      CORBA::Object_var const poa_object =
        orb->resolve_initial_references ("RootPOA");

      PortableServer::POA_var const root_poa =
        PortableServer::POA::_narrow (poa_object.in ());

      if (CORBA::is_nil (root_poa.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Panic: nil RootPOA\n"),
                          1);

      PortableServer::POAManager_var const poa_manager = root_poa->the_POAManager ();

      Hello *hello_impl {};
      ACE_NEW_RETURN (hello_impl,
                      Hello (orb.in ()),
                      1);
      // ServantBase_var takes ownership; hello_impl is a non-owning alias after this transfer.
      PortableServer::ServantBase_var owner_transfer (hello_impl);

      PortableServer::ObjectId_var const id =
        root_poa->activate_object (hello_impl);

      CORBA::Object_var const object = root_poa->id_to_reference (id.in ());

      Test::Hello_var const hello = Test::Hello::_narrow (object.in ());

      CORBA::String_var const ior = orb->object_to_string (hello.in ());

      CORBA::Object_var const ior_table_object = orb->resolve_initial_references ("IORTable");
      IORTable::Table_var const ior_table = IORTable::Table::_narrow (ior_table_object.in ());
      if (CORBA::is_nil (ior_table.in ()))
        {
          ACE_ERROR ((LM_ERROR, "could not get the IORTable, will not register\n"));
        }
      else
        {
          ior_table->rebind ("Hello", ior.in ());
        }

      // try and access the object with its friendly name
      std::string const full_corbaloc (ior.in ());

      std::string::size_type const first_slash = full_corbaloc.find ('/');

      std::string friendly_corbaloc = full_corbaloc.substr (0, first_slash);

      friendly_corbaloc += "/Hello";

      // Output the IOR to the <ior_output_file>
      {
        std::unique_ptr<FILE, int (*) (FILE *)> output_file (
          ACE_OS::fopen (ior_output_file, "w"), ACE_OS::fclose);
        if (!output_file)
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Cannot open output file for writing IOR: %s\n",
                             ior_output_file),
                            1);
        ACE_OS::fprintf (output_file.get (), "%s", friendly_corbaloc.c_str ());
      }

      poa_manager->activate ();

      orb->run ();

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) server - event loop finished\n"));

      root_poa->destroy (true, true);

      orb->destroy ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  return 0;
}
