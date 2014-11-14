// $Id$

#include "Hello.h"

#include "ace/Get_Opt.h"
#include "tao/IORTable/IORTable.h"
#include "orbsvcs/CosNamingC.h"

const ACE_TCHAR *ior_output_file = ACE_TEXT("test.ior");

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("o:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
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
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
    {
      ACE_DEBUG ((LM_DEBUG, "Begin of Hello_Server test\n"));

      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      //------ Get Root POA & POA Manager references

      CORBA::Object_var obj =
        orb->resolve_initial_references("RootPOA");

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (obj.in ());

      if (CORBA::is_nil (root_poa.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Panic: nil RootPOA\n"),
                          1);

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager ();

      //------- Get IOR Table reference to support CORBALOC URLs

      obj =
        orb->resolve_initial_references("IORTable");

      IORTable::Table_var ior_table =
        IORTable::Table::_narrow(obj.in());


      //------- Get NameService Root Context

      obj =
        orb->resolve_initial_references("NameService");

      CosNaming::NamingContextExt_var root_nc =
        CosNaming::NamingContextExt::_narrow(obj.in());

      //-------- Prepare Servant

      if (parse_args (argc, argv) != 0)
        return 1;

      Hello *hello_impl;
      ACE_NEW_RETURN (hello_impl,
                      Hello (orb.in ()),
                      1);
      PortableServer::ServantBase_var owner_transfer(hello_impl);

      Test::Hello_var hello =
        hello_impl->_this ();

      CORBA::String_var ior =
        orb->object_to_string (hello.in ());

      //--------- Publish IOR using various means

      if (!CORBA::is_nil(root_nc.in()))
        {
          CosNaming::Name_var objname= root_nc->to_name("HelloObj");
          root_nc->rebind (objname.in(),hello.in());
        }
      else
        {
          ACE_DEBUG ((LM_DEBUG,"Could not initialize name service\n"));
        }

      if (!CORBA::is_nil(ior_table.in()))
        ior_table->bind("HelloObj", ior.in());

      // Output the IOR to the <ior_output_file>
      FILE *output_file= ACE_OS::fopen (ior_output_file, "w");
      if (output_file == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Cannot open output file for writing IOR: %s",
                           ior_output_file),
                              1);
      ACE_OS::fprintf (output_file, "%s", ior.in ());
      ACE_OS::fclose (output_file);

      poa_manager->activate ();

      orb->run ();

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) server - event loop finished\n"));

      root_poa->destroy (1, 1);

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) server - Root poa destroyed\n"));

      orb->destroy ();

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) server - orb destroyed\n"));
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  return 0;
}
