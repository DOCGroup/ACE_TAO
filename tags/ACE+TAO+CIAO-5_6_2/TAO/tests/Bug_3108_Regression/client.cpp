// $Id$

#include "testC.h"
#include "ace/Task.h"
#include "ace/Get_Opt.h"

const char *ior = "file://test.ior";

test_var global_client;
int iterations = 100;
bool server_shutdown = true;

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
        server_shutdown = false;
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-k <ior> "
                           "-x"
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates sucessful parsing of the command line
  return 0;
}


class Client : public ACE_Task_Base
{
public:
  // ctor
  Client (test_ptr server);

  virtual int svc (void);

  // The server.
  test_var server_;
};

Client::Client (test_ptr server)
  :  server_ (test::_duplicate (server))
{
}

int
Client::svc (void)
{
  try
    {
      this->server_->test_method();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Client: exception raised");
    }
  return 0;
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv) != 0)
        return 1;

      CORBA::Object_var obj = orb->string_to_object(ior);
      test_var server = test::_narrow(obj);
      if (CORBA::is_nil(server))
          {
            ACE_ERROR_RETURN ((LM_DEBUG,
                               "Nil reference <%s>\n",
                               ior),
                              1);
          }

      Client client (server.in ());
      if (client.activate (THR_NEW_LWP | THR_JOINABLE,
                            45) != 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                            "Cannot activate client threads\n"),
                          1);

      client.thr_mgr ()->wait ();

      if (server_shutdown)
        {
          server->shutdown ();
        }

      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught");
      return -1;
    }

  return 0;
}
