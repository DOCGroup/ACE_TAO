// $Id$

#include "ace/Get_Opt.h"
#include "ace/Task.h"
#include "simple_testC.h"

ACE_RCSID(tests, client, "$Id$")

const char *ior = "file://test.ior";
int server_shutdown = 0;

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "k:x");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        ior = get_opts.opt_arg ();
        break;
      case 'x':
        server_shutdown = 1;
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

class Client
{
  // = TITLE
  //     Defines a class that is used to test and understand the
  //     different ways of loading the default and advanced TAO
  //     strategies.
  //
  // = DESCRIPTION
  //     A simple client which receives the CORBA boolean variabel
  //     from the server to indicate that everything went on good.
  //
public:
  Client ();
  // ctor

};

// ****************************************************************

Client::Client ()
{
}

int
main (int argc, char *argv[])
{
  ACE_TRY_NEW_ENV
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, "" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (parse_args (argc, argv) != 0)
        return 1;

      CORBA::Object_var object =
        orb->string_to_object (ior ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      Simple_Server_var server =
        Simple_Server::_narrow (object.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (server.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Object reference <%s> is nil\n",
                             ior),
                            1);
        }

      // Invoke a request on the server
      CORBA::Boolean ret_value =
        server->print_status (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (ret_value == 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "The server has been contacted !!\n",
                      0));
        }

      if (server_shutdown)
        {
          server->shutdown (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }

      orb->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Caught exception:");
      return 1;
    }
  ACE_ENDTRY;

  return 0;
}
