#include "client_i.h"
#include "ace/Get_Opt.h"
#include "ace/Task.h"

#include <string>

static std::basic_string<ACE_TCHAR> ior {ACE_TEXT ("file://test.ior")};
// Server IOR.

static bool shutdown_server {};
// Flag to tell server to shutdown.

static CORBA::UShort call_count {2};
// # of nested calls to be made.

static bool quiet {};
// The test is quiet...

class Client_Task
{
public:
  Client_Task (client_ptr const c,
               server_ptr const s);
  Client_Task (Client_Task const &) = delete;
  Client_Task (Client_Task &&) = delete;
  Client_Task &operator= (Client_Task const &) = delete;
  Client_Task &operator= (Client_Task &&) = delete;
  int svc ();

private:
  client_var client_ {};
  server_var server_ {};
};

Client_Task::Client_Task (client_ptr const c,
                          server_ptr const s)
  : client_ (client::_duplicate (c)),
    server_ (server::_duplicate (s))
{
}

int
Client_Task::svc ()
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
  catch (CORBA::Exception const &ex)
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
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("qxn:k:"));
  int option {};

  while ((option = get_opts ()) != -1)
    switch (option)
      {
      case 'q':
        quiet = true;
        break;

      case 'x':
        shutdown_server = true;
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

  // Indicates successful parsing of command line.
  return 0;
}

int
ACE_TMAIN (int argc,
           ACE_TCHAR **argv)
{
  try
    {
      CORBA::ORB_var const orb {CORBA::ORB_init (argc, argv)};

      int const result = parse_args (argc, argv);
      if (result != 0)
        return result;

      CORBA::Object_var object {orb->resolve_initial_references ("RootPOA")};

      PortableServer::POA_var const root_poa =
        PortableServer::POA::_narrow (object.in ());

      PortableServer::POAManager_var const poa_manager =
        root_poa->the_POAManager ();

      poa_manager->activate ();

      object = orb->string_to_object (ACE_TEXT_ALWAYS_CHAR (ior.c_str ()));

      server_var const server = server::_narrow (object.in ());

      // Create an client object to hand to the other side...
      client_i client_servant (quiet,
                               server.in ());

      PortableServer::ObjectId_var const id =
        root_poa->activate_object (&client_servant);

      CORBA::Object_var const object_act = root_poa->id_to_reference (id.in ());

      client_var const client_object = client::_narrow (object_act.in ());

      Client_Task client_tasks (client_object.in (), server.in ());

      client_tasks.svc ();

      if (shutdown_server)
        {
          server->shutdown ();
        }

      root_poa->destroy (true, true);

      orb->destroy ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("client::main");
      return -1;
    }
  return 0;
}
