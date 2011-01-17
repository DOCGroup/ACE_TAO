// $Id$

#include "ace/Get_Opt.h"
#include "ace/Task.h"
#include "simple_testC.h"

const ACE_TCHAR *ior = ACE_TEXT("file://test.ior");
int server_shutdown = 0;

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("k:x"));
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
  // Indicates successful parsing of the command line
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
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv) != 0)
        return 1;

      CORBA::Object_var object =
        orb->string_to_object (ior);

      Simple_Server_var server =
        Simple_Server::_narrow (object.in ());

      if (CORBA::is_nil (server.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Object reference <%s> is nil.\n",
                             ior),
                            1);
        }

      // Invoke a request on the server
      CORBA::Boolean ret_value =
        server->print_status ();

      if (ret_value == 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "The server has been contacted !!\n",
                      0));
        }

      if (server_shutdown)
        {
          server->shutdown ();
        }

      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Caught exception:");
      return 1;
    }

  return 0;
}
