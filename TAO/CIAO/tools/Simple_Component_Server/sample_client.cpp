// $Id$

#include "tools/Simple_Component_Server/Simple_ServerC.h"
#include "ace/Get_Opt.h"
#include "ace/Read_Buffer.h"
// @@ Include component stub file here:
//#include "helloC.h"

char *ior = 0;
int shutdown_server = 0;

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "i:s");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 's':                 // Shutdown server when exit?
        shutdown_server = 1;
        break;

      case 'i':                 // get component configuration
       ior = get_opts.opt_arg ();
      break;

      case '?':  // display help for use of the server.
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s\n"
                           "-i <simple server ior>\n"
                           "-s shutdown server when exit\n"
                           "\n",
                           argv [0]),
                          -1);
      }

  return 0;
}

int
main (int argc, char *argv[])
{
  ACE_TRY_NEW_ENV
    {
      // Initialize orb
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv ACE_ENV_ARG_PARAMETER);

      if (parse_args (argc, argv) != 0)
        return -1;

      CORBA::Object_var obj
        = orb->string_to_object (ior ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CIAO::Simple_Server_var cserve
        = CIAO::Simple_Server::_narrow (obj.in ()
                                        ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      /// @@ Narrow the object and do your stuff here.

      if (shutdown_server != 0)
        cserve->shutdown (ACE_ENV_SINGLE_ARG_PARAMETER);

      orb->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Who is the culprit \n");
      cerr << "Uncaught CORBA exception" << endl;
      return 1;
    }
  ACE_ENDTRY;

  return 0;
}
