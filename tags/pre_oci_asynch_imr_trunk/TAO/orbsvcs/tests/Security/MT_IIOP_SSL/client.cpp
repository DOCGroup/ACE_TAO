// $Id$

#include "ace/Get_Opt.h"
#include "Client_Worker.h"


const ACE_TCHAR *ior = ACE_TEXT("file://test.ior");
int niterations = 5;
int do_shutdown = 0;
int nthreads = 5;

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("k:n:i:x"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {

      case 'k':
        ior = get_opts.opt_arg ();
        break;

      case 'n':
        nthreads = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case 'i':
        niterations = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case 'x':
        do_shutdown = 1;
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-k <ior> "
                           "-n <nthreads>"
                           "-i <niterations> "
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
      // Initialize the ORB
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv) != 0)
        return 1;

      // Get Object Reference using IOR file
      CORBA::Object_var object =
        orb->string_to_object (ior);

      // Cast to Appropriate Type
      Simple_Server_var server =
        Simple_Server::_narrow (object.in ());

      if (CORBA::is_nil (server.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Object reference <%s> is nil\n", ior),
                              1);
        }


      Client_Worker client (server.in (), niterations);

      if (client.activate (THR_NEW_LWP | THR_JOINABLE, nthreads) != 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                         " (%P|%t) Cannot Activate Client Threads\n"),
                          1);

      client.thr_mgr ()->wait ();

      ACE_DEBUG ((LM_DEBUG,
                  "(%P|%t) threads finished\n"));

      // Shut down the server if -x option given in command line
      if (do_shutdown)
        {
          server->shutdown ();
        }

      // Destroying the ORB..
      orb->destroy ();
    }

  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }


  return 0;
}
