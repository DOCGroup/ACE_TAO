// $Id$

#include "tools/Simple_Component_Server/Simple_ServerC.h"
#include "ace/Get_Opt.h"
#include "ace/Read_Buffer.h"
#include "helloC.h"

char *ior = 0;
int shutdown_server = 0;

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "i:x");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'x':                 // Shutdown server when exit?
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
                           "-x shutdown server when exit\n"
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

      Components::CCMHome_var home = cserve->get_home (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      HelloHome_var hh = HelloHome::_narrow (home.in ()
                                             ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if  (CORBA::is_nil (hh.in ()))
        ACE_ERROR_RETURN ((LM_ERROR, "Argument is not a HelloHome reference\n"), -1);

      HelloWorld_var hw = hh->create ();

      if  (CORBA::is_nil (hw.in ()))
        ACE_ERROR_RETURN ((LM_ERROR, "Fail to create a HelloWorld reference\n"), -1);

      CORBA::String_var hi = hw->sayhello ("Simple_Component_Server"
                                           ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_INFO, "%s\n", hi.in ()));

      hh->remove_component (hw.in ()
                            ACE_ENV_ARG_PARAMETER);

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
