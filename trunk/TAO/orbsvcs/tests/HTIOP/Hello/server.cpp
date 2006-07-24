// $Id$

#include "Hello.h"

#include "tests/test_config.h"

#include "ace/Get_Opt.h"
#include "tao/IORTable/IORTable.h"
#include "orbsvcs/CosNamingC.h"

ACE_RCSID (Hello,
           server,
           "$Id$")

const char *ior_output_file = "test.ior";

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "o:");
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
  // Indicates sucessful parsing of the command line
  return 0;
}

int
main (int argc, char *argv[])
{
  ACE_START_TEST (ACE_TEXT ("Hello_server"));

  ACE_TRY_NEW_ENV
    {
      ACE_DEBUG ((LM_DEBUG, "Begin of Hello_Server test\n"));

      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, "" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      //------ Get Root POA & POA Manager references

      CORBA::Object_var obj =
        orb->resolve_initial_references("RootPOA" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (obj.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (root_poa.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Panic: nil RootPOA\n"),
                          1);

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      //------- Get IOR Table reference to support CORBALOC URLs

      obj =
        orb->resolve_initial_references("IORTable" ACE_ENV_ARG_PARAMETER);

      IORTable::Table_var ior_table =
        IORTable::Table::_narrow(obj.in() ACE_ENV_ARG_PARAMETER);


      //------- Get NameService Root Context

      obj =
        orb->resolve_initial_references("NameService" ACE_ENV_ARG_PARAMETER);

      CosNaming::NamingContextExt_var root_nc =
        CosNaming::NamingContextExt::_narrow(obj.in() ACE_ENV_ARG_PARAMETER);

      //-------- Prepare Servant

      if (parse_args (argc, argv) != 0)
        return 1;

      Hello *hello_impl;
      ACE_NEW_RETURN (hello_impl,
                      Hello (orb.in ()),
                      1);
      PortableServer::ServantBase_var owner_transfer(hello_impl);

      Test::Hello_var hello =
        hello_impl->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::String_var ior =
        orb->object_to_string (hello.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

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
        ior_table->bind("HelloObj", ior.in() ACE_ENV_ARG_PARAMETER);

      // Output the IOR to the <ior_output_file>
      FILE *output_file= ACE_OS::fopen (ior_output_file, "w");
      if (output_file == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Cannot open output file for writing IOR: %s",
                           ior_output_file),
                              1);
      ACE_OS::fprintf (output_file, "%s", ior.in ());
      ACE_OS::fclose (output_file);

      poa_manager->activate (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      orb->run (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) server - event loop finished\n"));

      root_poa->destroy (1, 1 ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) server - Root poa destroyed\n"));

      orb->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) server - orb destroyed\n"));
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG, "server exiting\n"));
    }
  ACE_CATCHANY
    {
      ACE_END_TEST;
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Exception caught:");
      return 1;
    }
  ACE_ENDTRY;

  ACE_END_TEST;
  return 0;
}
