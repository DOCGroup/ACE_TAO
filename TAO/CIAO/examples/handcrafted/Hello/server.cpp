// $Id$

#include "ciao/Container_Base.h"
#include "hello_executors.h"
#include "hello_servants.h"
#include "ace/Get_Opt.h"

char *ior_file_name_ = 0;

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "o:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':  // get the file name to write to
       ior_file_name_ = get_opts.opt_arg ();
      break;

      case '?':  // display help for use of the server.
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s"
                           " [-o] <ior_output_file>"
                           "\n",
                           argv [0]),
                          -1);
      }

  return 0;
}

int
write_IOR(const char* ior)
{
  FILE* ior_output_file_ =
    ACE_OS::fopen (ior_file_name_, "w");

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
  if (parse_args (argc, argv) != 0)
    return -1;

  ACE_TRY_NEW_ENV
    {
      // Initialize orb
      CORBA::ORB_var orb = CORBA::ORB_init (argc,
                                            argv
                                            ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK

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

      // Start Deployment part

      CIAO::Session_Container container (orb);
      container.init ();
      CORBA::Object_var home;

      {
        // Create a valid HelloHome servant:
        Components::HomeExecutorBase_var ahome
          = createHelloHome_Impl ();

        // @@ Check is_nil

        CCM_HelloHome_var hellohome = CCM_HelloHome::_narrow (ahome);

        // @@ check is_nil again?

        PortableServer::Servant servant =
          new CIAO_HelloHome_Servant (hellohome, &container);

        home = container.install_servant (servant
                                          ACE_ENV_ARG_PARAMETER);
        ACE_TRY_CHECK;
      }

      // Start Home registration part
      CORBA::String_var str = orb->object_to_string (home.in ()
                                                     ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK

      write_IOR (str.in ());

      cout << str.in () << endl;
      // End Home registration part

      // End Deployment part

      ACE_DEBUG ((LM_DEBUG,
                  "Running generic server...\n"));

      // Run the main event loop for the ORB.
      orb->run (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "server::main\t\n");
      return 1;
    }
  ACE_ENDTRY;

  return 0;
}
