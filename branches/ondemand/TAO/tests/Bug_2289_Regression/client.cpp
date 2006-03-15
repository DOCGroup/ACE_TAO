//
// $Id$
//
#include "TestS.h"
#include "MyInterfaceImpl.h"
#include "ace/Get_Opt.h"
#include "ace/OS_NS_stdio.h"

const char *server_ior = "";

const char *ior_output_file = "client.ior";

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "k:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        server_ior = get_opts.opt_arg ();
        break;
      case 'o':
        ior_output_file = get_opts.opt_arg ();
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-k <ior> "
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
  int result = 0;
  ACE_TRY_NEW_ENV
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, "" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (parse_args (argc, argv) != 0)
        return 1;

      CORBA::Object_var poa_object =
        orb->resolve_initial_references("RootPOA" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (poa_object.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (root_poa.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Panic: nil RootPOA\n"),
                          1);

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (parse_args (argc, argv) != 0)
        return 1;

      MyInterfaceImpl *test_impl;
      ACE_NEW_RETURN (test_impl,
                      MyInterfaceImpl (orb.in ()),
                      1);

      PortableServer::ServantBase_var owner_transfer(test_impl);

      MyInterface_var test_ref =
        test_impl->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::String_var ior =
        orb->object_to_string (test_ref.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Output the IOR to the <ior_output_file>
      FILE *output_file= ACE_OS::fopen (ior_output_file, "w");
      if (output_file != 0)
        {
          ACE_OS::fprintf (output_file, "%s", ior.in ());
          ACE_OS::fclose (output_file);
        }

      poa_manager->activate (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK

      CORBA::Object_var tmp =
        orb->string_to_object(server_ior ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      MyInterface_var server =
        MyInterface::_narrow(tmp.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (server.in ()))
        {
          ACE_ERROR_RETURN ((LM_DEBUG,
                             "Nil server reference <%s>\n",
                             ior.in()),
                            1);
        }
      

      CORBA::Boolean temp = server->myMethod (MyInterfaceImpl::my_string ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      
      if (temp)
        {
          ACE_DEBUG ((LM_DEBUG, "Test succeeded\n"));
	}
      else
        {
	  ACE_DEBUG ((LM_ERROR, "Test failed\n"));
	  result = 1;
	}	

      //hello->shutdown (ACE_ENV_SINGLE_ARG_PARAMETER);
      //ACE_TRY_CHECK;

      orb->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Exception caught:");
      return 1;
    }
  ACE_ENDTRY;

  return result;
}
