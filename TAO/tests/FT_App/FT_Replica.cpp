// $Id$
//FT_Replica.cpp

#include "FT_TestReplica_i.h"
#include "ace/Get_Opt.h"
#include "orbsvcs/CosNamingC.h"

namespace {

  const char *ior_output_file = 0;

  const char * nsName = "FT_TEST";
  int
  write_IOR(const char * fileName, const char* ior)
  {
    int result = -1;
    FILE* out = ACE_OS::fopen (fileName, "w");
    if (out)
    {
      ACE_OS::fprintf (out, "%s", ior);
      ACE_OS::fclose (out);
      result = 0;
    }
    return result;
  }


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



}


int main (int argc, char * argv[] )
{
  int result = parse_args (argc, argv);

  if (result == 0)
  {

    ACE_TRY_NEW_ENV
    {

      // Create an object that manages all the
      // details of being a server.
      TAO_ORB_Manager orbManager;

      result = orbManager.init (argc, argv
          ACE_ENV_ARG_PARAMETER);
      if(result == 0)
      {
        ACE_CHECK_RETURN (-1);

        CORBA::ORB_var orb = orbManager.orb();
        FT_TestReplica_i ftReplica(orb);

        // Register with the ORB.
        CORBA::String_var ftReplicaIOR =
          orbManager.activate (&ftReplica
            ACE_ENV_ARG_PARAMETER);
        ACE_CHECK_RETURN (-1);


        CORBA::Object_var naming_obj =
          orb->resolve_initial_references ("NameService" ACE_ENV_ARG_PARAMETER);
        ACE_TRY_CHECK;

        if (CORBA::is_nil(naming_obj.in ())){
          ACE_ERROR_RETURN ((LM_ERROR,
                             " (%P|%t) Unable to find the Naming Service\n"),
                            1);
        }

        CosNaming::NamingContext_var naming_context =
          CosNaming::NamingContext::_narrow (naming_obj.in () ACE_ENV_ARG_PARAMETER);
        ACE_TRY_CHECK;

        CosNaming::Name replica_name (1);
        replica_name.length (1);
        replica_name[0].id = CORBA::string_dup (nsName);

        FT_TEST::TestReplica_var replicaVar = ftReplica._this();
        // Register the replica with the Naming Context....
        naming_context->rebind (replica_name, replicaVar
                                ACE_ENV_ARG_PARAMETER);
        ACE_TRY_CHECK;

        if (ior_output_file != 0)
        {
          result = write_IOR(ior_output_file, ftReplicaIOR);
        }
        if (result == 0)
        {
          std::cout << "Running Fault Tolerant Replica. ";
          if (nsName != 0)
          {
            std::cout << " name:" << nsName;
          }
          if (ior_output_file)
          {
            std::cout << " file:" << ior_output_file;
          }
          std::cout << std::endl;

          // Run the main event loop for the ORB.
          result = orbManager.run (ACE_ENV_SINGLE_ARG_PARAMETER);
          if (result == -1)
          {
            ACE_ERROR_RETURN (
              (LM_ERROR, "FT_Replica_i::run"),
              -1);
          }
          ACE_TRY_CHECK;
          std::cout << "Fault Tolerant Replica terminated normally.";
          if (nsName != 0)
          {
            std::cout << " name:" << nsName;
          }
          if (ior_output_file)
          {
            std::cout << " file:" << ior_output_file;
          }
          std::cout << std::endl;
        }
        else
        {
          ACE_ERROR_RETURN (
            (LM_ERROR, "%p\n", "Write IOR failed\n"),
            -1);
        }
      }
      else
      {
        ACE_ERROR_RETURN (
          (LM_ERROR, "%p\n", "orb manager init failed\n"),
          -1);
      }
    }
    ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
        "server::main\t\n");
      result = -1;
    }
    ACE_ENDTRY;
  }
  return result;
}
