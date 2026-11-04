#include "ace/Get_Opt.h"
#include "ace/Task.h"
#include "testS.h"
#include "ace/OS_NS_unistd.h"

#include <string>

/**
 * Simpler Server Client implementation
 *
 * Implements the Simple_Server interface in test.idl
 */
class Simple_Server_i : public POA_Simple_Server
{
public:
  explicit Simple_Server_i (CORBA::ORB_ptr orb)
    : orb_ (CORBA::ORB::_duplicate (orb)) {}

  Simple_Server_i (Simple_Server_i const &) = delete;
  Simple_Server_i (Simple_Server_i &&) = delete;
  Simple_Server_i &operator= (Simple_Server_i const &) = delete;
  Simple_Server_i &operator= (Simple_Server_i &&) = delete;

  // = The Simple_Server methods.
  char *test_method (Simple_Server_ptr objref) override
  {
    ACE_DEBUG ((LM_DEBUG, "Client test_method() - sleeping\n"));
    ACE_OS::sleep (3);
    if (!CORBA::is_nil (objref))
      {
        ACE_DEBUG ((LM_DEBUG, "Client test_method() - signalling server\n"));
        objref->client_done ();
      }
    ACE_DEBUG ((LM_DEBUG, "Client test_method() - returning\n"));
    return CORBA::string_dup ("hello from client");
  }

  void client_done () override
  {
    ACE_ERROR ((LM_ERROR, "Unexpected call\n"));
  }

  void shutdown () override
  {
  }

private:
  CORBA::ORB_var orb_;
  // The ORB
};


static int
parse_args (int argc,
            ACE_TCHAR *argv[],
            std::basic_string<ACE_TCHAR> &ior,
            bool &do_shutdown)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("xk:s:"));
  int c {};

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'x':
        do_shutdown = true;
        break;

      case 'k':
        ior = get_opts.opt_arg ();
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("usage: %s "
                                     "-k <ior> "
                                     "[-x]"
                                     "\n"),
                           argv [0]),
                          -1);
      }
  // Indicates successful parsing of the command line
  return 0;
}

/**
 * Run a server thread
 *
 * Use the ACE_Task_Base class to run server threads
 */
class Worker : public ACE_Task_Base
{
public:
  explicit Worker (CORBA::ORB_ptr orb);
  Worker (Worker const &) = delete;
  Worker (Worker &&) = delete;
  Worker &operator= (Worker const &) = delete;
  Worker &operator= (Worker &&) = delete;

  int svc () override;
  // The thread entry point.

private:
  CORBA::ORB_var orb_;
  // The orb
};

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      std::basic_string<ACE_TCHAR> ior {ACE_TEXT ("file://test.ior")};
      bool do_shutdown {};
      CORBA::ORB_var const orb {CORBA::ORB_init (argc, argv)};

      CORBA::Object_var const poa_object {orb->resolve_initial_references ("RootPOA")};

      if (CORBA::is_nil (poa_object.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to initialize the POA.\n"),
                          1);

      PortableServer::POA_var const root_poa {PortableServer::POA::_narrow (poa_object.in ())};

      PortableServer::POAManager_var const poa_manager {root_poa->the_POAManager ()};

      if (parse_args (argc, argv, ior, do_shutdown) != 0)
        return 1;

      Simple_Server_i server_impl (orb.in ());

      PortableServer::ObjectId_var const id {root_poa->activate_object (&server_impl)};

      CORBA::Object_var const object {root_poa->id_to_reference (id.in ())};

      Simple_Server_var const server {Simple_Server::_narrow (object.in ())};

      CORBA::String_var const local_ior {orb->object_to_string (server.in ())};
      ACE_DEBUG ((LM_DEBUG,
                  "Client interface started on <%C>\n",
                  local_ior.in ()));
      CORBA::Object_var const remote_object {
        orb->string_to_object (ACE_TEXT_ALWAYS_CHAR (ior.c_str ())) };

      Simple_Server_var const remote_server {Simple_Server::_narrow (remote_object.in ())};

      if (CORBA::is_nil (remote_server.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("Object reference <%s> is nil\n"),
                             ior.c_str ()), 1);
        }

      if (do_shutdown)
        {
          remote_server->shutdown ();
        }
      else
        {
          poa_manager->activate ();
          Worker worker (orb.in ());
          if (worker.activate (THR_NEW_LWP | THR_JOINABLE,
                               1) != 0)
            ACE_ERROR_RETURN ((LM_ERROR,
                               "Cannot activate client thread\n"),
                              1);
          ACE_DEBUG ((LM_DEBUG, "ORB Thread started\n"));
          CORBA::String_var const str {remote_server->test_method (server.in ())};
          ACE_DEBUG ((LM_DEBUG, "Received \"%C\"\n", str.in ()));
          orb->shutdown (true);
          worker.thr_mgr ()->wait ();
          ACE_DEBUG ((LM_DEBUG, "ORB Thread completed\n"));
        }
      orb->destroy ();
    }
  catch (CORBA::Exception const &ex)
    {
      ACE_DEBUG ((LM_ERROR, ACE_TEXT ("(%P|%t) Exception caught: \n%s\n"),
                  ACE_TEXT_CHAR_TO_TCHAR (ex._info ().c_str ())));
      return 1;
    }

  return 0;
}

// ****************************************************************

Worker::Worker (CORBA::ORB_ptr orb)
  :  orb_ (CORBA::ORB::_duplicate (orb))
{
}

int
Worker::svc ()
{
  try
    {
      this->orb_->run ();
    }
  catch (CORBA::Exception const &)
    {
    }
  return 0;
}
