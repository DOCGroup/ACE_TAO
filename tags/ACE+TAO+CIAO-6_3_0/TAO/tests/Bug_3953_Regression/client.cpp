// $Id$

#include "client_i.h"
#include "ace/Get_Opt.h"
#include "ace/Task.h"

static const ACE_TCHAR *ior = ACE_TEXT ("file://test.ior");
// Server IOR.

static int shutdown_server = 0;
// Flag to tell server to shutdown.

static CORBA::UShort call_count = 2;
// # of nested calls to be made.

static int quiet = 0;
// The test is quiet...

class Client_Task
{
public:
  Client_Task (client_ptr c,
               server_ptr s);
  int svc (void);

  bool exception (void) const;

private:
  client_var client_;
  server_var server_;
};

Client_Task::Client_Task (client_ptr c,
                          server_ptr s)
  : client_ (client::_duplicate (c)),
    server_ (server::_duplicate (s))
{
}

int
Client_Task::svc (void)
{
  try
    {
      if (!quiet)
        ACE_DEBUG ((LM_DEBUG,
                    "(%t) Client_Task::svc calling start -> time to live = %d\n",
                    call_count));

      // Now, we can invoke an operation on the remote side.
      this->server_->start (this->client_.in (),
                            call_count);
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Client_Task::svc");
      return -1;
    }
  return 0;
}

static int
parse_args (int argc,
            ACE_TCHAR **argv)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("qxn:k:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'q':
        quiet = 1;
        break;

      case 'x':
        shutdown_server = 1;
        break;

      case 'n':
        call_count = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case 'k':
        ior = get_opts.opt_arg ();
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s"
                           " [-n number of nested calls]"
                           " [-k ior]"
                           " [-q (quite)]"
                           " [-x (shutdown server)]"
                           "\n",
                           argv[0]),
                          -1);
      }

  if (ior == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%s: no nested up calls server ior specified\n",
                       argv[0]),
                      -1);

  // Indicates successful parsing of command line.
  return 0;
}

int
ACE_TMAIN (int argc,
      ACE_TCHAR **argv)
{
  int result = 0;
  try
    {
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

      result = parse_args (argc, argv);
      if (result != 0)
        return result;

      CORBA::Object_var object = orb->resolve_initial_references ("RootPOA");

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (object.in ());

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager ();

      poa_manager->activate ();

      object = orb->string_to_object (ior);

      server_var server = server::_narrow (object.in ());

      // Create an client object to hand to the other side...
      client_i client_servant (quiet,
                               server.in ());

      PortableServer::ObjectId_var id =
        root_poa->activate_object (&client_servant);

      CORBA::Object_var object_act = root_poa->id_to_reference (id.in ());

      client_var client_object = client::_narrow (object_act.in ());

      Client_Task client_tasks (client_object.in (),
                                server.in ());

      client_tasks.svc ();

      if (shutdown_server)
        {
          server->shutdown ();
        }

      root_poa->destroy (1, 1);

      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("client::main");
      return -1;
    }
  return result;
}
