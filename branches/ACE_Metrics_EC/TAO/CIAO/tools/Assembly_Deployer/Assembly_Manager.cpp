// $Id$

/**
 * @file Assembly_Manager.cpp
 *
 * @brief CIAO's Assembly managing process.
 *
 */

#include "Assembly_Impl.h"
#include "Assembly_Service_Impl.h"
#include "CIAO_ServersC.h"
#include "Server_init.h"
#include "tao/IORTable/IORTable.h"
#include "ace/SString.h"
#include "ace/Read_Buffer.h"
#include "ace/Get_Opt.h"

char *config_filename_ = 0;
char *output_ior_filename_ = 0;

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "c:o:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'c': // Get the configuration file for deployment configuration info.
        config_filename_ = get_opts.opt_arg ();
        break;

      case 'o':  // get the file name to write to
       output_ior_filename_ = get_opts.opt_arg ();
      break;

      case '?':  // display help for use of the server.
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s\n"
                           "-c <deployment configuration file>\n"
                           "-o <ior_output_file>\n"
                           "\n",
                           argv [0]),
                          -1);
      }

  if (config_filename_ == 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "Require a deployment configuration flag.  Use -c to set it \n"
                         ),
                        -1);
    }

  return 0;
}

int
write_IOR(const char* ior)
{
  FILE* ior_output_file_ =
    ACE_OS::fopen (output_ior_filename_, "w");

  if (ior_output_file_)
    {
      ACE_OS::fprintf (ior_output_file_,
                       "%s",
                       ior);
      ACE_OS::fclose (ior_output_file_);
    }

  return 0;
}

int
main (int argc, char *argv[])
{
  ACE_TRY_NEW_ENV
    {
      // Initialize orb
      CORBA::ORB_var orb = CORBA::ORB_init (argc,
                                            argv
                                            ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CIAO::Server_init (orb.in ());

      if (parse_args (argc, argv) != 0)
        return -1;

      // Get reference to Root POA.
      CORBA::Object_var obj
        = orb->resolve_initial_references ("RootPOA"
                                           ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      PortableServer::POA_var poa
        = PortableServer::POA::_narrow (obj.in ()
                                        ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Activate POA manager
      PortableServer::POAManager_var mgr
        = poa->the_POAManager (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      mgr->activate (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CIAO::AssemblyFactory_Impl *assembly_factory_servant;

      ACE_NEW_RETURN (assembly_factory_servant,
                      CIAO::AssemblyFactory_Impl (orb.in (),
                                                  poa.in ()),
                      -1);

      PortableServer::ServantBase_var safe_servant (assembly_factory_servant);

      assembly_factory_servant->init (config_filename_
                                      ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Configuring AssemblyFactory.
      PortableServer::ObjectId_var cs_oid
        = poa->activate_object (assembly_factory_servant
                                ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      obj = poa->id_to_reference (cs_oid
                                  ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      Components::Deployment::AssemblyFactory_var assembly_factory_obj =
        Components::Deployment::AssemblyFactory::_narrow (obj.in ()
                                                          ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (assembly_factory_obj.in ()))
        ACE_ERROR_RETURN ((LM_ERROR, "Unable to activate AssemblyFactory object\n"), -1);


      CORBA::String_var str = orb->object_to_string (assembly_factory_obj.in ()
                                                     ACE_ENV_ARG_PARAMETER);

      CORBA::Object_var table_object =
        orb->resolve_initial_references ("IORTable"
                                         ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      IORTable::Table_var adapter =
        IORTable::Table::_narrow (table_object.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (adapter.in ()))
        {
          ACE_ERROR ((LM_ERROR, "Nil IORTable\n"));
        }
      else
        {
          adapter->bind ("AssemblyFactory", str.in () ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }

      // Now create the assembly service object
      CIAO::Assembly_Service_Impl *service_servant;
      ACE_NEW_RETURN (service_servant,
                      CIAO::Assembly_Service_Impl (orb.in (),
                                                   poa.in (),
                                                   assembly_factory_obj.in ()),
                      -1);

      safe_servant = service_servant;

      PortableServer::ObjectId_var as_oid
        = poa->activate_object (service_servant
                                ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      obj = poa->id_to_reference (as_oid
                                  ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CIAO::Assembly_Service_var service_obj =
        CIAO::Assembly_Service::_narrow (obj.in ()
                                         ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (service_obj.in ()))
        ACE_ERROR_RETURN ((LM_ERROR, "Unable to activate Assembly_Service object\n"), -1);


      str = orb->object_to_string (service_obj.in ()
                                   ACE_ENV_ARG_PARAMETER);

      write_IOR (str.in ());
      ACE_DEBUG ((LM_INFO, "Assembly_Manager IOR: %s\n", str.in ()));

      // End Deployment part

      ACE_DEBUG ((LM_DEBUG,
                  "Running Assembly_Manager...\n"));

      // Run the main event loop for the ORB.
      orb->run (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Assembly_Manager::main\t\n");
      return 1;
    }
  ACE_ENDTRY;

  return 0;
}
